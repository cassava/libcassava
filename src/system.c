/*
 * libcassava/system.c
 * vim: set cin ts=4 sw=4 et:
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

#include "system.h"

#include <dirent.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "list.h"
#include "list_str.h"
#include "string.h"

static inline int isdir(const char *path);
extern inline int get_filenames(const char *path, NodeStr **head);
extern inline int get_filepaths(const char *path, NodeStr **head);
extern inline int get_filenames_filter(const char *path, NodeStr **head,
        bool (*filter)(void *path, void *arguments), void *arguments);
extern inline int get_filepaths_filter(const char *path, NodeStr **head,
        bool (*filter)(void *path, void *arguments), void *arguments);
extern inline int get_filepaths_filter_regex(const char *path, NodeStr **head, const char *regex);
extern inline int get_filenames_filter_regex(const char *path, NodeStr **head, const char *regex);

unsigned short get_terminal_columns()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

int read_directory(const char *path, NodeStr **head, bool full_pathnames)
{
    assert(path != NULL);
    assert(head != NULL);

    /* Initialize head and count. */
    *head = NULL;
    int count = 0;

    /* Open dir specified by path. */
    DIR *dp = opendir(path);
    if (dp == NULL) goto error;

    /* Append directory entries into linked list. */
    struct dirent *entry;
    NodeStr *current = NULL;
    bool trail = (*(path+strlen(path)-1) == '/');
    while ((entry = readdir(dp)) != NULL) {
        NodeStr *new = list_node();
        if (full_pathnames)
            new->data = cs_strvcat(path, trail ? "" : "/", entry->d_name, NULL);
        else
            new->data = cs_strclone(entry->d_name);

        if (current == NULL) {
            *head = current = new;
        } else {
            current->next = new;
            current = current->next;
        }
        ++count;
    }

    closedir(dp);
    return count;

error:
    perror("Error (read_directory)");
    return -1;
}

int read_directory_filter_regex(const char *path, NodeStr **head, const char *regex, bool full_pathnames)
{
    assert(regex != NULL);

    int retval;
    struct filter_regex_args args;
    int errcode = regcomp(&args.preg, regex, REG_EXTENDED);
    if (errcode != 0) {
        char errbuf[BUFSIZ];
        regerror(errcode, &args.preg, errbuf, sizeof errbuf);
        fprintf(stderr, "Error (regcomp): %s\n", errbuf);
        retval = -1;
        goto finally;
    }

    if (full_pathnames)
        retval = get_filepaths_filter(path, head, filter_regex, (void *)&args);
    else
        retval = get_filenames_filter(path, head, filter_regex, (void *)&args);

finally:
    regfree(&args.preg);
    return retval;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool filter_isdir(void *filepath, void *unused)
{
    return isdir(filepath) == 1;
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool filter_isreg(void *filepath, void *unused)
{
    return isdir(filepath) == 0;
}
#pragma GCC diagnostic pop

bool filter_mtime(void *filepath, void *arguments)
{
    assert(arguments != NULL);
    struct filter_time_args *args = (struct filter_time_args *)arguments;

    struct stat fstat;
    if (stat(filepath, &fstat) != 0) {
        perror("Error (stat)");
        return false;
    }

    if (args->comparison < 0)
        return (fstat.st_mtime < args->time);
    else if (args->comparison > 0)
        return (fstat.st_mtime > args->time);
    else
        return (fstat.st_mtime == args->time);
}

/**
 * Returns whether \a filepath points to a directory.
 *
 * \param filepath File to test.
 * \return 0 if regular file, 1 if directory, -1 if error;
 */
static inline int isdir(const char *filepath)
{
    assert(filepath != NULL);

    struct stat fstat;
    if (stat(filepath, &fstat) != 0) {
        perror("Error (stat)");
        return -1;
    }
    return ! S_ISREG(fstat.st_mode);
}
