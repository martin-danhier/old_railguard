
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <bitset>
#include <cassert>
#include "core/EntityManager.h"

int main() {
    // Init SDL2
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    // Init the Entity system
    EntityManager entityManager(10000);


    // Quit SDL2
    SDL_Quit();
    return 0;
}