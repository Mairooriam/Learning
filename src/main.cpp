#include <iostream>
#include "RenderWindow.hpp"

int main(int argc, char* argv[]) {
    RenderWindow renderwindow;
    if (!renderwindow.initWindow(600, 400)) {
        printf("Failed to initialize!\n");
        return -1;
    }

    if (!renderwindow.loadMedia()) {
        printf("Failed to load media!\n");
        return -1;
    }

    // Apply the image
    SDL_BlitSurface(renderwindow.getHelloWorld(), NULL, renderwindow.getScreenSurface(), NULL);
    // Update the surface
    SDL_UpdateWindowSurface(renderwindow.getWindow());

    // Event loop to keep window open
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    renderwindow.updateScreenSurface();
                    SDL_BlitSurface(renderwindow.getHelloWorld(), NULL, renderwindow.getScreenSurface(), NULL);
                    SDL_UpdateWindowSurface(renderwindow.getWindow());
                }
            }
        }
        // Additional rendering or processing can be done here
    }

    return 0;
}