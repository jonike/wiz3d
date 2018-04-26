#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct font_s;

struct font_s* font_load(const char* filename, float height);
void font_retain(struct font_s* font);
void font_release(struct font_s* font);
float font_height(struct font_s* font);
float font_textwidth(struct font_s* font, const char* text);
float font_textheight(struct font_s* font, const char* text);
void font_draw(struct font_s* font, const char* text, float x, float y);

#ifndef SWIG
struct font_s* _font_loadbase64(const char* data, size_t size, float height);
struct font_s* _font_loadfrommemory(const unsigned char* data, float height);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FONT_H_INCLUDED */
