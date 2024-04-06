#include "external/glad.h"
#include "SDL.h"

#include "sm/shader.h"
#include "sm/model.h"
#include "sm/camera.h"
#include "sm/transform.h"
#include "sm/texture.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

struct Material {
	float ambient = 1.0f;
	float diffuse = 0.5f;
	float specular = 0.5f;
	float shininess = 128.f;
} material;

int main(int argc, char* argv[])
{
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
	SDL_Window* window = SDL_CreateWindow("ShaderMaker AG XO NS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
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

	// Enable backface cullin'
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); //Depth testing

	// The stuff lol
	{
		// Load and bind the textures
		auto* roofColor = new sm::Texture("assets/textures/roof_color.png");
		auto* roofNormal = new sm::Texture("assets/textures/roof_normal.png");

		roofColor->bindToTextureUnit(0);
		roofNormal->bindToTextureUnit(1);

		// Load the lighting shader
		sm::Shader* shader = sm::loadShader("assets/albedo.vert", "assets/albedo.frag");
		shader->use();
		shader->setFloat("u_sMaterial.nAmbient", material.ambient);
		shader->setFloat("u_sMaterial.nDiffuse", material.diffuse);
		shader->setFloat("u_sMaterial.nSpecular", material.specular);
		shader->setFloat("u_sMaterial.nShininess", material.shininess);
		shader->setInt("u_texMain", 0);
		shader->setInt("u_texNormal", 1);

		// Camera
		auto* camera = new sm::Camera();
		camera->position = glm::vec3(0.0f, 0.0f, 5.0f);
		camera->target = glm::vec3(0.0f, 0.0f, 0.0f); //Look at the center of the scene
		camera->aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		camera->fov = 60.0f; //Vertical field of view, in degrees

		// Monkey model
		auto* model = new sm::Model("assets/models/Suzanne.fbx");
		sm::Transform monkeyTransform;

		// Timing variables
		uint64_t lastFrameTicks = SDL_GetTicks64();
		float deltaTime = 0.f;

		SDL_Event event;
		bool running = true;
		while (running) {
			// Handle events
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				}
			}

			// Update + draw
			monkeyTransform.rotation = glm::rotate(monkeyTransform.rotation, 1.f * deltaTime, glm::vec3(0, 1, 0));

			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Update positions and transforms
			shader->use();
			shader->setMat4("u_matModel", monkeyTransform.modelMatrix());
			shader->setMat4("u_matView", camera->projectionMatrix() * camera->viewMatrix());
			shader->setVec3("u_vEyePos", camera->position);

			// Draw the monkey
			model->draw();

			// Update the window
			SDL_GL_SwapWindow(window);

			// Update delta time calculation
			deltaTime = (SDL_GetTicks64() - lastFrameTicks) / (1000.f);
			lastFrameTicks = SDL_GetTicks64();
		}


		delete shader, model;
	}

	// Cleanup!
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//system("pause");
	return 0;
}
