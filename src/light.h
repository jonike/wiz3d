#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "../lib/litelibs/litemath3d.h"
#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define _LIGHT_DIRECTIONAL 0
#define _LIGHT_POINT       1

int light_create(int light, int type);
void light_delete(int light);
bool_t light_exists(int light);
void light_setvisible(int light, bool_t visible);
bool_t light_visible(int light);
void light_setposition(int light, float x, float y, float z);
float light_x(int light);
float light_y(int light);
float light_z(int light);
void light_setrotation(int light, float pitch, float yaw);
float light_pitch(int light);
float light_yaw(int light);
void light_setcolor(int light, int color);
int light_color(int light);
void light_setattenuation(int light, float att);
float light_attenuation(int light);
void light_move(int light, float x, float y, float z);
void light_turn(int light, float pitch, float yaw, float roll);
void light_setambient(int color);
int light_ambient();

#ifndef SWIG
bool_t _light_prepare(int light, const lmat4_t* view_matrix);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LIGHT_H_INCLUDED */
