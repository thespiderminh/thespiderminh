#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<vector>
#include<string>

using namespace std;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

static SDL_Renderer* renderer;
static SDL_Event event;

void logSDLError(ostream& os, const string& msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << endl;
    if(fatal){
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) logSDLError(cout, "SDL_Init", true);
    gWindow = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    gScreenSurface = SDL_GetWindowSurface( gWindow );
    if(window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer  == nullptr) logSDLError(cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, string path){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}


int main(int argc, char* argv[]){
    srand(time(0));

    bool quit = false;
    int wrongGuess = 0, rightGuess = 0;

    initSDL(gWindow, renderer);

    SDL_Surface* hangman = IMG_Load("Hangman.png");
    SDL_Surface* screen = IMG_Load("Background.png");
    SDL_Surface* h = IMG_Load("h.png");
    SDL_Surface* a = IMG_Load("a.png");
    SDL_Surface* n = IMG_Load("n.png");
    SDL_Surface* g = IMG_Load("g.png");
    SDL_Surface* m = IMG_Load("m.png");
    SDL_Surface* start = IMG_Load("PlayButton1.png");
    SDL_Surface* title = IMG_Load("Title.png");
    SDL_Surface* rule = IMG_Load("Rule.png");
    SDL_Surface* word = IMG_Load("Word.png");

    SDL_Rect HM = {100, 230, 500, 500};
    SDL_Rect W = {50, 850, 800, 50};
    SDL_Rect RL = {0, 0, 854, 129};
    SDL_Rect TT = {1000, 100, 477, 267};
    SDL_Rect ST = {330, 550, 200, 200};
    SDL_Rect A = {180, 770, 200, 200};
    SDL_Rect H = {70, 780, 200, 200};
    SDL_Rect N = {280, 770, 200, 200};
    SDL_Rect G = {380, 770, 200, 200};
    SDL_Rect M = {480, 780, 200, 200};

    SDL_BlitSurface( screen, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface( gWindow );

    SDL_Texture* _title = loadTexture(renderer, "Title.png");

    while(TT.x > 240){
        TT.x -= 2;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, _title, NULL, &TT);
        SDL_RenderPresent(renderer);
    }
    SDL_BlitSurface( title, NULL, gScreenSurface, &TT);
    SDL_BlitSurface( start, NULL, gScreenSurface, &ST);
    SDL_UpdateWindowSurface( gWindow );

    bool checkCollision = 0;
    while(!quit){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
                quit = true;
                quitSDL(gWindow, renderer);
                return 0;
            }else if(event.type == SDL_MOUSEMOTION){
                if((event.motion.x > ST.x && event.motion.x < (ST.x+ST.w)) && (event.motion.y > ST.y && event.motion.y < (ST.y+ST.h))) checkCollision = 1;
                else checkCollision = 0;
                if(checkCollision==0) start = IMG_Load("PlayButton1.png");
                else start = IMG_Load("PlayButton2.png");
                SDL_BlitSurface( start, NULL, gScreenSurface, &ST);
                SDL_UpdateWindowSurface( gWindow );
            }
            if(checkCollision == 1 && event.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }
    quit = false;
    SDL_BlitSurface( screen, NULL, gScreenSurface, NULL);
    SDL_BlitSurface( rule, NULL, gScreenSurface, &RL);
    SDL_BlitSurface( hangman, NULL, gScreenSurface, &HM);
    SDL_BlitSurface( word, NULL, gScreenSurface, &W);

    SDL_UpdateWindowSurface( gWindow );

    while(!quit){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
                quit = true;
            }else if(event.type == SDL_KEYDOWN && rightGuess <= 3 && event.key.repeat == 0){
                switch(event.key.keysym.sym){
                    case SDLK_a:
                        SDL_BlitSurface( a, NULL, gScreenSurface, &A);rightGuess++;
                        break;
                    case SDLK_h:
                        SDL_BlitSurface( h, NULL, gScreenSurface, &H);rightGuess++;
                        break;
                    case SDLK_n:
                        SDL_BlitSurface( n, NULL, gScreenSurface, &N);rightGuess++;
                        break;
                    case SDLK_g:
                        SDL_BlitSurface( g, NULL, gScreenSurface, &G);rightGuess++;
                        break;
                    case SDLK_m:
                        SDL_BlitSurface( m, NULL, gScreenSurface, &M);rightGuess++;
                        break;
                    default:
                        wrongGuess++;
                        break;
                }
                SDL_UpdateWindowSurface( gWindow );
            }else if(event.type == SDL_KEYDOWN && rightGuess > 3 && event.key.repeat == 0){
                wrongGuess++;
            }
            if(wrongGuess == 1) hangman = IMG_Load("Hangman1.png");
            else if(wrongGuess == 2) hangman = IMG_Load("Hangman2.png");
            else if(wrongGuess == 3) hangman = IMG_Load("Hangman3.png");
            else if(wrongGuess == 4) hangman = IMG_Load("Hangman4.png");
            else if(wrongGuess == 5) hangman = IMG_Load("Hangman5.png");
            else if(wrongGuess == 6) hangman = IMG_Load("Hangman6.png");
            SDL_BlitSurface( hangman, NULL, gScreenSurface, &HM);
            SDL_UpdateWindowSurface( gWindow );
        }
    }
    SDL_Delay(100);
//        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//        SDL_RenderClear(renderer);
//        SDL_RenderPresent(renderer);
    quitSDL(gWindow, renderer);
    return 0;
}

