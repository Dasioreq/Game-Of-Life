#include <SDL2/SDL.h>
#include <chrono>
#include <cmath>

unsigned int xCells = 16*4, yCells = 9*4,  sizeMult = 25, margin = sizeMult / 15;
int timeSteps = 5;
bool quit = false, isPlaying = false, wraparound = true;

using namespace std;

void drawCell(unsigned int i,unsigned int j, SDL_Renderer* renderer, SDL_Window* window, unsigned int color)
{
    SDL_Renderer *rectRenderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Rect cellRect;
    cellRect.x = (sizeMult * i) + margin;
    cellRect.y = (sizeMult * j) + margin;
    cellRect.w = sizeMult - margin;
    cellRect.h = sizeMult - margin;

    SDL_SetRenderDrawColor(renderer, color, color, color, 255);
    SDL_RenderFillRect(renderer, &cellRect);

    SDL_RenderPresent(renderer);
}

void sleep(double millis, SDL_Renderer* renderer, SDL_Window* window, unsigned char** cells)
{
    auto startTime = chrono::system_clock::now();
    chrono::duration<double, std::milli> elapsed = startTime - startTime;
     
    while(elapsed.count() < millis)
    {
        SDL_Event event;
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if(event.type==SDL_KEYDOWN)
            {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    quit = true;
                    break;
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                {
                    if(isPlaying)
                        isPlaying = false;
                    else
                        isPlaying = true;

                    break;
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
                {
                    isPlaying = false;
                    for(unsigned int i = 0; i < xCells; i++)
                    {
                        for(unsigned int j = 0; j < yCells; j++)
                        {
                            cells[i][j] = 0x00;
                            drawCell(i, j, renderer, window, 0);   
                        }
                    }
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
                    timeSteps++;
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && timeSteps > 1)
                    timeSteps--;
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w)
                {
                    isPlaying = false;
                    for(unsigned int i = 0; i < xCells; i++)
                    {
                        for(unsigned int j = 0; j < yCells; j++)
                        {
                            cells[i][j] = 0x00;
                            drawCell(i, j, renderer, window, 0);   
                        }
                    }
                    if(wraparound)
                        wraparound = false;
                    else
                        wraparound = true;
                }
            }
        }
        elapsed = chrono::system_clock::now() - startTime;
    }
}

