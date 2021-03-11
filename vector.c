#include <assert.h>
#include <errno.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

int vec_empty(struct vector *vec, size_t itemsz) {
  assert(vec);

  vec->data = NULL;
  vec->nmem = 0;
  vec->capacity = 0;
  vec->itemsz = itemsz;
  return 0;
}

// Helper function that finds the next power of 2
static size_t next_pow2(size_t n) {
  // Half of the total bits
  size_t bits = sizeof(size_t) * 4;
  --n;
  for (size_t shift = 1; shift <= bits; shift *= 2)
    n |= n >> shift;
  ++n;
  return n;
}

int vec_prealloc(struct vector *vec, size_t capacity, size_t itemsz) {
  assert(vec);

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
  assert(vec);

  if (index >= vec->nmem)
    return NULL;
  return vec->data + (vec->itemsz * index);
}

void *vec_push(struct vector *vec, const void *item) {
  assert(vec);
  assert(item);

  if (vec->nmem + 1 > vec->capacity) {
    if (vec->capacity == 0)
      vec->capacity = 1;
    // Calculate the new capacity
    size_t old = vec->capacity;
    vec->capacity = next_pow2(vec->capacity);
    if (vec->capacity == old)
      vec->capacity *= 2;

    if (!(vec->data = reallocarray(vec->data, vec->capacity, vec->itemsz)))
      return NULL;
  }
  void *pitem = vec->data + ((vec->nmem++) * vec->itemsz);
  memcpy(pitem, item, vec->itemsz);
  return pitem;
}

void *vec_pop(struct vector *vec, void *item) {
  assert(vec);

  if (vec->nmem == 0)
    return NULL;

  --(vec->nmem);
  if (item) {
    void *pitem = vec->data + ((vec->nmem) * vec->itemsz);
    memcpy(item, pitem, vec->itemsz);
  }
  return item;
}

int vec_reserve(struct vector *vec, size_t capacity) {
  assert(vec);

  if (capacity <= vec->capacity)
    // Already larger than necessary
    return vec->capacity;

  void *data = reallocarray(vec->data, capacity, vec->itemsz);
  if (!data)
    return -1;

  vec->data = data;
  vec->capacity = capacity;
  return capacity;
}

int vec_truncate(struct vector *vec, size_t capacity) {
  assert(vec);

  if (capacity >= vec->capacity)
    // Already smaller than necessary
    return vec->capacity;

  void *data = reallocarray(vec->data, capacity, vec->itemsz);
  if (!data)
    return -1;

  vec->data = data;
  vec->capacity = capacity;
  return capacity;
}

int vec_shrink(struct vector *vec) {
  assert(vec);

  void *data = reallocarray(vec->data, vec->nmem, vec->itemsz);
  if (!data)
    return -1;

  vec->data = data;
  vec->capacity = vec->nmem;
  return 0;
}

inline void vec_qsort(struct vector *vec, comparison_fn_t compare) {
  assert(vec);

  qsort(vec->data, vec->nmem, vec->itemsz, compare);
}

inline void *vec_lfind(const struct vector *vec, const void *key,
                       comparison_fn_t compare) {
  assert(vec);

  size_t nmem = vec->nmem;
  return lfind(key, vec->data, &nmem, vec->itemsz, compare);
}

inline void *vec_lsearch(struct vector *vec, const void *key,
                         comparison_fn_t compare) {
  assert(vec);

  if (vec_reserve(vec, vec->nmem + 1) < 0)
    // Reallocarray failed
    return NULL;

  return lsearch(key, vec->data, &vec->nmem, vec->itemsz, compare);
}

inline void *vec_bsearch(const struct vector *vec, const void *key,
                         comparison_fn_t compare) {
  assert(vec);

  return bsearch(key, vec->data, vec->nmem, vec->itemsz, compare);
}
