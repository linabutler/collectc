/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef COLLECTC_VECTOR_H_
#define COLLECTC_VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief A contiguous growable array.
 *
 * Vectors dynamically allocate space for their elements on the heap,
 * and support O(1) indexing and amortized O(1) pushing.
 *
 * The length of a vector is the number of elements that it currently holds.
 * The capacity is the maximum number of elements that the vector can hold
 * before reallocating.
 *
 * Vectors assume that their elements have the same size and type. Inserting
 * an element of a different size, or accessing an element through a pointer
 * to a different type, is undefined.
 *
 * Be careful when holding long-lived pointers to a vector or its elements.
 * Removing elements from a vector can invalidate existing pointers to
 * any elements, not just the ones that were removed. Inserting new elements
 * can invalidate all pointers to both the vector and any elements, if
 * the vector needs to reallocate to hold the new elements.
 *
 * Vectors are not internally synchronized (in the thread-safety sense).
 * If multiple threads access and modify the same vector concurrently,
 * they must synchronize those operations; for example, by using a lock.
 *
 * @class vector_t collectc/vector.h
 */
typedef uintptr_t vector_t;

/**
 * @brief Creates a new, empty vector.
 *
 * Aborts on memory allocation failure.
 *
 * @param[in] initialCapacity The starting capacity of the vector. The vector
 * will be able to hold this number of elements before reallocating.
 * If zero, the vector won't allocate until it's modified.
 * @param[in] elementSize The size of each element.
 * @return The new vector.
 *
 * @memberof vector_t
 * @static
 */
vector_t vector_new(size_t initialCapacity, size_t elementSize);

/**
 * @return The number of elements in the vector.
 *
 * @memberof vector_t
 */
size_t vector_len(const vector_t vec);

/**
 * @return The number of elements that the vector can hold
 * without reallocating.
 *
 * @memberof vector_t
 */
size_t vector_capacity(const vector_t vec);

/**
 * @return The size of each element.
 *
 * @memberof vector_t
 */
size_t vector_element_size(const vector_t vec);

/**
 * @return `true` if the vector is empty.
 *
 * @memberof vector_t
 */
bool vector_is_empty(const vector_t vec);

/**
 * Returns the first element of the vector.
 *
 * This operation is O(1).
 *
 * @param[in] vec The vector.
 *
 * @return A pointer to the first element, or
 * `null` if the vector is empty.
 *
 * @memberof vector_t
 */
const void *vector_first(vector_t vec);

/**
 * Returns a pointer to an element in the vector.
 *
 * This operation is O(1).
 *
 * @param[in] vec The vector.
 * @param[in] index The zero-based index of the element.
 *
 * @return A constant pointer to the element at the index, or
 * `null` if the index is out-of-bounds.
 *
 * @memberof vector_t
 */
const void *vector_at(const vector_t vec, size_t index);

/**
 * Returns a mutable pointer to an element in the vector.
 *
 * This operation is O(1).
 *
 * @param[in] vec The vector.
 * @param[in] index The zero-based index of the element.
 *
 * @return A mutable pointer to the element at the index, or
 * `null` if the index is out-of-bounds.
 *
 * @memberof vector_t
 */
void *vector_at_mut(vector_t vec, size_t index);

/**
 * Returns the last element of the vector.
 *
 * This operation is O(1).
 *
 * @param[in] vec The vector.
 *
 * @return A pointer to the last element, or
 * `null` if the vector is empty.
 *
 * @memberof vector_t
 */
const void *vector_last(const vector_t vec);

/**
 * Reserves capacity for at least the given number of elements,
 * such that the vector will be able to hold
 * `vector_len(vec) + extraCapacity` elements before reallocating.
 *
 * If the vector reallocates, the value of the pointer to the vector will
 * change, and any existing pointers to the vector or its elements will
 * become invalid.
 *
 * Reserving is O(capacity) if the vector needs to reallocate.
 *
 * @param[inout] vec A pointer to the vector.
 * @param[in] extraCapacity Additional capacity to reserve.
 *
 * @memberof vector_t
 */
