#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL2/SDL.h>

#include "chess.cpp"
#include "colors.h"
#include "draw.h"
#include "frame.h"

const int WINDOW_HEIGHT = 1600;
const int WINDOW_WIDTH = 1600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

Uint32* buffer = NULL;

void getBuffer() {
    int* windowWidthTemp = new int(WINDOW_WIDTH);
    SDL_LockTexture(texture, NULL, (void**) &buffer, windowWidthTemp);
    delete windowWidthTemp;
}

void flip() {
    int* windowWidthTemp = new int(WINDOW_WIDTH);
    SDL_UnlockTexture(texture);
    SDL_UpdateTexture(texture, NULL, buffer, (*windowWidthTemp) * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    buffer = NULL;
    delete windowWidthTemp;
}

void handleInput(SDL_Event* event, FrameState* frameState) {

    // Mouse buttons down
    if ((*event).type == SDL_MOUSEBUTTONDOWN) {

        if ((*event).button.button == SDL_BUTTON_LEFT) {
            frameState->mouse->leftButtonDown();
        }

        if ((*event).button.button == SDL_BUTTON_RIGHT) {
            frameState->mouse->rightButtonDown();
        }
    }

    // Mouse buttons up
    if ((*event).type == SDL_MOUSEBUTTONUP) {

        if ((*event).button.button == SDL_BUTTON_LEFT) {
            frameState->mouse->leftButtonUp();
        }

        if ((*event).button.button == SDL_BUTTON_RIGHT) {
            frameState->mouse->rightButtonUp();
        }
    }

    return;
}

int main(int argc, char* argv[]) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL_Init Failed! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "CreateWindow Failed! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "CreateRenderer Failed! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Create texture
    int* windowWidthTemp = new int(WINDOW_WIDTH);
    int* windowHeightTemp = new int(WINDOW_HEIGHT);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, *windowWidthTemp, *windowHeightTemp);
    if (texture == NULL) {
        fprintf(stderr, "CreateTexture Failed! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    delete windowHeightTemp, windowWidthTemp;

    initBoard();
    atexit(SDL_Quit);

    FrameState* frameState = new FrameState();
    Drawer* drawer;
    SDL_Event event;

    //system("pause");

    // Main event loop
    bool leave = false;
    while (!leave) {

        // Mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        frameState->mouse->setPos( (int) (mouseX * 2.5), (int) (mouseY * 10 / 3) ); // SDL is whack the mouse coords dont line up with pixels
        // std::cout<< (int) (mouseX * 2.5) << ", " << (int) (mouseY * 10 / 3) << "\n";
        
        // Handle SDL events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) { leave = true; std::cout<<"closed"; }
            else handleInput(&event, frameState);
        }

        getBuffer();

        drawer = new Drawer(buffer, (int) WINDOW_WIDTH, (int) WINDOW_HEIGHT);
        doFrame(drawer, frameState);
        delete drawer;

        flip();

        // Make current frameState become last frame
        frameState->nextFrame();
        frameState->incrementCounter();

        //SDL_Delay(1000);
    }

    delete frameState;

    // Destroy the window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    // system("pause");

    return 0;
}