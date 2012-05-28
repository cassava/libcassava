/*
 * libcassava/string.c
 * vim: set cin ts=4 sw=4 et:
 *
 * Copyright (c) 2011-2012 Ben Morgan <neembi@googlemail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

static int compare_str(const void *, const void *);

char *cs_strclone(const char *input)
{
    assert(input != NULL);

    size_t len = strlen(input);
    char *clone = malloc(sizeof (char) * (len+1));
    if (clone != NULL)
        strcpy(clone, input);
    return clone;
}

char *cs_strcat(const char *f1, const char *f2)
{
    assert(f1 != NULL);
    assert(f2 != NULL);

    size_t len = strlen(f1);
    char *str = malloc((len + strlen(f2) + 1) * sizeof (char));
    if (str != NULL) {
        strcpy(str, f1);
        strcpy(str+len, f2);
    }
    return str;
}

char *cs_strjoin(char **arr, int n, const char *delim, int extra)
{
    char *str, *t;
    const char *s;
    size_t len = extra + 1;  /* one extra for '\0' */
    int i, m;

    m = strlen(delim);
    if (m == 0)
        return cs_stracat(NULL, arr, n, extra);
    len += (n-1) * m;
    for (i = 0; i < n; i++)
        len += strlen(arr[i]);
    t = str = malloc(len * sizeof (char));
    for (i = 0; i < n; i++) {
        s = arr[i];
        while ((*t = *s) != '\0')
            (t++, s++);
        if (i < n-1) {
            /* delimeter only between elements */
            s = delim;
            while ((*t = *s) != '\0')
                (t++, s++);
        }
    }
    return str;
}

char *cs_stracat(const char *f, char **arr, int n, int extra)
{
    char *str, *t;
    const char *s;
    size_t len = extra + 1;
    int i;

    if (f != NULL)
        len += strlen(f);
    for (i = 0; i < n; i++) {
        len += strlen(arr[i]);
    }
    t = str = malloc(len * sizeof (char));
    if (f != NULL)
        while ((*t = *f) != '\0')
            (t++, f++);
    for (i = 0; i < n; i++) {
        s = arr[i];
        while ((*t = *s) != '\0')
            (t++, s++);
    }
    return str;
}

char *cs_strvcat(const char *f, ...)
{
    va_list ap;
    char *str, *t;
    const char *s;
    size_t len = 1;

    assert(f != NULL);

    /* determine length of final string */
    va_start(ap, f); s=f;
    while (s != NULL) {
        len += strlen(s);
        s = va_arg(ap, char *);
    }
    va_end(ap);

    /* create new string */
    str = t = malloc(len * sizeof (char));
    if (str != NULL) {
        va_start(ap, f);
        s=f; /* we start with f */
        while (s != NULL) {
            strcpy(t, s);
            t += strlen(s);
            s = va_arg(ap, char *);
        }
        va_end(ap);
    }

    return str;
}

char *cs_substr(const char *input, unsigned start_offset, unsigned end_offset)
{
    size_t size;
    char *result;

    assert(end_offset > start_offset);
    assert(strlen(input) > end_offset);

    size = end_offset - start_offset;
    result = malloc((size+1) * sizeof (char));

    input += start_offset;
    *(result + size) = '\0'; 
    while (size-- > 0)
        *(result+size) = *(input+size);

    return result;
}

bool cs_isprefix(const char *child, const char *parent)
{
    assert(child != NULL);
    assert(parent != NULL);

    while (*child != '\0')
        if (*child++ != *parent++)
            return false;
    return true;
}

void cs_qsort(char **array, size_t len)
{
    qsort(array, len, sizeof (char *), compare_str);
}

/**
 * Comparison function for qsort() in cs_qsort().
 *
 * The actual arguments to this function are "pointers to
 * pointers to char", but strcmp(3) arguments are "pointers
 * to char", hence the following cast plus dereference.
 *
 * \param p1 String one.
 * \param p2 String two.
 * \return -1 if p1 < p2, 0 if p1 == p2, 1 if p1 > p2.
 */
static int compare_str(const void *p1, const void *p2)
{
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

