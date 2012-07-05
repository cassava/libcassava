/*
 * debug.h
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

/**
 * \file
 *
 * This file defines macros similar to \e assert.h that assist in
 * running debugging code and printing statements.
 *
 * If the macro \p NDEBUG is \e not defined, then the three following
 * \b macros are enabled:
 *
 *   \li debug(...)
 *   \li debug_puts(str)
 *   \li debug_printf(...)
 *
 * \warning The macros are \e not identical to their implied counterparts,
 * for example debug_puts() and debug_printf() \e require the first
 * argument to be a character literal (as the preprocessor is used to
 * concatenate several character literals).
 *
 * \example
 * \code
 *     ...
 *     char *str = get_my_string();
 *     debug_puts(str);            // error
 *     debug_printf("%s\n", str);  // correct
 *     ...
 *     debug(puts(str));           // also correct
 *     ...
 * \endcode
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG

#include <stdio.h>

#define STRINGIFY_LEVEL1_(str) #str
#define STRINGIFY_LEVEL0_(str) STRINGIFY_LEVEL1_(str)
#define DEBUG_FILENO_ __FILE__ " (" STRINGIFY_LEVEL0_(__LINE__) "): "

#define debug(...) __VA_ARGS__
#define debug_puts(str) puts(DEBUG_FILENO_ str)
#define debug_printf(...) fprintf(stderr, DEBUG_FILENO_ __VA_ARGS__)

#else /* NDEBUG defined */

#define VOID_POINTER_CAST_ (void) (0)

#define debug(...) VOID_POINTER_CAST_
#define debug_puts(str) VOID_POINTER_CAST_
#define debug_printf(...) VOID_POINTER_CAST_

#endif /* NDEBUG */

#endif /* DEBUG_H */