void vector_reserve(vector_t *vec, size_t extraCapacity);

/**
 * Inserts elements into the vector, shifting all following
 * elements to the right.
 *
 * Inserting an element at the beginning of the vector has a
 * worst-case complexity of O(n).
 *
 * @param[in] vec A pointer to the vector.
 * @param[in] index The zero-based index at which to insert the new elements.
 * @param[in] elements A pointer to the first element. The pointed-to elements
 * must have the same size as the vector's element size.
 * @param[in] count The number of elements.
 *
 * @memberof vector_t
 */
void vector_insert(vector_t *vec, size_t index, const void *elements, size_t count);

/**
 * Appends elements to the vector.
 *
 * Pushing an element always modifies the pointed-to vector, and also
 * modifies the value of the pointer if the vector reallocates.
 *
 * Pushing is an amortized O(1) operation: O(1) if the vector
 * has enough capacity to hold the new elements, or O(capacity)
 * if the vector needs to reallocate to hold the elements.
 *
 * Aborts on memory allocation failure.
 *
 * @param[inout] vec A pointer to the vector.
 * @param[in] elements A pointer to the first element. The pointed-to
 * elements must have the same size as the vector's element size.
 * @param[in] count The number of elements.
 *
 * @memberof vector_t
 */
void vector_push(vector_t *vec, const void *elements, size_t count);

/**
 * Copies elements from the vector.
 *
 * If the pointed-to memory for the slice doesn't have enough space to
 * hold the copied elements, the behavior is undefined.
 *
 * Aborts if the range `[index, index + count]` is
 * out-of-bounds.
 *
 * @param[in] vec The vector.
 * @param[in] index The zero-based index at which to begin
 * copying elements from the vector.
 * @param[out] slice A pointer to the memory that will
 * hold the copied elements.
 * @param[in] count The number of elements to copy.
 *
 * @memberof vector_t
 */
void vector_slice(const vector_t vec, size_t index, void *slice, size_t count);

/**
 * Appends the contents of another vector to this vector.
 *
 * Extending modifies this vector in the same way as pushing.
 *
 * Extending is O(n) with respect to the other vector, and
 * O(capacity + n) if this vector needs to reallocate.
 *
 * Aborts on memory allocation failure, or if the size of
 * this vector's elements doesn't match the size of the
 * other vector's elements.
 *
 * @param[inout] vec A pointer to this vector.
 * @param[in] other The other vector.
 *
 * @memberof vector_t
 */
void vector_extend(vector_t *vec, const vector_t other);

/**
 * Removes elements from the vector, shifting all following
 * elements to the left.
 *
 * Removing an element from the beginning of the vector has a
 * worst-case complexity of O(n).
 *
 * Aborts if the range `[index, index + count]` is
 * out-of-bounds.
 *
 * @param[in] vec The vector.
 * @param[in] index The zero-based index of the first element to remove.
 * @param[in] count The number of elements to remove, including the
 * first element.
 *
 * @memberof vector_t
 */
void vector_remove(vector_t vec, size_t index, size_t count);

/**
 * Removes all elements from the vector.
 *
 * Clearing a vector won't shrink its capacity, but will
 * invalidate any existing pointers to the vector's elements.
 *
 * This operation is O(1).
 *
 * @param[in] vec The vector.
 *
 * @memberof vector_t
 */
void vector_clear(vector_t vec);

/**
 * Destroys the vector, freeing any memory allocated for it.
 *
 * This invalidates any existing pointers to the vector and its elements.
 * Inserting or accessing elements of a destroyed vector is undefined.
 *
 * @param[in] vec The vector.
 *
 * @memberof vector_t
 */
void vector_delete(vector_t vec);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COLLECTC_VECTOR_H_
