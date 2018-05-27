#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../lib/litelibs/litemath3d.h"
#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

int camera_create(int camera);
void camera_delete(int camera);
bool_t camera_exists(int camera);
void camera_setvisible(int camera, bool_t visible);
bool_t camera_visible(int camera);
void camera_setviewport(int camera, int x, int y, int width, int height);
int camera_viewportx(int camera);
int camera_viewporty(int camera);
int camera_viewportwidth(int camera);
int camera_viewportheight(int camera);
void camera_setclearcolor(int camera, int color);
int camera_clearcolor(int camera);
void camera_setortho(int camera, bool_t ortho);
bool_t camera_ortho(int camera);
void camera_setfov(int camera, float fov);
float camera_fov(int camera);
void camera_setmindistance(int camera, float distance);
void camera_setmaxdistance(int camera, float distance);
float camera_mindistance(int camera);
float camera_maxdistance(int camera);
void camera_setposition(int camera, float x, float y, float z);
float camera_x(int camera);
float camera_y(int camera);
float camera_z(int camera);
void camera_setrotation(int camera, float pitch, float yaw, float roll);
float camera_pitch(int camera);
float camera_yaw(int camera);
float camera_roll(int camera);
void camera_move(int camera, float x, float y, float z);
void camera_turn(int camera, float pitch, float yaw, float roll);

#ifndef SWIG
lmat4_t _camera_prepare(int camera);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CAMERA_H_INCLUDED */
