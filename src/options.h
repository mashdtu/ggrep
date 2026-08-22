#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

/**
 * options for the ggrep program
 */
typedef struct
{
    bool ignore_case;    // whether to ignore case when matching
    bool line_numbers;   // whether to print line numbers
    bool invert;         // whether to invert match (print non-matching lines)
    const char *pattern; // the pattern to search for
    int file_count;      // the number of files to search
    char **files;        // the list of files to search
} Options;

/**
 * prints the usage information for the ggrep program.
 */
void print_usage(void);

/**
 * parses the command line options and arguments for the ggrep program.
 * @arg argc the number of command line arguments
 * @arg argv the array of command line arguments
 * @arg opts the options struct to populate with parsed values
 * @return 0 on success, -1 on error
 */
int parse_options(int argc, char *argv[], Options *opts);

#endif
