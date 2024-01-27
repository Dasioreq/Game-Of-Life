#include <SDL2/SDL.h>
#include <iostream>
#include <windows.h>
#include <cmath>

unsigned const int xCells = 16*3, yCells = 9*3,  sizeMult = 30;

using namespace std;

unsigned char cells[xCells][yCells];
unsigned char newCells[xCells][yCells];

void drawCell(unsigned int i,unsigned int j, SDL_Renderer* renderer, SDL_Window* window, unsigned int color)
{
    SDL_Renderer *rectRenderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Rect cellRect;
    cellRect.x = (sizeMult * i) + (sizeMult / 15);
    cellRect.y = (sizeMult * j) + (sizeMult / 15);
    cellRect.w = sizeMult - sizeMult / 15;
    cellRect.h = sizeMult - sizeMult / 15;

    SDL_SetRenderDrawColor(renderer, color, color, color, 255);
    SDL_RenderFillRect(renderer, &cellRect);

    SDL_RenderPresent(renderer);
}

void updateCells(SDL_Renderer* renderer, SDL_Window* window)
{
    for(int i = 0; i < xCells; i++)
    {
        for(int j = 0; j < yCells; j++)
            newCells[i][j] = cells[i][j];
    }

    for(int i = 0; i < xCells; i++)
    {
        for(int j = 0; j < yCells; j++)
        {
            if(cells[i][j] != 0)
            {
                if((cells[i][j]) & 0x01)
                {
                    if((cells[i][j] >> 1) > 3 || (cells[i][j] >> 1) < 2)
                    {
                        newCells[i][j] -= 0x01;
                        newCells[i][(j + 1) % yCells] -= 0x02;
                        newCells[i][(yCells + (j - 1)) % yCells] -= 0x02;
                        newCells[(i + 1) % xCells][j] -= 0x02;
                        newCells[(i + 1) % xCells][(j + 1) % yCells] -= 0x02;
                        newCells[(i + 1) % xCells][(yCells + (j - 1)) % yCells] -= 0x02;
                        newCells[(xCells + (i - 1)) % xCells][j] -= 0x02;
                        newCells[(xCells + (i - 1)) % xCells][(j + 1) % yCells] -= 0x02;
                        newCells[(xCells + (i - 1)) % xCells][(yCells + (j - 1)) % yCells] -= 0x02;
                        drawCell(i, j, renderer, window, 0);
                    }
                }
                else
                {
                    if(cells[i][j] >> 1 == 0x03)
                    {
                        newCells[i][j] += 0x01;
                        newCells[i][(j + 1) % yCells] += 0x02;
                        newCells[i][(yCells + (j - 1)) % yCells] += 0x02;
                        newCells[(i + 1) % xCells][j] += 0x02;
                        newCells[(i + 1) % xCells][(j + 1) % yCells] += 0x02;
                        newCells[(i + 1) % xCells][(yCells + (j - 1)) % yCells] += 0x02;
                        newCells[(xCells + (i - 1)) % xCells][j] += 0x02;
                        newCells[(xCells + (i - 1)) % xCells][(j + 1) % yCells] += 0x02;
                        newCells[(xCells + (i - 1)) % xCells][(yCells + (j - 1)) % yCells] += 0x02;
                        drawCell(i, j, renderer, window, 255);
                    }
                }
            }  
            else
            {
                drawCell(i, j, renderer, window, 0);
                continue; 
            }
        }
    }


    for(unsigned int i = 0; i < xCells; i++)
    {
        for(unsigned int j = 0; j < yCells; j++)
        {
            cells[i][j] = (unsigned char)newCells[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    for(unsigned int i = 0; i < xCells; i++)
    {
        for(unsigned int j = 0; j < yCells; j++)
        {
            cells[i][j] = (unsigned char)0x00;
        }
    }

	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xCells * sizeMult, yCells * sizeMult, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event event;

    bool quit = false, isPlaying = false;
    int xMouse;
    int yMouse;

    for(int i = 0; i < xCells; i++)
    {
        for(unsigned int j = 0; j < yCells; j++)
        {
            drawCell(i, j, renderer, window, 0);
        }
    }

    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if(event.type==SDL_KEYDOWN)
            {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    quit = true;
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT && !isPlaying) 
                {
                    updateCells(renderer, window);
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                {
                    if(isPlaying)
                        isPlaying = false;
                    else
                        isPlaying = true;
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
                {
                    isPlaying = false;
                    for(unsigned int i = 0; i < xCells; i++)
                    {
                        for(unsigned int j = 0; j < yCells; j++)
                        {
                            cells[i][j] = (unsigned char)0x00;
                        }
                    }
                    updateCells(renderer, window);
                }
            }
            else if(event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&xMouse, &yMouse);

                xMouse = floor(xMouse / sizeMult);
                yMouse = floor(yMouse / sizeMult);
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                if(event.button.button == SDL_BUTTON_LEFT && !isPlaying)
                {
                    if((cells[xMouse][yMouse]) & 0x01)
                    {
                        cells[xMouse][(yMouse)] -= 0x01;
                        cells[(xMouse+1) % xCells][(yMouse % yCells)] -= (unsigned char)0x02;
                        cells[(xMouse+1) % xCells][((yMouse+1) % yCells)] -= (unsigned char)0x02;
                        cells[(xMouse+1) % xCells][((yCells + (yMouse-1)) % yCells)] -= (unsigned char)0x02;
                        cells[xMouse % xCells][((yMouse+1) % yCells)] -= (unsigned char)0x02;
                        cells[xMouse % xCells][((yCells + (yMouse-1)) % yCells)] -= (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][(yMouse % yCells)] -= (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][((yMouse+1) % yCells)] -= (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][((yCells + (yMouse-1)) % yCells)] -= (unsigned char)0x02;
                        drawCell(xMouse, yMouse, renderer, window, 0);
                    }
                    else
                    {
                        cells[xMouse][(yMouse)] += (unsigned char)0x01;
                        cells[(xMouse+1) % xCells][(yMouse % yCells)] += (unsigned char)0x02;
                        cells[(xMouse+1) % xCells][((yMouse+1) % yCells)] += (unsigned char)0x02;
                        cells[(xMouse+1) % xCells][((yCells + (yMouse-1)) % yCells)] += (unsigned char)0x02;
                        cells[xMouse % xCells][((yMouse+1) % yCells)] += (unsigned char)0x02;
                        cells[xMouse % xCells][((yCells + (yMouse-1)) % yCells)] += (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][(yMouse % yCells)] += (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][((yMouse+1) % yCells)] += (unsigned char)0x02;
                        cells[(xCells + (xMouse-1)) % xCells][((yCells + (yMouse-1)) % yCells)] += (unsigned char)0x02;
                        drawCell(xMouse, yMouse, renderer, window, 255);
                    }
                }
            }
        }

        if(isPlaying)
        {
            updateCells(renderer, window);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}