
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <bitset>
#include <cassert>
#include <unordered_map>
#include "core/EntityManager.h"
#include "rendering/Renderer.h"

using namespace railguard::core;
using namespace railguard::rendering;

int main() {
    // Init SDL2
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    
    

    // Quit SDL2
    SDL_Quit();
    return 0;
}