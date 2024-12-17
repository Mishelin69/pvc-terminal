#pragma once

typedef enum {
    COLOR_MODE_DEFAULT,
    COLOR_MODE_RED,
    COLOR_MODE_GREEN,
    COLOR_MODE_BLUE,
    COLOR_MODE_YELLOW,
    COLOR_MODE_CYAN,
    COLOR_MODE_MAGENTA,
    COLOR_MODE_ENEMY
} ColorMode;

typedef struct Pixel {
    char c;
    ColorMode m;
} Pixel;
