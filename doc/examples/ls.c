/*
 * libcassava/examples/ls.c
 * vim: set cindent expandtab tabstop=4 shiftwidth=4 colorcolumn=101:
 *
 * Copyright (c) 2012 Ben Morgan <neembi@googlemail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file
 * \b ls.c - an extremely simple implementation of the *nix system tool \e ls.
 * Compile like this:
 *   gcc -std=c99 -O2 -Wall -Wextra -pedantic -o ls ls.c -lcassava -lm
 *
 * If you compile ls.c this way, or even with -march=native, then it is
 * significantly faster than ls that comes with the system, obviously with less
 * features however.
 *
 * \note \e ls takes into account the locale when it sorts, this program does
 * not do that, rather it uses the C locale.
 */

#include <stdlib.h>
#include <stdio.h>

#include <libcassava/list.h>
#include <libcassava/list_str.h>
#include <libcassava/string.h>
#include <libcassava/system.h>
#include <libcassava/util.h>

int list(const char *path)
{
    NodeStr *head;
    int retval = 0;
    int result = get_filenames_filter_regex(path, &head, "^[^.].*");

    // Ignore the case when result is 0; we don't want to waste function calls!
    if (result < 0) {
        printf("Error: cannot read '%s'.\n", path);
        retval = 1;
    } else if (result > 0) {
        char **array;
        size_t len = list_to_array(head, (void ***)&array);
        cs_qsort(array, len);
        print_columns(array, len);
        free(array);
    }

    list_free_all(&head);
    return retval;
}

int main(int argc, char **argv)
{
    int retval = 0;

    if (argc == 1) {
        retval |= list(".");
    } else {
        while (--argc > 0)
            retval |= list(*++argv);
    }

    return retval;
}
