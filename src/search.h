#ifndef SEARCH_H
#define SEARCH_H

#include "options.h"

/**
 * searches for the specified pattern in the given file using the provided options.
 * @arg filename the name of the file to search
 * @arg opts the options struct containing search parameters
 * @return 0 on success, -1 on error
 */
int search_file(const char *filename, const Options *opts);

/**
 * searches for the specified pattern in the standard input using the provided options.
 * @arg opts the options struct containing search parameters
 * @return 0 on success, -1 on error
 */
int search_stdin(const Options *opts);

#endif
