#pragma once

typedef struct {
    char short_flag;
    char *long_flag;
    char *format; // null if expects no parameters (i.e. sets boolean value)
    char *label;
    char *description;
    void *ptr;
} Option;

typedef struct {
    char **argv;
    size_t argc;
    size_t curr_idx;
    Option *optionv;
    size_t optionc;
} ParserData;

void parser_init_data(ParserData *data, char **argv, size_t argc, Option *optionv, size_t optionc);

void parser_parse(ParserData *data);

bool parser_find_option_w_long_flag(ParserData data, char *long_flag, Option *option);

bool parser_find_option_w_short_flag(ParserData data, char short_flag, Option *option);

bool parser_is_finished(ParserData data);

char* parser_consume_arg(ParserData *data);
