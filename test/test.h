/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef COLLECTC_TEST_H_
#define COLLECTC_TEST_H_

#include <stdlib.h>

#define t_assert(cond, ...)                                                                                            \
    do {                                                                                                               \
        if (!(cond)) {                                                                                                 \
            t_log(__func__, __LINE__, #cond, __VA_ARGS__);                                                             \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

void t_log(const char *func, int line, const char *expr, const char *format, ...);

#endif // COLLECTC_TEST_H_
