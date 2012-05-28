/*
 * libcassava/list_str.c
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
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "list_str.h"
#include "list.h"
#include "string.h"

#include <assert.h>
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


void list_print(const NodeStr *head, const char *sep)
{
    const NodeStr *iter;

    if (sep == NULL)
        sep = "";

    for (iter = head; iter->next != NULL; iter = iter->next)
        printf("%s%s", iter->data, sep);
    puts(iter->data);
}

void list_println(const NodeStr *head, const char *prefix)
{
    const NodeStr *iter;

    if (prefix == NULL)
        prefix = "";

    for (iter = head; iter != NULL; iter = iter->next)
        printf("%s%s\n", prefix, iter->data);
}

const NodeStr *list_search(const NodeStr *haystack, const char *needle)
{
    const NodeStr *iter;

    assert(needle != NULL);

    for (iter = haystack; iter != NULL; iter = iter->next)
        if (strcmp(iter->data, needle) == 0)
            return iter;

    return NULL;
}

char *list_strjoin(const NodeStr *head, const char *delim)
{
    char *str, *t;
    const char *s;
    size_t len = 1;
    size_t m, n;
    const NodeStr *node;

    n = list_length(head);
    if (n == 0)
        return NULL;

    m = strlen(delim);
    len += (n-1) * m;
    for (node = head; node != NULL; node = node->next)
        len += strlen(node->data);

    t = str = malloc(len * sizeof (char));
    for (node = head; node != NULL; node = node->next, n--) {
        s = node->data;
        while ((*t = *s) != '\0')
            t++, s++;
        if (n > 1) {
            s = delim;
            while ((*t = *s) != '\0')
                t++, s++;
        }
    }
    return str;
}

int list_filter_regex(NodeStr **head, const char *regex)
{
    char errbuf[MAX_ERROR_LINE_LENGTH];      /* for holding error messages by regex.h */
    int errcode;
    int count = 0;
    NodeStr *current;
    NodeStr *iter;

    /* compile regex */
    regex_t preg;
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return -1;
    }

    current = NULL;
    iter = *head;
    while (iter != NULL) {
        errcode = regexec(&preg, iter->data, 0, NULL, 0);
        if (errcode == 0) {
            if (current == NULL) {
                *head = current = iter;
            } else {
                current->next = iter;
                current = iter;
            }
            count++;
            iter = iter->next;
        } else if (errcode == REG_NOMATCH) {
            NodeStr *temp = iter->next;
            free(iter->data);
            free(iter);
            iter = temp;
        } else {
            regerror(errcode, &preg, errbuf, sizeof errbuf);
            fprintf(stderr, "Error (regexec): %s\n", errbuf);
            return -1;
        }
    }

    regfree(&preg);
    if (current != NULL)
        current->next = NULL;
    return count;
}
