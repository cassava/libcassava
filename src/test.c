/*
 * libcassava/test.c
 * vim: set cin ts=4 sw=4 fdm=syntax et:
 *
 * Copyright (c) 2012 Ben Morgan <neembi@googlemail.com>
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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "bitset.h"
#include "debug.h"
#include "list.h"
#include "list_str.h"
#include "string.h"
#include "system.h"
#include "util.h"

//: string.h
void test_strclone(char *input)
{
    char *str;
    printf("test_str_clone(%s)\n", input);

    str = cs_strclone("hello world!");
    puts(str);
    free(str);

    str = cs_strclone("");
    puts(str);
    free(str);

    str = cs_strclone(input);
    puts(str);
    free(str);
}

//: list.h
void test_list_filter(char *path)
{
    printf("test_list_filter(%s)\n", path);
    NodeStr *head;
    get_filepaths(path, &head);

    struct filter_regex_args args;
    regcomp(&args.preg, "benmorgan", REG_EXTENDED);
    list_filter(&head, filter_regex, &args);
    regfree(&args.preg);
    list_free_all(&head);
}

//: system.h
void test_get_filepaths(char *path)
{
    printf("test_get_filepaths(%s)\n", path);
    NodeStr *head;
    get_filepaths(path, &head);
    list_free_all(&head);
}

void test_get_filenames_filter_regex(char *path)
{
    printf("test_get_filenames_filter_regex(%s)\n", path);
    NodeStr *head;
    get_filenames_filter_regex(path, &head, "^[^.].*$");
    list_free_all(&head);
}

void test_get_filenames(char *path)
{
    printf("test_get_filenames(%s)\n", path);
    NodeStr *head;
    get_filenames(path, &head);
    list_free_all(&head);
}

//: util.h
void test_print_columns(char *path)
{
    printf("test_print_columns(%s)\n", path);

    NodeStr *head;
    char **array;

    get_filenames_filter_regex(path, &head, "^[^.].*$");
    size_t count = list_to_array(head, (void ***)&array);
    print_columns(array, count);

    free(array);
    list_free_all(&head);
}

//: bitset.h
void test_bitset(const char *binstr)
{
    printf("test_bitset(%s)\n", binstr);

    bitset_t *bs = new_bitset(strlen(binstr));
    size_t count = 0;
    while (*binstr != '\0')
        set_bit(bs, count++, *binstr++ == '1' ? true : false);
    size_t i = 0;
    while (i < count)
        putchar(get_bit(bs, i++) ? '1' : '0');
    putchar('\n');
}


int main(int argc, char **argv)
{
    puts("running main()...");

    char *path = "/etc";
    if (argc > 1)
        path = argv[1];

    goto bitset;

string:
    puts("testing string.h functions...");
    test_strclone(path);

list:
    puts("testing list.h functions...");
    test_list_filter(path);

system:
    puts("testing system.h functions...");
    test_get_filepaths(path);
    test_get_filenames(path);
    test_get_filenames_filter_regex(path);
    test_print_columns(path);

bitset:
    puts("testing bitset.h functions...");
    test_bitset(path);

    return 0;
}
