/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <collectc.h>

#include "test.h"

void test_vector_mutation(void) {
    vector_t vec = vector_new(10, sizeof(int));
    t_assert(vector_is_empty(vec), "got %zu", vector_len(vec));

    int elements[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector_push(&vec, elements, 9);
    t_assert(vector_len(vec) == 9, "got %zu", vector_len(vec));

    {
        vector_remove(vec, 4, 3);
        t_assert(vector_len(vec) == 6, "got %zu", vector_len(vec));
        int expected[6] = {1, 2, 3, 4, 8, 9};
        for (size_t i = 0; i < vector_len(vec); i++) {
            const int *element = vector_at(vec, i);
            t_assert(*element == expected[i], "at %zu: got %d; want %d", i, *element, expected[i]);
        }
    }

    {
        int newElement = 10;
        vector_push(&vec, &newElement, 1);
        t_assert(vector_len(vec) == 7, "got %zu", vector_len(vec));
        int expected[7] = {1, 2, 3, 4, 8, 9, 10};
        for (size_t i = 0; i < vector_len(vec); i++) {
            const int *element = vector_at(vec, i);
            t_assert(*element == expected[i], "at %zu: got %d; want %d", i, *element, expected[i]);
        }
    }

    {
        int newElements[3] = {5, 6, 7};
        vector_insert(&vec, 4, newElements, 3);
        t_assert(vector_len(vec) == 10, "got %zu", vector_len(vec));
        int expected[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        for (size_t i = 0; i < vector_len(vec); i++) {
            const int *element = vector_at(vec, i);
            t_assert(*element == expected[i], "at %zu: got %d; want %d", i, *element, expected[i]);
        }
    }

    {
        vector_t other = vector_new(0, sizeof(int));
        for (int i = 11; i <= 14; i++) {
            vector_push(&other, &i, 1);
        }
        t_assert(vector_len(other) == 4, "got %zu", vector_len(other));
        vector_extend(&vec, other);
        t_assert(vector_len(vec) == 14, "got %zu", vector_len(vec));
        int expected[14] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
        for (size_t i = 0; i < vector_len(vec); i++) {
            const int *element = vector_at(vec, i);
            t_assert(*element == expected[i], "at %zu: got %d; want %d", i, *element, expected[i]);
        }
        vector_delete(other);
    }

    vector_delete(vec);
}

void test_vector_slice(void) {
    vector_t vec = vector_new(8, sizeof(int));
    t_assert(vector_is_empty(vec), "got %zu", vector_len(vec));

    int elements[6] = {1, 2, 3, 4, 5, 6};
    vector_push(&vec, elements, 6);

    {
        int expected[6] = {1, 2, 3, 4, 5, 6};
        int actual[6];
        vector_slice(vec, 0, actual, 6);
        for (size_t i = 0; i < 6; i++) {
            t_assert(actual[i] == expected[i], "at %zu: got %d; want %d", i, actual[i], expected[i]);
        }
    }

    {
        int expected[3] = {3, 4, 5};
        int actual[3];
        vector_slice(vec, 2, actual, 3);
        for (size_t i = 0; i < 3; i++) {
            t_assert(actual[i] == expected[i], "at %zu: got %d; want %d", i, actual[i], expected[i]);
        }
    }

    {
        int actual;
        vector_slice(vec, 5, &actual, 1);
        t_assert(actual == 6, "got %d", actual);
    }
}

void test_vector_iteration(void) {
    vector_t vec = vector_new(0, sizeof(int));
    int elements[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector_push(&vec, elements, 9);

    {
        const int *actual = vector_first(vec);
        int expected[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (size_t i = 0; i < vector_len(vec); i++) {
            t_assert(actual[i] == expected[i], "at %zu: got %d; want %d", i, actual[i], expected[i]);
        }
    }

    {
        vector_t outputs = vector_new(0, sizeof(int));
        for (const int *output = vector_first(vec), *last = vector_last(vec); output <= last; output++) {
            vector_push(&outputs, output, 1);
        }
        t_assert(vector_len(outputs) == 9, "got %zu", vector_len(outputs));
        int expected[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (size_t i = 0; i < 9; i++) {
            const int *actual = vector_at(outputs, i);
            t_assert(*actual == expected[i], "at %zu: got %d; want %d", i, *actual, expected[i]);
        }
        vector_delete(outputs);
    }

    {
        vector_t outputs = vector_new(0, sizeof(int));
        for (const int *output = vector_last(vec), *first = vector_first(vec); output >= first; output--) {
            vector_push(&outputs, output, 1);
        }
        t_assert(vector_len(outputs) == 9, "got %zu", vector_len(outputs));
        int expected[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
        for (size_t i = 0; i < 9; i++) {
            const int *actual = vector_at(outputs, i);
            t_assert(*actual == expected[i], "at %zu: got %d; want %d", i, *actual, expected[i]);
        }
        vector_delete(outputs);
    }

    vector_delete(vec);
}

void test_vector_nops(void) {
    vector_t vec = vector_new(0, sizeof(int));
    t_assert(vector_is_empty(vec), "got %zu", vector_len(vec));

    int elements[3] = {1, 2, 3};
    vector_push(&vec, elements, 3);

    vector_slice(vec, 0, NULL, 0);
    vector_insert(&vec, 1, NULL, 0);
    vector_push(&vec, NULL, 0);
    vector_remove(vec, 3, 0);

    vector_delete(vec);
}
