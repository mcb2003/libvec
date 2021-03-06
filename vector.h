#ifndef LIBVEC_VECTOR_H
#define LIBVEC_VECTOR_H

#include <stddef.h>
#include <sys/types.h>

// A comparison function for sorting and searching.
// Glibc defines this for you, but it's more portible to just
// define it myself.
typedef int (*comparison_fn_t)(const void *, const void *);

struct vector {
  void *data;
  size_t nmem;
  size_t capacity;
  size_t itemsz;
};

void vec_empty(struct vector *vec, size_t itemsz);
ssize_t vec_prealloc(struct vector *vec, size_t capacity, size_t itemsz);
void vec_free(struct vector *vec);

void *vec_get(const struct vector *vec, size_t index);
void *vec_get_unchecked(const struct vector *vec, size_t index);

void *vec_push(struct vector *restrict vec, const void *restrict item);
void *vec_pop(struct vector *restrict vec, void *restrict item);

ssize_t vec_realloc(struct vector *vec, size_t capacity);
ssize_t vec_reserve(struct vector *vec, size_t capacity);
ssize_t vec_truncate(struct vector *vec, size_t capacity);
ssize_t vec_shrink(struct vector *vec);

void vec_qsort(struct vector *vec, comparison_fn_t compare);
void *vec_lfind(const struct vector *restrict vec, const void *restrict key,
                comparison_fn_t compare);
void *vec_lsearch(struct vector *restrict vec, const void *restrict key,
                  comparison_fn_t compare);
void *vec_bsearch(const struct vector *restrict vec, const void *restrict key,
                  comparison_fn_t compare);

void *vec_swap_remove(struct vector *restrict vec, size_t index,
                      void *restrict item);

#endif
