#include "options.h"
#include "search.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    Options opts;

    if (parse_options(argc, argv, &opts) != 0)
    {
        return 2;
    }

    int found = 0;

    if (opts.file_count == 0) {
        found = (search_stdin(&opts) == 0);
    } else {
        for (int i = 0; i < opts.file_count; i++) {
            if (search_file(opts.files[i], &opts) == 0) {
                found = 1;
            }
        }
    }

    return found ? 0 : 1;
}
