#include "external/glad.h"
#include <SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include "sm/shader.h"
#include "sm/maker/window.h"
#include "sm/maker/savegraph.h"
#include "sm/model.h"
#include "sm/camera.h"
#include "sm/transform.h"
#include "sm/texture.h"
#include "sm/shaderGenerator.h"
#include "sm/framebuffer.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

struct Material {
    float ambient = 1.0f;
    float diffuse = 0.5f;
    float specular = 0.5f;
    float shininess = 128.f;
} material;

int main(int argc, char *argv[]) {
    SDL_Log("%s", "main()");

    // SDL setup
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Window + context acquisition
    SDL_Window *window = SDL_CreateWindow("ShaderMaker AG XO NS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize window: %s", SDL_GetError());
        return 1;
    }

    // Load OpenGL Context
    SDL_GL_LoadLibrary(nullptr);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create OpenGL context: %s", SDL_GetError());
        return 1;
    }

    // Load OpenGL
    int version = gladLoadGLLoader(SDL_GL_GetProcAddress);
    if (version == 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize OpenGL context");
        return -1;
    }

    // Load IMGUI
    ImGuiContext *imguiContext = ImGui::CreateContext();
    ImGui::GetIO().DisplaySize = ImVec2{SCREEN_WIDTH, SCREEN_HEIGHT};
    ImGui::GetIO().Fonts->Build();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();

    // GRAPHICS INITIALIZATION //

    // Enable backface cullin'
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST); // Depth testing

    // The stuff lol
    {
        // Set up ShaderMaker
        sm::maker::Window maker{
                std::vector<sm::maker::PinSpec>{ // inputs
                        {"u_sMaterial.nAmbient",   sm::maker::PinType::FLOAT},
                        {"u_sMaterial.nDiffuse",   sm::maker::PinType::FLOAT},
                        {"u_sMaterial.nSpecular",  sm::maker::PinType::FLOAT},
                        {"u_sMaterial.nShininess", sm::maker::PinType::FLOAT},
                        {"in_sIn.vPosition",       sm::maker::PinType::VEC3},
                        {"in_sIn.vNormal",         sm::maker::PinType::VEC3},
                        {"in_sIn.vUv",             sm::maker::PinType::VEC2},
                        {"in_sIn.matTBN",          sm::maker::PinType::MAT3},
                        {"u_texMain",              sm::maker::PinType::SAMPLER2D},
                        {"u_texNormal",            sm::maker::PinType::SAMPLER2D},
                        {"u_vLightDirection",      sm::maker::PinType::VEC3},
                        {"u_vLightColor",          sm::maker::PinType::VEC3},
                        {"u_vAmbientColor",        sm::maker::PinType::VEC3}
                },
                std::vector<sm::maker::PinSpec>{ // outputs
                        {"out_vColor", sm::maker::PinType::VEC4},
                },
        };
        ax::NodeEditor::NodeId outputNodeId{};
        std::string generatedCode{};
        bool shouldUsePostProcess = false;

        // Load and bind the textures
        auto *roofColor = new sm::Texture("assets/textures/roof_color.png");
        auto *roofNormal = new sm::Texture("assets/textures/roof_normal.png");

        roofColor->bindToTextureUnit(0);
        roofNormal->bindToTextureUnit(1);

        // Load the lighting shader
        sm::Shader *shader = sm::loadShader("assets/albedo.vert", "assets/albedo.frag");
        shader->use();
        shader->setFloat("u_sMaterial.nAmbient", material.ambient);
        shader->setFloat("u_sMaterial.nDiffuse", material.diffuse);
        shader->setFloat("u_sMaterial.nSpecular", material.specular);
        shader->setFloat("u_sMaterial.nShininess", material.shininess);
        shader->setInt("u_texMain", 0);
        shader->setInt("u_texNormal", 1);

        //Shader Generators
        sm::ShaderGenerator shaderGenLit("assets/lit.vert", "assets/litTemplate.frag"); //for lit vert and frag
        sm::ShaderGenerator shaderGenPost("assets/postProcess.vert", "assets/postTemplate.frag"); //for post process vert and frag

        // Camera
        auto *camera = new sm::Camera();
        camera->position = glm::vec3(0.0f, 0.0f, 5.0f);
        camera->target = glm::vec3(0.0f, 0.0f, 0.0f); //Look at the center of the scene
        camera->aspectRatio = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
        camera->fov = 60.0f; //Vertical field of view, in degrees

        // Monkey model
        auto *model = new sm::Model("assets/models/Suzanne.fbx");
        sm::Transform monkeyTransform;

        //Framebuffer
        sm::Framebuffer framebuffer = sm::createFramebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB);
        unsigned int dummyVAO;
        glCreateVertexArrays(1, &dummyVAO);

        // Timing variables
        uint64_t lastFrameTicks = SDL_GetTicks64();
        float deltaTime = 0.f;

        // The game loop!
        SDL_Event event;
        bool running = true;
        while (running) {
            // EVENTS //
            while (SDL_PollEvent(&event)) {
                // Send to IMGUI
                ImGui_ImplSDL2_ProcessEvent(&event);

                // Process our own events
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                }
            }

            // UPDATING //
            // Update IMGUI
            ImGui::NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();

            // Shader Maker
            maker.doGui();

            ImGui::Begin("Compose Window", nullptr);
            {
                if (ImGui::Button("MAKE NEW SHADER")) {
                    outputNodeId = maker.getOutputNodeId();
                    generatedCode = maker.composeCodeForNodeId(outputNodeId);

                    //If the post process box isn't checked make a lit shader
                    if (!shouldUsePostProcess) {
                        shaderGenLit.generatedShader = shaderGenLit.generateShaderLit(generatedCode);
                        shaderGenLit.hasCode = true;
                    } 
                    else if (shouldUsePostProcess) { //If it is checked make a post process shader
                        shaderGenPost.generatedShader = shaderGenPost.generateShaderPost(generatedCode);
                        shaderGenPost.hasCode = true;
                    }
                }

                ImGui::SameLine();

                ImGui::Text("Output Node ID: %d", outputNodeId.Get());
                ImGui::Checkbox("Use Postprocess", &shouldUsePostProcess);
                if (ImGui::CollapsingHeader("Generated GLSL")) {
                    if (generatedCode.empty()) {
                        ImGui::Text("No GLSL Generated...");
                    }
                    else {
                        ImGui::Text("%s", generatedCode.c_str());
                    }
                }

                if(ImGui::Button("SAVE SHADER")) {
                    sm::maker::savegraph::saveGraphToFile("temp.txt", maker);
                }

                if(ImGui::Button("LOAD SHADER")) {
                    sm::maker::savegraph::loadFileToGraph("temp.txt", maker);
                }
            }
            ImGui::End();
            //END OF UI


            // DRAWING //
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
            glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Spin da monkey
            monkeyTransform.rotation = glm::rotate(monkeyTransform.rotation, 1.f * deltaTime, glm::vec3(0, 1, 0));

            roofColor->bindToTextureUnit(0);
            roofNormal->bindToTextureUnit(1);

            //If shader generator hasn't generated code yet we use the default
            if (!shaderGenLit.hasCode)
            {
                // Update positions and transforms
                shader->use();
                shader->setMat4("u_matModel", monkeyTransform.modelMatrix());
                shader->setMat4("u_matView", camera->projectionMatrix()* camera->viewMatrix());
                shader->setVec3("u_vEyePos", camera->position);

                // Draw the monkey
                model->draw();
            }
            else {
                shaderGenLit.generatedShader->use();
                shaderGenLit.generatedShader->setMat4("u_matModel", monkeyTransform.modelMatrix());
                shaderGenLit.generatedShader->setMat4("u_matView", camera->projectionMatrix()* camera->viewMatrix());
                shaderGenLit.generatedShader->setVec3("u_vEyePos", camera->position);
                shaderGenLit.generatedShader->setFloat("u_sMaterial.nAmbient", material.ambient);
                shaderGenLit.generatedShader->setFloat("u_sMaterial.nDiffuse", material.diffuse);
                shaderGenLit.generatedShader->setFloat("u_sMaterial.nSpecular", material.specular);
                shaderGenLit.generatedShader->setFloat("u_sMaterial.nShininess", material.shininess);
                shaderGenLit.generatedShader->setInt("u_texMain", 0);
                shaderGenLit.generatedShader->setInt("u_texNormal", 1);

                // Draw the monkey
                model->draw();
            }

            //Postprocessing, second render pass to screen
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderGenPost.generatedShader->use();

            glBindTextureUnit(0, framebuffer.colorBuffer[0]);
            glBindVertexArray(dummyVAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Draw IMGUI
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Flip the window
            SDL_GL_SwapWindow(window);

            // Update delta time calculation
            uint64_t current_time = SDL_GetTicks64();
            deltaTime = (current_time - lastFrameTicks) / (1000.f);
            lastFrameTicks = current_time;
        }

        delete shader, model, camera;
    }

    // Cleanup!
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //system("pause");
    return 0;
}
