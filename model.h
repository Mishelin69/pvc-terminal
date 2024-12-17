#pragma once

#include "pixel.h"

typedef struct AsciiModel {

    char* model_data;
    int model_rows;
    int model_cols;

} AsciiModel;

AsciiModel* create_empty_model(int rows, int cols);

void copy_model(AsciiModel* src, AsciiModel* dst);

AsciiModel* load_model(const char* model_path);

void draw_model(const AsciiModel* model, Pixel* output, ColorMode m, 
                const int offset_rows, const int offset_cols, 
                const int output_rows, const int output_cols);

void undraw_model(const AsciiModel* model, Pixel* output, ColorMode m, 
                const int offset_rows, const int offset_cols, 
                const int output_rows, const int output_cols);
