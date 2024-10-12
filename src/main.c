#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "autom.h"
#include "cmd.h"

void print_usage() {
    printf(
        "Options:\n"
        "   -R, --rule     {number}    set rule as integer (default 110)\n"
        "   -c, --column   {number}    set amount of cells horizontally\n"
        "   -r, --row      {number}    set amount of cells vertically\n"
        "   -s, --scale    {number}    set image scale to a positive integer\n"
        "   -f, --file     {string}    set file name of extracted image\n"
        "   -C, --centered             make first row centered\n"
        "   -e, --extract              extract final state as .ppm image\n"
        "   -S, --scroll               slowly print rows non-stop\n"
        "   -h, --help                 see this message\n"
    );
}

int main(int argc, char **argv) {
    srand(time(NULL));

    Autom autom = autom_create(20, 20, 1, 110);
    autom.file_name = "eleca-image";

    bool extract = false;
    bool scroll = false;
    bool help = false;
    bool center = false;

    Option options[] = {
        (Option) {
            .long_flag = "column",
            .short_flag = 'c',
            .format = "%llu",
            .ptr = &autom.cols_len,
        },
        (Option) {
            .long_flag = "row",
            .short_flag = 'r',
            .format = "%llu",
            .ptr = &autom.rows_len,
        },
        (Option) {
            .long_flag = "rule",
            .short_flag = 'R',
            .format = "%u",
            .ptr = &autom.rule,
        },
        (Option) {
            .long_flag = "scale",
            .short_flag = 's',
            .format = "%llu",
            .ptr = &autom.img_scale,
        },
        (Option) {
            .long_flag = "file",
            .short_flag = 'f',
            .format = "%s",
            .ptr = &autom.file_name,
        },
        (Option) {
            .long_flag = "centered",
            .short_flag = 'C',
            .format = NULL,
            .ptr = &center,
        },
        (Option) {
            .long_flag = "extract",
            .short_flag = 'e',
            .format = NULL,
            .ptr = &extract,
        },
        (Option) {
            .long_flag = "scroll",
            .short_flag = 'S',
            .format = NULL,
            .ptr = &scroll,
        },
        (Option) {
            .long_flag = "help",
            .short_flag = 'h',
            .format = NULL,
            .ptr = &help,
        },
    };

    ParserData parser_data;
    parser_init_data(&parser_data, argv, argc, options, sizeof(options)/sizeof(Option));
    parser_parse(&parser_data);
    autom_realloc(&autom);

    if (help) {
        print_usage();
        return 0;
    }

    if (center)
        autom_make_centered(autom);
    else
        autom_randomize(autom);

    if (extract)
        autom_extract_ppm(&autom);
    else if (scroll)
        autom_print_nonstop(&autom, '@', '.');
    else
        autom_print_all(&autom, '@', '.');

    autom_discard(autom);
    return 0;
}
