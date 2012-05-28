/*
 * libcassava/util.c
 * vim: set cin ts=4 sw=4 et cc=101:
 *
 * Copyright (c) 2011-2012 Ben Morgan <neembi@googlemail.com>
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

#include "util.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"
#include "system.h"
#include "debug.h"

static unsigned sum_over(const size_t *array, size_t len);
static void putfill(const char *string, size_t size);

/**
 * \details The implementation of this function takes \$ O(n^2) \$ time.
 *
 * TODO Test extreme cases: with 0 elements.
 */
void print_columns(char **array, size_t len)
{
#define pos(col, row) (col)*(num_rows) + (row)
    assert(array != NULL);

    size_t str_len[len], col_len[len];
    size_t num_cols = len, num_rows, col, row, i;

    for (i = 0; i < len; i++) {
        str_len[i] = strlen(array[i]);
        col_len[i] = 0;
    }

    /* Find out how many columns/rows we will have: */
    size_t term_width = get_terminal_columns();
    if (term_width == 0) term_width = 80;
    for (num_rows = 1; num_rows < len; num_rows++) {
        num_cols = ceil(((double)len)/num_rows);
        for (col = 0; col < num_cols; col++)
            col_len[col] = 0;

        for (col = 0; col < num_cols; col++) {
            for (row = 0; row < num_rows && (i = pos(col, row)) < len; row++) {
                if (str_len[i] > col_len[col])
                    col_len[col] = str_len[i];
            }
        }

        /* I am leaving PRINT_COLUMNS_SEP_WIDTH + 1 columns on the edge for readability! */
        if ((sum_over(col_len, num_cols) + PRINT_COLUMNS_SEP_WIDTH*num_cols) < term_width) {
            break;
        }
    }

    /* Print the strings in columns: */
    /* TODO: prevent printing of two spaces at the end! */
    debug_printf("len = %u, num_cols = %u, num_rows = %u\n", len, num_cols, num_rows);
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < num_cols; col++) {
            size_t idx = pos(col, row);
            if (idx < len)
                putfill(array[idx], col_len[col] + PRINT_COLUMNS_SEP_WIDTH);
            else
                break;
        }
        putchar('\n');
    }
#undef pos
}

/**
 * Return the sum over an array of size_t.
 */
static unsigned sum_over(const size_t *array, size_t len)
{
    assert(array != NULL);

    size_t i, sum = 0;
    for (i = 0; i < len; i++)
        sum += array[i];
    return sum;
}

/**
 * Put a string left aligned to stdout, padding the right with spaces.
 */
static void putfill(const char *string, size_t size)
{
    assert(string != NULL);

    size_t i = 0;
    for (; *string != '\0'; i++)
        putchar(*string++);
    while (i++ < size)
        putchar(' ');
}
