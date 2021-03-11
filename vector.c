#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

int vec_empty(struct vector *vec, size_t itemsz) {
  if (!vec) {
    errno = EINVAL;
    return -1;
  }

  vec->data = NULL;
  vec->nmem = 0;
  vec->capacity = 0;
  vec->itemsz = itemsz;
  return 0;
}

// Helper function that finds the next power of 2
size_t next_pow2(size_t n) {
  // Half of the total bits
  size_t bits = sizeof(size_t) * 4;
  --n;
  for (size_t shift = 1; shift <= bits; shift *= 2)
    n |= n >> shift;
  ++n;
  return n;
}

int vec_prealloc(struct vector *vec, size_t capacity, size_t itemsz) {
  if (!vec) {
    errno = EINVAL;
    return -1;
  }
  if (capacity == 0)
    return vec_empty(vec, itemsz);

  // Round up allocations to powers of 2 for reallocation efficiency
  capacity = next_pow2(capacity);

  if (!(vec->data = calloc(capacity, itemsz)))
    return -1;
  vec->nmem = 0;
  vec->capacity = capacity;
  vec->itemsz = itemsz;
  return capacity;
}

void vec_free(struct vector *vec) {
  if (!vec)
    return;

  free(vec->data);
  vec->data = NULL;
  vec->nmem = 0;
  vec->capacity = 0;
}

void *vec_get(const struct vector *vec, size_t index) {
  if (index >= vec->nmem)
    return NULL;
  return vec->data + (vec->itemsz * index);
}

void *vec_push(struct vector *vec, const void *item) {
  if (vec->nmem + 1 > vec->capacity) {
    if (vec->capacity == 0)
      vec->capacity = 1;
    vec->capacity *= 2;
    if (!(vec->data = reallocarray(vec->data, vec->capacity, vec->itemsz)))
      return NULL;
  }
  void *pitem = vec->data + ((vec->nmem++) * vec->itemsz);
  memcpy(pitem, item, vec->itemsz);
  return pitem;
}