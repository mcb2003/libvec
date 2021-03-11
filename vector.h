#ifndef LIBVEC_VECTOR_H
#define LIBVEC_VECTOR_H
#include <stddef.h>

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

#endif
