/*
 * bitset.h
 * vim: set cindent expandtab tabstop=4 shiftwidth=4 colorcolumn=81:
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

#ifndef LIBCASSAVA_BITSET_H
#define LIBCASSAVA_BITSET_H

#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int bitset_t;

bitset_t *new_bitset(size_t elements);

/**
 * Get a bit stored at position \a pos in the bitset \a array.
 *
 * Behaviour is undefined if \a pos is outside of bitset array bounds.
 */
bool get_bit(const bitset_t *array, size_t pos);

/**
 * Set a bit stored at position \a pos in the bitset \a array.
 *
 * Behaviour is undefined if \a pos is outside of bitset array bounds.
 */
void set_bit(bitset_t *array, size_t pos, bool value);

#endif /* LIBCASSAVA_BITSET_H */
