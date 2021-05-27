#include <iostream>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main() {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    int a = 45;
    glm::vec3 color(1,2,3);
    std::cout << "Hello world\n";

    SDL_Quit();
    return 0;
}