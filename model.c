#include "model.h"
#include <ctype.h>
#include <malloc.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

AsciiModel* create_empty_model(int rows, int cols) {

    AsciiModel* model = (AsciiModel*) malloc(sizeof(AsciiModel));

    if (!model) {
        return NULL;
    }

    char* model_data = malloc(sizeof(char) * rows * cols);

    if (!model_data) {
        free(model);
        return NULL;
    }

    model->model_data = model_data;
    model->model_rows = rows;
    model->model_cols = cols;

    return model;
}


void copy_model(AsciiModel* src, AsciiModel* dst) {

    if (!src || !dst) {
        return;
    }

    dst->model_rows = src->model_rows;
    dst->model_cols = src->model_cols;
    memcpy(dst->model_data, src->model_data, src->model_cols * src->model_rows);
}

int scan_int(FILE* f) {

    int a = 0;
    char c = fgetc(f);

    while(isdigit(c)) {
        a *= 10;
        a += c - '0';
        c = fgetc(f);
    }

    return a;
}

AsciiModel* load_model(const char* model_path) {

    if (!model_path || strlen(model_path) < 1) {
        return NULL;
    }

    FILE* model_file = fopen(model_path, "r");

    if (!model_path) {
        return NULL;
    }

    int rows = scan_int(model_file);
    int cols = scan_int(model_file);
    AsciiModel* model = create_empty_model(rows, cols); 

    if (!model) {
        return NULL;
    }

    char c;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            
            c = fgetc(model_file);

            if (c == EOF) {
                free(model);
                return NULL;
            }
            
            model->model_data[i*cols + j] = c;
        }

        c = fgetc(model_file);

        if (c != '\n') {
            free(model);
            return NULL;
        }
    }

    return model;
}

void draw_model(const AsciiModel* model, Pixel* output, ColorMode m, 
                const int offset_rows, const int offset_cols, 
                const int output_rows, const int output_cols) {

    if (!model || !output || offset_rows < 1 || offset_rows > output_rows ||
        offset_cols < 1 || offset_cols > output_cols || output_rows < 1 || output_cols < 1) {

        return;
    }

    Pixel p = { ' ', m };
    for (int i = 0; i < model->model_rows; ++i) {
        for (int j = 0; j < model->model_cols; ++j) {
            int index = (offset_rows + i)*output_cols + offset_cols + j; 
            p.c = model->model_data[i*model->model_cols + j];
            output[index] = p; 
        }
    }     
}

void undraw_model(const AsciiModel* model, Pixel* output, ColorMode m, 
                const int offset_rows, const int offset_cols, 
                const int output_rows, const int output_cols) {

    if (!model || !output || offset_rows < 1 || offset_rows > output_rows ||
        offset_cols < 1 || offset_cols > output_cols || output_rows < 1 || output_cols < 1) {

        return;
    }

    Pixel p = { ' ', m };
    for (int i = 0; i < model->model_rows; ++i) {
        for (int j = 0; j < model->model_cols; ++j) {
            int index = (offset_rows + i)*output_cols + offset_cols + j; 
            output[index] = p; 
        }
    }     
}