void updateCells(SDL_Renderer* renderer, SDL_Window* window, unsigned char** cells)
{
    auto startTime = chrono::system_clock::now();

    unsigned char** newCells = new unsigned char*[xCells];

    for(int i = 0; i < xCells; i++)
    {
        newCells[i] = new unsigned char[yCells];

        for(int j = 0; j < yCells; j++)
            newCells[i][j] = cells[i][j];
    }

    if(wraparound)
    {
        for(int i = 0; i < xCells; i++)
        {
            for(int j = 0; j < yCells; j++)
            {
                if(cells[i][j] != 0)
                {
                    int xRight = (i + 1) % xCells;
                    int xLeft = (xCells + (i - 1)) % xCells;
                    int yBelow = (j + 1) % yCells;
                    int yAbove = (yCells + (j - 1)) % yCells;

                    if((cells[i][j]) & 0x01)
                    {
                        if((cells[i][j] >> 1) > 3 || (cells[i][j] >> 1) < 2)
                        {
                            newCells[i][j] -= 0x01;
                            newCells[i][yAbove] -= 0x02;
                            newCells[i][yBelow] -= 0x02;
                            newCells[xRight][j] -= 0x02;
                            newCells[xRight][yAbove] -= 0x02;
                            newCells[xRight][yBelow] -= 0x02;
                            newCells[xLeft][j] -= 0x02;
                            newCells[xLeft][yAbove] -= 0x02;
                            newCells[xLeft][yBelow] -= 0x02;
                            drawCell(i, j, renderer, window, 0);
                        }
                    }
                    else
                    {
                        if(cells[i][j] >> 1 == 0x03)
                        {
                            newCells[i][j] += 0x01;
                            newCells[i][yAbove] += 0x02;
                            newCells[i][yBelow] += 0x02;
                            newCells[xRight][j] += 0x02;
                            newCells[xRight][yAbove] += 0x02;
                            newCells[xRight][yBelow] += 0x02;
                            newCells[xLeft][j] += 0x02;
                            newCells[xLeft][yAbove] += 0x02;
                            newCells[xLeft][yBelow] += 0x02;
                            drawCell(i, j, renderer, window, 255);
                        }
                    }
                }  
                else
                {        
                    continue; 
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < xCells; i++)
        {
            for(int j = 0; j < yCells; j++)
            {
                if(cells[i][j] != 0)
                {
                    int xRight = i + 1;
                    int xLeft = i - 1;
                    int yAbove = j - 1;
                    int yBelow = j + 1;

                    if((cells[i][j]) & 0x01)
                    {
                        if((cells[i][j] >> 1) > 3 || (cells[i][j] >> 1) < 2)
                        {
                            newCells[i][j] -= 0x01;
                            if(yAbove >= 0)
                            {
                                newCells[i][yAbove] -= 0x02;
                                if(xRight < xCells)
                                    newCells[xRight][yAbove] -= 0x02;
                                if(xLeft >= 0)
                                    newCells[xLeft][yAbove] -= 0x02;
                            }
                            if(yBelow < yCells)
                            {
                                newCells[i][yBelow] -= 0x02;
                                if(xRight < xCells)
                                    newCells[xRight][yBelow] -= 0x02;
                                if(xLeft >= 0)
                                    newCells[xLeft][yBelow] -= 0x02;
                            }
                            if(xRight < xCells)
                                newCells[xRight][j] -= 0x02;
                            if(xLeft >= 0)
                                newCells[xLeft][j] -= 0x02;
                            drawCell(i, j, renderer, window, 0);
                        }
                    }
                    else
                    {
                        if(cells[i][j] >> 1 == 0x03)
                        {
                            newCells[i][j] += 0x01;
                            if(yAbove >= 0)
                            {
                                newCells[i][yAbove] += 0x02;
                                if(xRight < xCells)
                                    newCells[xRight][yAbove] += 0x02;
                                if(xLeft >= 0)
                                    newCells[xLeft][yAbove] += 0x02;
                            }
                            if(yBelow < yCells)
                            {
                                newCells[i][yBelow] += 0x02;
                                if(xRight < xCells)
                                    newCells[xRight][yBelow] += 0x02;
                                if(xLeft >= 0)
                                    newCells[xLeft][yBelow] += 0x02;
                            }
                            if(xRight < xCells)
                                newCells[xRight][j] += 0x02;
                            if(xLeft >= 0)
                                newCells[xLeft][j] += 0x02;
                            drawCell(i, j, renderer, window, 255);
                        }
                    }
                }  
                else
                {        
                    continue; 
                }
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

    delete[] newCells;

    chrono::duration<double, std::milli> elapsed = chrono::system_clock::now() - startTime;

    sleep(1000 / timeSteps - elapsed.count(), renderer, window, cells);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xCells * sizeMult, yCells * sizeMult, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event event;

    int xMouse;
    int yMouse;

    unsigned char** cells = new unsigned char*[xCells];

    for(int i = 0; i < xCells; i++)
    {
        cells[i] = new unsigned char[yCells];

        for(int j = 0; j < yCells; j++)
        {
            cells[i][j] = 0x00;
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
                    updateCells(renderer, window, cells);
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
                            cells[i][j] = 0x00;
                            drawCell(i, j, renderer, window, 0);   
                        }
                    }
                }
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
                    timeSteps++;
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && timeSteps > 1)
                    timeSteps--;
                else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_w)
                {
                    isPlaying = false;
                    for(unsigned int i = 0; i < xCells; i++)
                    {
                        for(unsigned int j = 0; j < yCells; j++)
                        {
                            cells[i][j] = 0x00;
                            drawCell(i, j, renderer, window, 0);   
                        }
                    }
                    if(wraparound)
                        wraparound = false;
                    else
                        wraparound = true;
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
                if(wraparound)
                {
                    if(event.button.button == SDL_BUTTON_LEFT && !isPlaying)
                    {
                        int xRight = (xMouse + 1) % xCells;
                        int xLeft = (xCells + (xMouse - 1)) % xCells;
                        int yAbove = (yMouse + 1) % yCells;
                        int yBelow = (yCells + (yMouse - 1)) % yCells;

                        if((cells[xMouse][yMouse]) & 0x01)
                        {
                            cells[xMouse][yMouse] -= 0x01;
                            cells[xRight][yMouse] -= 0x02;
                            cells[xRight][yAbove] -= 0x02;
                            cells[xRight][yBelow] -= 0x02;
                            cells[xMouse][yAbove] -= 0x02;
                            cells[xMouse][yBelow] -= 0x02;
                            cells[xLeft][yMouse] -= 0x02;
                            cells[xLeft][yAbove] -= 0x02;
                            cells[xLeft][yBelow] -= 0x02;
                            drawCell(xMouse, yMouse, renderer, window, 0);
                        }
                        else
                        {
                            cells[xMouse][yMouse] += 0x01;
                            cells[xRight][yMouse] += 0x02;
                            cells[xRight][yAbove] += 0x02;
                            cells[xRight][yBelow] += 0x02;
                            cells[xMouse][yAbove] += 0x02;
                            cells[xMouse][yBelow] += 0x02;
                            cells[xLeft][yMouse] += 0x02;
                            cells[xLeft][yAbove] += 0x02;
                            cells[xLeft][yBelow] += 0x02;
                            drawCell(xMouse, yMouse, renderer, window, 255);
                        }
                    }
                }
                else
                {
                    if(event.button.button == SDL_BUTTON_LEFT && !isPlaying)
                    {
                        int xRight = xMouse + 1;
                        int xLeft = xMouse - 1;
                        int yAbove = yMouse - 1;
                        int yBelow = yMouse + 1;

                        if(cells[xMouse][yMouse] & 0x01)
                        {
                            cells[xMouse][yMouse] -= 0x01;
                            if(yAbove >= 0)
                            {
                                cells[xMouse][yAbove] -= 0x02;
                                if(xRight < xCells)
                                    cells[xRight][yAbove] -= 0x02;
                                if(xLeft >= 0)
                                    cells[xLeft][yAbove] -= 0x02;
                            }
                            if(yBelow < yCells)
                            {
                                cells[xMouse][yBelow] -= 0x02;
                                if(xRight < xCells)
                                    cells[xRight][yBelow] -= 0x02;
                                if(xLeft >= 0)
                                    cells[xLeft][yBelow] -= 0x02;
                            }
                            if(xRight < xCells)
                                cells[xRight][yMouse] -= 0x02;
                            if(xLeft >= 0)
                                cells[xLeft][yMouse] -= 0x02;
                            drawCell(xMouse, yMouse, renderer, window, 0);
                        }
                        else
                        {
                            cells[xMouse][yMouse] += 0x01;
                            if(yAbove >= 0)
                            {
                                cells[xMouse][yAbove] += 0x02;
                                if(xRight < xCells)
                                    cells[xRight][yAbove] += 0x02;
                                if(xLeft >= 0)
                                    cells[xLeft][yAbove] += 0x02;
                            }
                            if(yBelow < yCells)
                            {
                                cells[xMouse][yBelow] += 0x02;
                                if(xRight < xCells)
                                    cells[xRight][yBelow] += 0x02;
                                if(xLeft >= 0)
                                    cells[xLeft][yBelow] += 0x02;
                            }
                            if(xRight < xCells)
                                cells[xRight][yMouse] += 0x02;
                            if(xLeft >= 0)
                                cells[xLeft][yMouse] += 0x02;
                            drawCell(xMouse, yMouse, renderer, window, 255);
                        }
                    }
                }
            }
        }

        if(isPlaying)
        {
            updateCells(renderer, window, cells);
        }
    }

    delete[] cells;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}