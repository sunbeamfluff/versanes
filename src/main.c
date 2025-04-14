#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 720
#define HEIGHT 480
#define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
#define FONT_SIZE 16

// Define a mapping from keys to NES controller buttons and labels
typedef struct {
    SDL_Keycode key;
    uint8_t value;
    const char* label;
} KeyMapping;

// Controller 1 uses: LALT (A), LCTRL (B), 5 (Select), 1 (Start), arrows
KeyMapping controller1Keys[] = {
    { SDLK_LALT,   1,   "A" },
    { SDLK_LCTRL,  2,   "B" },
    { SDLK_5,      4,   "Select" },
    { SDLK_1,      8,   "Start" },
    { SDLK_UP,    16,   "Up" },
    { SDLK_DOWN,  32,   "Down" },
    { SDLK_LEFT,  64,   "Left" },
    { SDLK_RIGHT,128,   "Right" }
};

// Controller 2 uses: S (A), A (B), 5 (Select), 1 (Start), R/F/D/G for directions
KeyMapping controller2Keys[] = {
    { SDLK_s,      1,   "A" },
    { SDLK_a,      2,   "B" },
    { SDLK_5,      4,   "Select" },
    { SDLK_1,      8,   "Start" },
    { SDLK_r,     16,   "Up" },
    { SDLK_f,     32,   "Down" },
    { SDLK_d,     64,   "Left" },
    { SDLK_g,    128,   "Right" }
};

#define NUM_KEYS 8

// Track pressed keys for each controller
bool keyState1[NUM_KEYS] = { false };
bool keyState2[NUM_KEYS] = { false };

// Function declarations
bool initSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font);
void handleEvents(SDL_Event* e, bool* running);
void setKeyState(SDL_Keycode code, bool pressed);
uint8_t getControllerState(bool* keyState, KeyMapping* keyMap);
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
void renderDetailedInfo(SDL_Renderer* renderer, TTF_Font* font, uint8_t rawValue1, uint8_t rawValue2);
void showMessageBox(const char* title, const char* message);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    if (!initSDL(&window, &renderer, &font)) {
        showMessageBox("Error", "Failed to initialize SDL or TTF");
        return 1;
    }

    SDL_Event e;
    bool running = true;

    while (running) {
        handleEvents(&e, &running);

        // Get the current 8-bit state for each controller based on pressed keys
        uint8_t value1 = getControllerState(keyState1, controller1Keys);
        uint8_t value2 = getControllerState(keyState2, controller2Keys);

        // Render a visual representation of the current button states
        renderDetailedInfo(renderer, font, value1, value2);

        SDL_Delay(1000 / 60); // Cap the frame rate at 60 FPS
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

// Initializes SDL video, font rendering, window, and renderer
bool initSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    if (TTF_Init() == -1) return false;

    *window = SDL_CreateWindow("NES Keyboard Controller Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) return false;

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) return false;

    *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!*font) return false;

    return true;
}

// Process SDL events and update key states accordingly
void handleEvents(SDL_Event* e, bool* running) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            *running = false;
        } else if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
            bool pressed = (e->type == SDL_KEYDOWN);
            setKeyState(e->key.keysym.sym, pressed);
            if (e->key.keysym.sym == SDLK_ESCAPE) {
                *running = false;
            }
        }
    }
}

// Maps pressed key to controller 1 or 2 state tracking
void setKeyState(SDL_Keycode code, bool pressed) {
    for (int i = 0; i < NUM_KEYS; ++i) {
        if (controller1Keys[i].key == code) {
            keyState1[i] = pressed;
        }
        if (controller2Keys[i].key == code) {
            keyState2[i] = pressed;
        }
    }
}

// Builds an 8-bit value from each button's pressed state
uint8_t getControllerState(bool* keyState, KeyMapping* keyMap) {
    uint8_t state = 0;
    for (int i = 0; i < NUM_KEYS; ++i) {
        if (keyState[i]) {
            state |= keyMap[i].value;
        }
    }
    return state;
}

// Render text to the screen at a given position with a color
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Draws the full state of both controllers, including which buttons are active
void renderDetailedInfo(SDL_Renderer* renderer, TTF_Font* font, uint8_t rawValue1, uint8_t rawValue2) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = { 255, 255, 255 };
    SDL_Color green = { 0, 255, 0 };
    SDL_Color red   = { 255, 0, 0 };

    // Convert controller state to binary string
    char bin1[9], bin2[9];
    for (int i = 0; i < 8; ++i) {
        bin1[i] = (rawValue1 & (1 << (7 - i))) ? '1' : '0';
        bin2[i] = (rawValue2 & (1 << (7 - i))) ? '1' : '0';
    }
    bin1[8] = '\0';
    bin2[8] = '\0';

    renderText(renderer, font, "Controller 1:", 10, 10, white);
    renderText(renderer, font, bin1, 200, 10, white);
    renderText(renderer, font, "Controller 2:", 10, 30, white);
    renderText(renderer, font, bin2, 200, 30, white);

    // Display each button's state for both controllers
    int y = 60;
    for (int i = 0; i < NUM_KEYS; ++i) {
        SDL_Color c1 = (rawValue1 & controller1Keys[i].value) ? green : red;
        SDL_Color c2 = (rawValue2 & controller2Keys[i].value) ? green : red;

        char label1[64], label2[64];
        snprintf(label1, sizeof(label1), "C1 - %s: %s", controller1Keys[i].label, (c1.r == 0 ? "Pressed" : "Released"));
        snprintf(label2, sizeof(label2), "C2 - %s: %s", controller2Keys[i].label, (c2.r == 0 ? "Pressed" : "Released"));

        renderText(renderer, font, label1, 10, y, c1);
        renderText(renderer, font, label2, WIDTH / 2, y, c2);
        y += 20;
    }

    renderText(renderer, font, "Press ESC to quit", 10, HEIGHT - 30, white);
    SDL_RenderPresent(renderer); // Update window contents
}

// Utility for showing basic error/info dialog
void showMessageBox(const char* title, const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, NULL);
}
