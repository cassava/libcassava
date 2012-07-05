/*
 * bitset.c
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

#include "bitset.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define BITS_IN_UNIT   (8 * sizeof (bitset_t))
#define ARRAY_INDEX(x) ((x) / BITS_IN_UNIT + ((x) % BITS_IN_UNIT ? 1 : 0))

bitset_t *new_bitset(size_t elements)
{
    return malloc(ARRAY_INDEX(elements) * sizeof (bitset_t));
}

bool get_bit(const bitset_t *array, size_t pos)
{
    assert(array != NULL);

    return array[ARRAY_INDEX(pos)] & 1U << (pos % BITS_IN_UNIT);
}

void set_bit(bitset_t *array, size_t pos, bool value)
{
    assert(array != NULL);

    if (value)
        array[ARRAY_INDEX(pos)] |= (bitset_t) 1 << (pos % BITS_IN_UNIT);
    else
        array[ARRAY_INDEX(pos)] &= ~((bitset_t) 1 << (pos % BITS_IN_UNIT));
}
