/*
 * libcassava/util.c
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
#include "list.h"
#include "list_str.h"
#include "string.h"

#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


/*
 * TODO: This can be implemented much more elegantly.
 */
int get_regex_files(const char *regex, const char *dir, NodeStr **head)
{
    char errbuf[MAX_ERROR_LINE_LENGTH];      /* for holding error messages by regex.h */
    struct dirent *entry;
    int errcode, count, complete;
    NodeStr *current;
    regex_t preg;
    DIR *dp;

    /* initialize head and count */
    *head = NULL;
    count = 0;

    /* open db dir */
    dp = opendir(dir);
    if (dp == NULL) {
        perror("Error (opendir)");
        return -1;
    }

    /* compile regex */
    errcode = regcomp(&preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        regerror(errcode, &preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        return -1;
    }

    current = NULL;
    complete = (*(dir+strlen(dir)-1) == '/');
    while ((entry = readdir(dp)) != NULL) {
        /* make sure type is not dir, then do a regex on it */
        struct stat fstat;
        char *path = cs_strvcat(dir, complete ? "" : "/", entry->d_name, NULL);
        if (stat(path, &fstat) != 0) {
            perror("Error (stat)");
            free(path);
            return -1;
        }
        if (! S_ISREG(fstat.st_mode))
            goto free_path_continue;

        errcode = regexec(&preg, entry->d_name, 0, NULL, 0);
        if (errcode == 0) {
            NodeStr *new = list_node();
            new->data = path;
            if (current == NULL) {
                *head = current = new;
            } else {
                current->next = new;
                current = new;
            }
            count++;
        } else if (errcode == REG_NOMATCH) {
            goto free_path_continue;
        } else {
            free(path);
            regerror(errcode, &preg, errbuf, sizeof errbuf);
            fprintf(stderr, "Error (regexec): %s\n", errbuf);
            return -1;
        }
        continue;

    free_path_continue:
        free(path);
    }

    closedir(dp);
    regfree(&preg);
    if (current != NULL)
        current->next = NULL;
    return count;
}

int get_younger_files(const time_t age, const char *dir, NodeStr **head)
{
    struct dirent *entry;
    DIR *dp;
    int count, complete;
    NodeStr *current;

    /* initialize head and count */
    *head = NULL;
    count = 0;

    /* open db dir */
    dp = opendir(dir);
    if (dp == NULL) {
        perror("Error (opendir)");
        return -1;
    }

    current = NULL;
    complete = (*(dir+strlen(dir)-1) == '/');
    while ((entry = readdir(dp)) != NULL) {
        /* make sure type is not dir, then compare ages */
        struct stat fstat;
        char *path = cs_strvcat(dir, complete ? "" : "/", entry->d_name, NULL);
        if (stat(path, &fstat) != 0) {
            perror("Error (stat)");
            free(path);
            return -1;
        }

        if (S_ISREG(fstat.st_mode) && fstat.st_mtime > age) {
            NodeStr *new = list_node();
            new->data = path;
            if (current == NULL) {
                *head = current = new;
            } else {
                current->next = new;
                current = new;
            }
            count++;
            continue;
        }

        free(path);
    }

    closedir(dp);
    if (current != NULL)
        current->next = NULL;
    return count;
}

/* vim: set cin ts=4 sw=4 et: */
