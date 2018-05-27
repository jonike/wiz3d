#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

int image_create(int image, int width, int height);
int image_load(int image, const char* filename);
void image_delete(int image);
bool_t image_exists(int image);
int image_width(int image);
int image_height(int image);
void image_setfilter(bool_t filter);

#ifndef SWIG
const void* _image_ptr(int image);;
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IMAGE_H_INCLUDED */
