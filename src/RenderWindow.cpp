#include "RenderWindow.hpp"
#include <string>

RenderWindow::RenderWindow() : m_window(nullptr), m_screenSurface(nullptr) {}

RenderWindow::~RenderWindow()
{
    //Deallocate surface
	SDL_FreeSurface( m_helloWorld );
	m_helloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( m_window );
	m_window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


void RenderWindow::updateScreenSurface()
{
    //Get window surface
    m_screenSurface = SDL_GetWindowSurface( m_window );
}


bool RenderWindow::initWindow(int p_height, int p_width)
{
    bool success = true;
    //Create window
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        p_height, p_width, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
        if( m_window  == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            m_screenSurface = SDL_GetWindowSurface( m_window );
        }
    }
	return success;
}

bool RenderWindow::loadMedia()
{
    	//Loading success flag
	bool success = true;
    const char* path = "resources/textures/hello_world.bmp";
	//Load splash image
	m_helloWorld = SDL_LoadBMP( path );
	if( m_helloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
		success = false;
	}

	return success;
}
