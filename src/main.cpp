#include "SDL.h"

#include "external/glad.h"

#include "shader.h"

int main(int argc, char *argv[])
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
    SDL_Window *window = SDL_CreateWindow("ShaderMaker AG XO NS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,
                                          900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize window: %s", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to create OpenGL context: %s", SDL_GetError());
        return 1;
    }

    // The stuff lol
    {
        sm::Shader *shader = sm::loadShader("assets/albedo.vert", "assets/albedo.frag");

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
            glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Update the window
            SDL_GL_SwapWindow(window);
        }

        delete shader;
    }

    // Cleanup!
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    system("pause");
    return 0;
}
