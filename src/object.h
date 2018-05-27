#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "../lib/litelibs/litemath3d.h"
#include "types.h"

#define _BLEND_SOLID  0
#define _BLEND_ALPHA  1
#define _BLEND_ADD    2
#define _BLEND_MUL    3

#ifdef __cplusplus
extern "C"
{
#endif

/*int object_create(int object);*/
int object_createcube(int object);
int object_createtriangle(int object);
int object_load(int object, const char* filename);
int object_instantiate(int object, int from);
void object_delete(int object);
bool_t object_exists(int object);

void object_setname(int object, const char* name);
const char* object_name(int object);
void object_setvisible(int object, bool_t visible);
bool_t object_visible(int object);

void object_setposition(int object, float x, float y, float z);
float object_x(int object);
float object_y(int object);
float object_z(int object);
void object_setrotation(int object, float pitch, float yaw, float roll);
float object_pitch(int object);
float object_yaw(int object);
float object_roll(int object);
void object_setscale(int object, float x, float y, float z);
float object_scalex(int object);
float object_scaley(int object);
float object_scalez(int object);
void object_move(int object, float x, float y, float z);
void object_turn(int object, float pitch, float yaw, float roll);

int object_numbuffers(int object);
void object_setblend(int object, int buffer, int mode);
int object_blend(int object, int buffer);
void object_setimage(int object, int buffer, int image);
int object_image(int object, int buffer);
void object_setcolor(int object, int buffer, int color);
int object_color(int object, int buffer);
void object_setemissive(int object, int buffer, int color);
int object_emissive(int object, int buffer);
void object_setspecular(int object, int buffer, int color);
int object_specular(int object, int buffer);
void object_setshininess(int object, int buffer, float shininess);
float object_shininess(int object, int buffer);
void object_setculling(int object, int buffer, bool_t culling);
bool_t object_culling(int object, int buffer);
void object_setdepthwriting(int object, int buffer, bool_t depthwriting);
bool_t object_depthwriting(int object, int buffer);
void object_setlighting(int object, int buffer, bool_t lighting);
bool_t object_lighting(int object, int buffer);
void object_setfog(int object, int buffer, bool_t fog);
bool_t object_fog(int object, int buffer);

#ifndef SWIG
bool_t _object_bufferexists(int object, int buffer);
void _object_draw(int object, const lmat4_t* view_matrix);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OBJECT_H_INCLUDED */
