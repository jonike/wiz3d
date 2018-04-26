#include "../lib/litelibs/litegfx.h"
#include "../lib/litelibs/litemath3d.h"
#include "../lib/stretchy_buffer.h"
#include "color.h"
#include "image.h"
#include "mesh.h"
#include "screen.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

struct material_s
{
  int image;
  int color;
  int blend;
  int flags;
};

struct buffer_s
{
  struct material_s material;
  lvert_t*          vertices;
  unsigned short*   indices;
};

struct mesh_s
{
  size_t            refcount;
  struct buffer_s*  buffers;
};

#pragma pack(push, 1)
struct mshmaterial_s
{
  int color;
  int specular;
  int emissive;
  int ambient;
  unsigned char blendmode;
  unsigned char flags;
  float specpower;
  float cubealpha;
  float refrcoef;
  unsigned char usedtexs;
};
#pragma pack(pop)


struct mesh_s* mesh_create()
{
  struct mesh_s* mesh;
  
  mesh = (struct mesh_s*)malloc(sizeof(struct mesh_s));
  mesh->refcount = 1;
  mesh->buffers = NULL;
  return mesh;
}

struct mesh_s* mesh_load(const char* filename, bool_t image_filtering)
{
  FILE* f;
  size_t filenamelen;
  char* path;
  struct mesh_s* mesh;
  char id[5];
  unsigned short numsurfs;
  int i, j;
  int len;

  /* open file */
  f = fopen(filename, "rb");
  if ( !f ) return NULL;

  /* read id */
  id[4] = 0;
  fread(id, sizeof(char), 4, f);
  if ( strcmp(id, "ME01") != 0 ) return NULL;

  /* store path */
  filenamelen = strlen(filename);
  path = (char*)malloc(filenamelen+1);
  dir_extract(filename, path, filenamelen+1);
  filenamelen = strlen(path);

  /* create mesh */
  mesh = mesh_create();

  /* read surfaces */
  fread(&numsurfs, sizeof(numsurfs), 1, f);
  for ( i = 0; i < numsurfs; ++i )
  {
    struct mshmaterial_s mat;
    int image = -1;
    int flags = 0;
    int numindices;
    unsigned short numvertices;
    unsigned char vertexflags;
    int buffer;

    /* read material */
    fread(&mat, sizeof(mat), 1, f);

    /* read textures */
    if ( mat.usedtexs & 1 ) /* color texture */
    {
      char* str;

      fread(&len, sizeof(len), 1, f);
      str = (char*)malloc(filenamelen+len+1);
      str[filenamelen+len] = 0;
      strcpy(str, path);
      fread(str + filenamelen, sizeof(char), len, f);
      image = image_load(-1, str, image_filtering);
      free(str);
    }
    else if ( mat.usedtexs & 2 ) /* normal tex */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }
    else if ( mat.usedtexs & 4 ) /* specular tex */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }
    else if ( mat.usedtexs & 8 ) /* emissive tex */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }
    else if ( mat.usedtexs & 16 ) /* ambient tex */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }
    else if ( mat.usedtexs & 32 ) /* lightmap */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }
    else if ( mat.usedtexs & 64 ) /* cubemap */
    {
      fread(&len, sizeof(len), 1, f);
      fseek(f, len, SEEK_CUR);
    }

    /* read number of indices, number of vertices and vertex flags */
    fread(&numindices, sizeof(numindices), 1, f);
    fread(&numvertices, sizeof(numvertices), 1, f);
    fread(&vertexflags, sizeof(vertexflags), 1, f);

    /* create buffer */
    buffer = mesh_addbuffer(mesh);
    mesh_setimage(mesh, buffer, image);
    mesh_setcolor(mesh, buffer, mat.color);
    mesh_setblend(mesh, buffer, mat.blendmode);
    mesh_setflags(mesh, buffer, mat.flags);

    /* read indices */
    for ( j = 0; j < numindices; j += 3 )
    {
      unsigned short indices[3];
      fread(indices, sizeof(indices), 1, f);
      mesh_addtriangle(mesh, buffer, indices[0], indices[1], indices[2]);
    }

    /* read vertices */
    for ( j = 0; j < numvertices; ++j )
    {
      lvec3_t position;
      lvec3_t normal;
      lvec3_t tangent;
      int color = _COLOR_WHITE;
      float tex0[2] = { 0, 0 };
      float tex1[2];
      int bones[4];
      float weights[4];

      fread(&position, sizeof(position), 1, f);
      if ( vertexflags & 1 ) fread(&normal, sizeof(normal), 1, f);
      else normal = lvec3(0, 0, -1);
      if ( vertexflags & 2 ) fread(&tangent, sizeof(tangent), 1, f);
      if ( vertexflags & 4 ) fread(&color, sizeof(color), 1, f);
      else color = _COLOR_WHITE;
      if ( vertexflags & 8 ) fread(tex0, sizeof(tex0), 1, f);
      if ( vertexflags & 16) fread(tex1, sizeof(tex1), 1, f);
      if ( vertexflags & 32 )
      {
        fread(bones, sizeof(bones), 1, f);
        fread(weights, sizeof(weights), 1, f);
      }
      mesh_addvertex(mesh, buffer, position.x, position.y, position.z, normal.x, normal.y, normal.z, tex0[0], tex0[1]);
    }
  }

  /* free temp resources */
  free(path);
  fclose(f);

  return mesh;
}

