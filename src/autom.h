#pragma once
#include <stdbool.h>

typedef struct {
    size_t cols_len;
    size_t rows_len;
    size_t img_scale;
    unsigned char rule;
    bool *cols;
    char *file_name;
} Autom;

Autom autom_create(size_t cols_len, size_t rows_len, size_t img_scale, unsigned char rule);

void autom_realloc(Autom *autom);

void autom_discard(Autom autom);

void autom_make_centered(Autom autom);

void autom_randomize(Autom autom);

void autom_advance(Autom *autom);

void autom_print_step(Autom autom, char block, char space);

void autom_print_all(Autom *autom, char block, char space);

void autom_print_nonstop(Autom *autom, char block, char space);

void autom_extract_ppm(Autom *autom);
