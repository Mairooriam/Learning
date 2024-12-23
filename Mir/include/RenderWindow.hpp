#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP
#pragma once
#include <SDL.h>
#include <cstdio>
class RenderWindow{
    public:
        RenderWindow();
        bool initWindow(int p_height, int p_width);
        bool loadMedia();

        SDL_Window* getWindow() const { return m_window; }
        SDL_Surface* getScreenSurface() const { return m_screenSurface; }
        SDL_Surface* getHelloWorld() const { return m_helloWorld; }
        void updateScreenSurface();
        ~RenderWindow();
    private:
    //The window we'll be rendering to
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_screenSurface = nullptr;
    SDL_Surface* m_helloWorld = nullptr;
};

#endif