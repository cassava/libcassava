/*
 * libcassava/system.h
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
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file
 * Functions that are pertinent to the system.
 *
 * \author Ben Morgan
 * \date 2012
 */

#ifndef LIBCASSAVA_SYSTEM_H
#define LIBCASSAVA_SYSTEM_H

/**
 * Get the number of columns in the current terminal.
 * This also works correctly if the terminal has been resized.
 *
 * \return Number of columns in current terminal.
 */
extern unsigned short get_terminal_columns();

#endif /* LIBCASSAVA_SYSTEM_H */

