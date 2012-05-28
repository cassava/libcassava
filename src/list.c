/*
 * libcassava/list.c
 * vim: set cin ts=4 sw=4 cc=101 et:
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

#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


/*
 * struct list_node is defined by the user of this library.
 * See list.h for more information on that.
 */
struct list_node {
    void *data;
    struct list_node *next;
};

struct list_node *list_node()
{
    struct list_node *ptr;

    ptr = malloc(sizeof (struct list_node));
    ptr->data = NULL;
    ptr->next = NULL;
    return ptr;
}

bool list_empty(const struct list_node *head)
{
    return (head == NULL);
}

size_t list_length(const struct list_node *head)
{
    size_t n = 0;

    while (head != NULL) {
        head = head->next;
        n++;
    }
    return n;
}

size_t list_to_array(const struct list_node *head, void ***output)
{
    assert(output != NULL);

    const struct list_node *iter;
    size_t count = 1;

    /* count how many non-NULL items there are */
    for (iter = head; iter != NULL; iter = iter->next)
        if (iter->data != NULL)
            count++;

    /* add all non-NULL items to array and terminate with NULL */
    *output = malloc(count * sizeof (void *));
    if (count > 1) {
        size_t index = 0;
        for (iter = head; iter != NULL; iter = iter->next)
            if (iter->data != NULL)
                (*output)[index++] = iter->data;
    }
    (*output)[count-1] = NULL;

    return count-1;
}

void list_push(struct list_node **head, void *data)
{
    assert(head != NULL);

    struct list_node *node = list_node();
    node->data = data;

    node->next = *head;
    *head = node;
}

void *list_pop(struct list_node **head)
{
    assert(head != NULL);

    struct list_node *next;
    void *data;

    if (list_empty(*head))
        return NULL;

    data = (*head)->data;
    next = (*head)->next;
    free(*head);
    *head = next;
    return data;
}

void list_insert(struct list_node **head, struct list_node *node)
{
    struct list_node *tail;

    assert(head != NULL);
    if (list_empty(*head)) {
        *head = node;
        return;
    }

    tail = (*head)->next;
    (*head)->next = node;
    if (tail != NULL) {
        while (node->next != NULL)
            node = node->next;
        node->next = tail;
    }
}

struct list_node *list_remove(struct list_node **head)
{
    assert(head != NULL);

    struct list_node *top;
    struct list_node *next;

    if (list_empty(*head))
        return NULL;

    next = (*head)->next;
    top = *head;
    top->next = NULL;
    *head = next;
    return top;
}

size_t list_filter(struct list_node **head, bool (*filter)(void *, void *), void *arguments)
{
    assert(head != NULL);
    assert(filter != NULL);

    struct list_node *iter = *head;
    struct list_node *current = *head = NULL;
    size_t count = 0;
    while (iter != NULL) {
        if (filter(iter->data, arguments)) {
            if (current == NULL) {
                *head = current = iter;
            } else {
                current->next = iter;
                current = current->next;
            }
            iter = iter->next;
            current->next = NULL;
            ++count;
        } else {
            struct list_node *prev = iter;
            iter = iter->next;
            free(prev->data);
            free(prev);
        }
    }

    return count;
}

void list_free_nodes(struct list_node **head)
{
    assert(head != NULL);

    while (*head != NULL) {
        struct list_node *temp = (*head)->next;
        free(*head);
        *head = temp;
    }
    *head = NULL;
}

void list_free_all(struct list_node **head)
{
    assert(head != NULL);

    while (*head != NULL) {
        struct list_node *temp;
        if ((*head)->data != NULL)
            free((*head)->data);
        temp = (*head)->next;
        free(*head);
        *head = temp;
    }
    *head = NULL;
}
