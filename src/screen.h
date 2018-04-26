#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void screen_set(int width, int height, bool_t fullscreen, bool_t resizable);
void screen_sync();
void screen_settitle(const char* title);
void screen_setdrawcolor(int color);
void screen_setdrawfont(const char* filename, float height);
void screen_setdrawfontdefault();
void screen_drawtext(const char* text, float x, float y);
void screen_print(const char* text);
/*const char* screen_input(const char* prompt);*/
void screen_clearprints();
int screen_width();
int screen_height();
float screen_delta();
int screen_fps();
bool_t screen_opened();

int screen_desktopwidth();
int screen_desktopheight();

float screen_textwidth(const char* text);
float screen_textheight(const char* text);

#ifndef SWIG
void* _screen_pointer();
bool_t _screen_synced();
void _screen_setlighting(); /* setup lighting based on number of lights in the scene */
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCREEN_H_INCLUDED */
