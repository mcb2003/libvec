#ifndef LIBVEC_VECTOR_H
#define LIBVEC_VECTOR_H
#include <stddef.h>

// A comparison function.
// Glibc defines this for you, but I thaught it would be more portible to just
// define it myself.
typedef int (*comparison_fn_t)(const void *, const void *);

struct vector {
  void *data;
  size_t nmem;
  size_t capacity;
  size_t itemsz;
};

int vec_empty(struct vector *vec, size_t itemsz);
int vec_prealloc(struct vector *vec, size_t capacity, size_t size);
void vec_free(struct vector *vec);

void *vec_get(const struct vector *vec, size_t index);

void *vec_push(struct vector *vec, const void *item);
void *vec_pop(struct vector *vec, void *item);

int vec_reserve(struct vector *vec, size_t capacity);
int vec_shrink(struct vector *vec);

void vec_sort(struct vector *vec, comparison_fn_t compare);
void *vec_find(const struct vector *vec, const void *key,
               comparison_fn_t compare);

#endif
