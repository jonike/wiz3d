#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct mesh_s;

struct mesh_s* mesh_create();
struct mesh_s* mesh_load(const char* filename, bool_t image_filtering);
void mesh_retain(struct mesh_s* mesh);
void mesh_release(struct mesh_s* mesh);
int mesh_addbuffer(struct mesh_s* mesh);
int mesh_addvertex(struct mesh_s* mesh, int buffer, float x, float y, float z, float nx, float ny, float nz, float u, float v);
int mesh_addtriangle(struct mesh_s* mesh, int buffer, int v0, int v1, int v2);
int mesh_numbuffers(struct mesh_s* mesh);
void mesh_setimage(struct mesh_s* mesh, int buffer, int image);
int mesh_image(struct mesh_s* mesh, int buffer);
void mesh_setcolor(struct mesh_s* mesh, int buffer, int color);
int mesh_color(struct mesh_s* mesh, int buffer);
void mesh_setblend(struct mesh_s* mesh, int buffer, int blend);
int mesh_blend(struct mesh_s* mesh, int buffer);
void mesh_setflags(struct mesh_s* mesh, int buffer, int flags);
int mesh_flags(struct mesh_s* mesh, int buffer);

#ifndef SWIG
void _mesh_draw(struct mesh_s* mesh);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MESH_H_INCLUDED */
