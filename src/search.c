#define _POSIX_C_SOURCE 200809L

#include "regex.h"
#include "search.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * checks if the given line contains the specified pattern.
 * @arg line the line to search
 * @arg pattern the pattern to search for
 * @return true if the pattern is found in the line, false otherwise
 */
static bool contains(const char *line, const char *pattern)
{
    size_t line_len = strlen(line);
    size_t pat_len = strlen(pattern);

    if (pat_len == 0)
    {
        return true;
    }

    if (pat_len > line_len)
    {
        return false;
    }

    for (size_t i = 0; i <= line_len - pat_len; i++)
    {
        size_t j = 0;
        while (j < pat_len && line[i + j] == pattern[j])
        {
            j++;
        }
        if (j == pat_len)
        {
            return true;
        }
    }

    return false;
}

/**
 * checks if the given line contains the specified pattern, ignoring case.
 * @arg line the line to search
 * @arg pattern the pattern to search for
 * @return true if the pattern is found in the line, false otherwise
 */
static bool contains_icase(const char *line, const char *pattern)
{
    size_t line_len = strlen(line);
    size_t pat_len = strlen(pattern);

    if (pat_len == 0)
    {
        return true;
    }

    if (pat_len > line_len)
    {
        return false;
    }

    for (size_t i = 0; i <= line_len - pat_len; i++)
    {
        size_t j = 0;
        while (j < pat_len && tolower((unsigned char)line[i + j]) ==
                                  tolower((unsigned char)pattern[j]))
        {
            j++;
        }
        if (j == pat_len)
        {
            return true;
        }
    }

    return false;
}

/**
 * checks if the given line matches the search criteria specified in the options.
 * @arg line the line to check
 * @arg opts the options specifying the search criteria
 * @return true if the line matches the criteria, false otherwise
 */
static bool matches(const char *line, const Options *opts)
{
    bool found;

    if (opts->regex)
        found = contains_regex(line, opts->pattern);
    else if (opts->ignore_case)
        found = contains_icase(line, opts->pattern);
    else
        found = contains(line, opts->pattern);

    return opts->invert ? !found : found;
}

/**
 * removes the trailing newline character from the given line, if present.
 * @arg line the line to modify
 */
static void strip_newline(char *line)
{
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }
}

/**
 * searches the given file stream for lines matching the specified pattern and options.
 * @arg fp the file stream to search
 * @arg filename the name of the file (for display purposes)
 * @arg opts the options specifying the search criteria
 * @arg show_filename whether to display the filename in the output
 * @return 0 if any matching lines were found, 1 if no matches were found
 */
static int search_stream(FILE *fp, const char *filename, const Options *opts,
                         bool show_filename)
{
    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    int line_num = 0;
    int found = 0;

    while ((len = getline(&line, &cap, fp)) != -1)
    {
        line_num++;
        strip_newline(line);

        if (matches(line, opts))
        {
            found = 1;
            if (show_filename)
            {
                printf("%s:", filename);
            }
            if (opts->line_numbers)
            {
                printf("%d:", line_num);
            }
            printf("%s\n", line);
        }
    }

    free(line);
    return found ? 0 : 1;
}

int search_file(const char *filename, const Options *opts)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        fprintf(stderr, "ggrep: cannot open '%s': ", filename);
        perror(NULL);
        return 2;
    }

    bool show_filename = (opts->file_count > 1);
    int result = search_stream(fp, filename, opts, show_filename);

    fclose(fp);
    return result;
}

int search_stdin(const Options *opts)
{
    return search_stream(stdin, "(stdin)", opts, false);
}
