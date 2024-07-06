/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <collectc.h>

#include <limits.h>
#include <stdlib.h>
#include <string.h>

/**
 * The maximum element size that can be encoded in the higher bits of a
 * tagged "pointer" for a zero-capacity vector.
 */
static const size_t MAX_ZERO_CAPACITY_ELEMENT_SIZE = (size_t)1 << (sizeof(vector_t) * CHAR_BIT - 1);

/** The allocation header for an above-zero-capacity vector. */
typedef struct vector_header {
    size_t capacity;
    size_t length;
    size_t elementSize;
} vector_header_t;

_Static_assert(
    // Dynamically allocated memory addresses must be aligned such that
    // the least significant bit is never set. This lets us distinguish
    // between real pointers returned by `malloc`, and tagged "pointers"
    // that encode a zero-capacity vector's element size in the higher bits.
    (_Alignof(max_align_t) & 1) == 0,
    "Can't use tagged pointers for zero-capacity vectors"
);

/** Recovers and returns a pointer to the base address of a vector. */
static inline void *vector_base(vector_t vec) {
    bool isZeroCapacity = (vec & 1) != 0;
    return isZeroCapacity ? NULL : (void *)vec;
}

/**
 * Returns a pointer to the element at the given index,
 * without bounds checking. The vector's capacity must be
 * above zero.
 */
static inline void *vector_at_unchecked(vector_t vec, size_t index) {
    char *base = vector_base(vec);
    vector_header_t *header = (vector_header_t *)base;
    return base + sizeof(*header) + (index * header->elementSize);
}

vector_t vector_new(size_t initialCapacity, size_t elementSize) {
    bool isZeroCapacity = initialCapacity == 0 && elementSize <= MAX_ZERO_CAPACITY_ELEMENT_SIZE;
    if (isZeroCapacity) {
        return ((vector_t)elementSize << 1) | 1;
    }
    void *base = malloc(sizeof(vector_header_t) + (initialCapacity * elementSize));
    if (base == NULL) {
        abort();
    }
    vector_header_t *header = base;
    header->capacity = initialCapacity;
    header->length = 0;
    header->elementSize = elementSize;
    return (vector_t)header;
}

size_t vector_len(vector_t vec) {
    vector_header_t *header = vector_base(vec);
    return header == NULL ? 0 : header->length;
}

size_t vector_capacity(vector_t vec) {
    vector_header_t *header = vector_base(vec);
    return header == NULL ? 0 : header->capacity;
}

size_t vector_element_size(vector_t vec) {
    vector_header_t *header = vector_base(vec);
    return header == NULL ? vec >> 1 : header->elementSize;
}

bool vector_is_empty(vector_t vec) {
    return vector_len(vec) == 0;
}

void vector_reserve(vector_t *vec, size_t extraCapacity) {
    size_t length = vector_len(*vec);
    size_t oldCapacity = vector_capacity(*vec);
    if (length + extraCapacity <= oldCapacity) {
        return;
    }
    size_t elementSize = vector_element_size(*vec);
    size_t newCapacity = oldCapacity + (oldCapacity / 2 * 3) + extraCapacity;
    void *oldBase = vector_base(*vec);
    void *newBase = realloc(oldBase, sizeof(vector_header_t) + (newCapacity * elementSize));
    if (newBase == NULL) {
        abort();
    }
    vector_header_t *newHeader = newBase;
    newHeader->length = length;
    newHeader->capacity = newCapacity;
    newHeader->elementSize = elementSize;
    *vec = (vector_t)newHeader;
}

void vector_insert(vector_t *vec, size_t index, const void *elements, size_t count) {
    vector_reserve(vec, count);
    vector_header_t *header = vector_base(*vec);
    if (header == NULL || index > header->length) {
        abort();
    }
    void *at = vector_at_unchecked(*vec, index);
    if (index < header->length) {
        void *to = vector_at_unchecked(*vec, index + count);
        memmove(to, at, (header->length - 1 - count) * header->elementSize);
    }
    memcpy(at, elements, count * header->elementSize);
    header->length += count;
}

void vector_push(vector_t *vec, const void *elements, size_t count) {
    size_t length = vector_len(*vec);
    vector_insert(vec, length, elements, count);
}

void vector_slice(vector_t vec, size_t index, void *slice, size_t count) {
    vector_header_t *header = vector_base(vec);
    if (header == NULL || index + count > header->length) {
        abort();
    }
    void *from = vector_at_unchecked(vec, index);
    memcpy(slice, from, count * header->elementSize);
}

void vector_extend(vector_t *vec, vector_t other) {
    if (vector_element_size(*vec) != vector_element_size(other)) {
        abort();
    }
    char *otherBase = vector_base(other);
    if (otherBase != NULL) {
        vector_header_t *otherHeader = (vector_header_t *)otherBase;
        vector_push(vec, otherBase + sizeof(*otherHeader), otherHeader->length);
    }
}

void vector_remove(vector_t vec, size_t index, size_t count) {
    vector_header_t *header = vector_base(vec);
    if (header == NULL || index + count > vector_len(vec)) {
        abort();
    }
    void *from = vector_at_unchecked(vec, index + count);
    void *to = vector_at_unchecked(vec, index);
    memmove(to, from, (header->length - index - count) * header->elementSize);
    header->length -= count;
}

const void *vector_first(const vector_t vec) {
    return vector_at(vec, 0);
}

const void *vector_at(const vector_t vec, size_t index) {
    return vector_at_mut(vec, index);
}

void *vector_at_mut(vector_t vec, size_t index) {
    return index < vector_len(vec) ? vector_at_unchecked(vec, index) : NULL;
}

const void *vector_last(const vector_t vec) {
    size_t length = vector_len(vec);
    return length > 0 ? vector_at_unchecked(vec, length - 1) : NULL;
}

void vector_clear(vector_t vec) {
    vector_header_t *header = vector_base(vec);
    if (header != NULL) {
        header->length = 0;
    }
}

void vector_delete(vector_t vec) {
    free(vector_base(vec));
}
