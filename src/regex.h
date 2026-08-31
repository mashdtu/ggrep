#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

/**
 * checks whether the entire string matches the regular expression pattern.
 * @arg string the string to match
 * @arg pattern the regular expression pattern
 * @return true if the string matches, false otherwise
 */
bool contains_regex(const char *string, const char *pattern);

#endif
