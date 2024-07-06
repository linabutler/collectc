/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "test.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void t_log(const char *func, int line, const char *expr, const char *format, ...) {
    char *message = NULL;

    va_list args, argsCopy;
    va_start(args, format);
    va_copy(argsCopy, args);

    int length = vsnprintf(NULL, 0, format, args);
    if (length < 0) {
        goto error;
    }

    message = malloc(length + 1);
    if (message == NULL) {
        goto error;
    }
    if (vsnprintf(message, length + 1, format, argsCopy) != length) {
        goto error;
    }
    fprintf(stderr, "[%s:%d] (%s): %s\n", func, line, expr, message);
    goto end;

error:
    fprintf(stderr, "[%s:%d]: %s\n", func, line, expr);

end:
    va_end(argsCopy);
    va_end(args);
    fflush(stderr);
    free(message);
}
