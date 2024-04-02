#define GL_GLEXT_PROTOTYPES
#include "SDL.h"
#include "SDL_opengl.h"

#include "shader.h"

int main(int argc, char *argv[]) {
    SDL_Log("%s", "main()");
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_Window *window = SDL_CreateWindow("ShaderMaker AG XO NS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,
                                          900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_DestroyWindow(window);

    SDL_Quit();

    system("pause");
    return 0;
}
