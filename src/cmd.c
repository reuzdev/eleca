#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"

#define ERR_MSG(before_colon, after_colon) "error (" before_colon "): " after_colon " (--help for usage)\n"

bool str_starts_w(char *str, char *prefix) {
    return !strncmp(str, prefix, strlen(prefix));
}

bool parser_find_option_w_long_flag(ParserData data, char *long_flag, Option *option) {
    for (size_t i = 0; i < data.optionc; i++) {
        if (!strcmp(data.optionv[i].long_flag, long_flag)) {
            *option = data.optionv[i];
            return true;
        }
    }
    return false;
}

bool parser_find_option_w_short_flag(ParserData data, char short_flag, Option *option) {
    for (size_t i = 0; i < data.optionc; i++) {
        if (data.optionv[i].short_flag == short_flag) {
            *option = data.optionv[i];
            return true;
        }
    }
    return false;
}

void parser_init_data(ParserData *data, char **argv, size_t argc, Option *optionv, size_t optionc) {
    data->argv = argv;
    data->argc = argc;
    data->optionv = optionv;
    data->optionc = optionc;
    data->curr_idx = 1;
}

bool parser_is_finished(ParserData data) {
    return data.curr_idx >= data.argc;
}

char* parser_consume_arg(ParserData *data) {
    if (parser_is_finished(*data))
        return NULL;
    return data->argv[data->curr_idx++];
}

void parser_parse(ParserData *data) {
    while (!parser_is_finished(*data)) {
        char *flag_arg = parser_consume_arg(data);
        Option option;

        if (str_starts_w(flag_arg, "--")) {
            char *flag = flag_arg + 2;
            if (!parser_find_option_w_long_flag(*data, flag, &option)) {
                fprintf(stderr, ERR_MSG("%s", "unknown flag"), flag_arg);
                exit(1);
            }
        }
        else if (str_starts_w(flag_arg, "-")) {
            if (strlen(flag_arg) != 2) {
                fprintf(stderr, ERR_MSG("%s", "short flags must only consist of a dash and a letter"), flag_arg);
                exit(1);
            }
            char flag = flag_arg[1];
            if (!parser_find_option_w_short_flag(*data, flag, &option)) {
                fprintf(stderr, ERR_MSG("%s", "unknown flag"), flag_arg);
                exit(1);
            }
        }
        else {
            fprintf(stderr, ERR_MSG("%s", "argument not associated with any flags"), flag_arg);
            exit(1);
        }

        if (option.format == NULL) {
            *(bool*)option.ptr = true;
            continue;
        }

        char *value_arg = parser_consume_arg(data);
        if (value_arg == NULL) {
            fprintf(stderr, ERR_MSG(, "expected value after `%s`"), flag_arg);
            exit(1);
        }

        if (!strcmp(option.format, "%s")) {
            *(char**)option.ptr = value_arg;
            continue;
        }

        if (!sscanf(value_arg, option.format, option.ptr)) {
            fprintf(stderr, ERR_MSG("%s", "invalid value for `%s`"), value_arg, flag_arg);
            exit(1);
        }
    }
}
