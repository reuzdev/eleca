#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "autom.h"

Autom autom_create(size_t cols_len, size_t rows_len, size_t img_scale, unsigned char rule) {
    bool *cols = (bool*) calloc(cols_len, sizeof(bool));
    return (Autom) {
        .cols_len = cols_len,
        .rows_len = rows_len,
        .img_scale = img_scale,
        .rule = rule,
        .cols = cols,
        .file_name = NULL,
    };
}

void autom_realloc(Autom *autom) {
    autom->cols = realloc(autom->cols, autom->cols_len);
}

void autom_discard(Autom autom) {
    free(autom.cols);
    free(autom.file_name);
}

void autom_make_centered(Autom autom) {
    for (size_t i = 0; i < autom.cols_len; i++) {
        autom.cols[i] = false;
    }
    autom.cols[autom.cols_len / 2] = true;
    autom.cols[(size_t)(ceilf(autom.cols_len / 2.0) - 1)] = true;
}

void autom_randomize(Autom autom) {
    for (size_t i = 0; i < autom.cols_len; i++) {
        autom.cols[i] = rand() < RAND_MAX / 2;
    }
}

void autom_advance(Autom *autom) {
    bool *next_cols = calloc(autom->cols_len, sizeof(bool));

    for (size_t i = 0; i < autom->cols_len; i++) {
        bool upper_bit = autom->cols[(i - 1 + autom->cols_len) % autom->cols_len];
        bool middle_bit = autom->cols[i];
        bool lower_bit = autom->cols[(i + 1) % autom->cols_len];
        int rule_bit_idx = (upper_bit << 2) | (middle_bit << 1) | lower_bit;
        next_cols[i] = (autom->rule >> rule_bit_idx) & 1;
    }

    free(autom->cols);
    autom->cols = next_cols;
}

void autom_print_step(Autom autom, char block, char space) {
    for (size_t i = 0; i < autom.cols_len; i++) {
        putchar(autom.cols[i] ? block : space);
    }
    putchar('\n');
}

void autom_print_all(Autom *autom, char block, char space) {
    for (size_t i = 0; i < autom->rows_len; i++) {
        autom_print_step(*autom, block, space);
        autom_advance(autom);
    }
}

void autom_print_nonstop(Autom *autom, char block, char space) {
    while (true) {
        autom_print_step(*autom, block, space);
        autom_advance(autom);
        usleep(31250);
    }
}

void autom_rasterize_row(Autom autom, char *pixel_buff) {
    char *col_begin = pixel_buff;
    for (size_t col = 0; col < autom.cols_len; col++) {
        for (size_t i = 0; i < autom.img_scale * 3; i++) {
            *col_begin = !autom.cols[col];
            col_begin++;
        }
    }
}

void autom_extract_ppm(Autom *autom) {
    char *file_name = calloc(strlen(autom->file_name) + 5, sizeof(char));
    strcpy(file_name, autom->file_name);
    strcat(file_name, ".ppm");

    remove(file_name);
    FILE *fptr = fopen(file_name, "a");
    fprintf(fptr, "P6 %llu %llu 1\n", autom->cols_len * autom->img_scale, autom->rows_len * autom->img_scale);
    char *row_pixel_buff = calloc(autom->cols_len * autom->img_scale * 3, sizeof(char));

    for (size_t row = 0; row < autom->rows_len; row++) {
        autom_rasterize_row(*autom, row_pixel_buff);
        for (size_t i = 0; i < autom->img_scale; i++) {
            fwrite(row_pixel_buff, sizeof(char), autom->img_scale * 3 * autom->cols_len, fptr);
        }
        autom_advance(autom);
    }

    free(row_pixel_buff);
    free(file_name);
    fclose(fptr);
}
