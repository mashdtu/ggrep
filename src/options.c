#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void)
{
    printf("Usage: ggrep [options] pattern [file...]\n");
    printf("\n");
    printf("Search for a literal substring in files or stdin.\n");
    printf("\n");
    printf("Options:\n");
    printf("  -i        Ignore case when matching\n");
    printf("  -n        Print line numbers\n");
    printf("  -v        Invert match (print non-matching lines)\n");
    printf("  -h        Show this help message\n");
    printf("  --help    Show this help message\n");
}

int parse_options(int argc, char *argv[], Options *opts)
{
    opts->ignore_case = false;
    opts->line_numbers = false;
    opts->invert = false;
    opts->pattern = NULL;
    opts->file_count = 0;
    opts->files = NULL;

    int i = 1;

    for (; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            break;
        }

        if (strcmp(argv[i], "--help") == 0)
        {
            print_usage();
            exit(EXIT_SUCCESS);
        }

        if (strcmp(argv[i], "--") == 0)
        {
            i++;
            break;
        }

        const char *p = &argv[i][1];
        while (*p)
        {
            switch (*p)
            {
            case 'i':
                opts->ignore_case = true;
                break;
            case 'n':
                opts->line_numbers = true;
                break;
            case 'v':
                opts->invert = true;
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "ggrep: unknown option '-%c'\n", *p);
                return -1;
            }
            p++;
        }
    }

    if (i >= argc)
    {
        fprintf(stderr, "ggrep: missing pattern\n");
        fprintf(stderr, "Try 'ggrep --help' for more information.\n");
        return -1;
    }

    opts->pattern = argv[i];
    i++;

    opts->files = &argv[i];
    opts->file_count = argc - i;

    return 0;
}