void mesh_retain(struct mesh_s* mesh)
{
  ++mesh->refcount;
}

void mesh_release(struct mesh_s* mesh)
{
  int i;

  if ( --mesh->refcount == 0 ) {
    for ( i = 0; i < sb_count(mesh->buffers); ++i )
    {
      sb_free(mesh->buffers[i].vertices);
      sb_free(mesh->buffers[i].indices);
    }

    sb_free(mesh->buffers);
    free(mesh);
  }
}

int mesh_addbuffer(struct mesh_s* mesh)
{
  struct buffer_s* buffer;
  
  buffer = sb_add(mesh->buffers, 1);
  buffer->material.image = -1;
  buffer->material.color = _COLOR_WHITE;
  buffer->material.blend = 0;
  buffer->material.flags = 3;
  buffer->vertices = NULL;
  buffer->indices = NULL;

  return sb_count(mesh->buffers) - 1;
}

int mesh_addvertex(struct mesh_s* mesh, int buffer, float x, float y, float z, float nx, float ny, float nz, float u, float v)
{
  sb_push(mesh->buffers[buffer].vertices, lvert(x, y, z, nx, ny, nz, u, v, 1, 1, 1, 1));
  return sb_count(mesh->buffers[buffer].vertices) - 1;
}

int mesh_addtriangle(struct mesh_s* mesh, int buffer, int v0, int v1, int v2)
{
  unsigned short* index;

  index = sb_add(mesh->buffers[buffer].indices, 3);
  index[0] = v0;
  index[1] = v1;
  index[2] = v2;
  return (sb_count(mesh->buffers[buffer].indices) - 3) / 3;
}

int mesh_numbuffers(struct mesh_s* mesh)
{
  return sb_count(mesh->buffers);
}

void mesh_setimage(struct mesh_s* mesh, int buffer, int image)
{
  mesh->buffers[buffer].material.image = image;
}

int mesh_image(struct mesh_s* mesh, int buffer)
{
  return mesh->buffers[buffer].material.image;
}

void mesh_setcolor(struct mesh_s* mesh, int buffer, int color)
{
  mesh->buffers[buffer].material.color = color;
}

int mesh_color(struct mesh_s* mesh, int buffer)
{
  return mesh->buffers[buffer].material.color;
}

void mesh_setblend(struct mesh_s* mesh, int buffer, int blend)
{
  mesh->buffers[buffer].material.blend = blend;
}

int mesh_blend(struct mesh_s* mesh, int buffer)
{
  return mesh->buffers[buffer].material.blend;
}

void mesh_setflags(struct mesh_s* mesh, int buffer, int flags)
{
  mesh->buffers[buffer].material.flags = flags;
}

int mesh_flags(struct mesh_s* mesh, int buffer)
{
  return mesh->buffers[buffer].material.flags;
}

void _mesh_draw(struct mesh_s* mesh)
{
  int i;

  for ( i = 0; i < sb_count(mesh->buffers); ++i )
  {
    lgfx_setblend(mesh_blend(mesh, i));
    ltex_bindcolor(_image_ptr(mesh_image(mesh, i)));
    lgfx_setcolor(
      color_red(mesh_color(mesh, i)) / 255.0,
      color_green(mesh_color(mesh, i)) / 255.0,
      color_blue(mesh_color(mesh, i)) / 255.0,
      color_alpha(mesh_color(mesh, i)) / 255.0);
    lgfx_setculling((mesh_flags(mesh, i) & 1) == 1);
    lgfx_setdepthwrite((mesh_flags(mesh, i) & 2) == 2);
    if ( (mesh_flags(mesh, i) & 3) == 3 ) _screen_setlighting();
    else lgfx_setlighting(FALSE, 0);
    lvert_drawindexed(
      mesh->buffers[i].vertices,
      mesh->buffers[i].indices,
      sb_count(mesh->buffers[i].indices),
      MODE_TRIANGLES
    );
  }
}
