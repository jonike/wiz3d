#include "../lib/litelibs/litegfx.h"
#include "../lib/litelibs/litemath3d.h"
#include "color.h"
#include "image.h"
#include "mesh.h"
#include "object.h"
#include "util.h"
#include <string.h>

struct object_s
{
  char            name[STRING_SIZE];
  struct mesh_s*  mesh;
  bool_t          visible;
  lvec3_t         position;
  lvec3_t         rotation;
  lvec3_t         scale;
};

static struct object_s* _objects[NUM_OBJECTS] = { NULL };

int object_create(int object)
{
  /* delete previous object if it exists */
  if ( object_exists(object) ) object_delete(object);

  /* find available object index if -1 is received */
  if ( object == -1 )
  {
    int i;
    for ( i = 0; i < NUM_OBJECTS; ++i )
    {
      if ( !object_exists(i) )
      {
        object = i;
        break;
      }
    }
  }

  /* return if there are no more objects available */
  if ( object == -1 ) return -1;

  /* create object */
  _objects[object] = (struct object_s*)malloc(sizeof(struct object_s));
  _objects[object]->mesh = mesh_create();
  object_setname(object, "");
  object_setvisible(object, TRUE);
  object_setposition(object, 0, 0, 0);
  object_setrotation(object, 0, 0, 0);
  object_setscale(object, 1, 1, 1);

  return object;
}

int object_createcube(int object)
{
  object = object_create(object);
  if ( object != -1 )
  {
    struct mesh_s* mesh;
    int buffer;

    mesh = _objects[object]->mesh;
    buffer = mesh_addbuffer(mesh);
    
    /* add front face */
    mesh_addvertex(mesh, buffer, -0.5f,  0.5f, -0.5f, 0, 0, -1, 0, 0);
    mesh_addvertex(mesh, buffer,  0.5f,  0.5f, -0.5f, 0, 0, -1, 1, 0);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 1);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 1);
    mesh_addtriangle(mesh, buffer, 0, 1, 2);
    mesh_addtriangle(mesh, buffer, 0, 2, 3);

    /* add right face */
    mesh_addvertex(mesh, buffer,  0.5f,  0.5f, -0.5f, 1, 0, 0, 0, 0);
    mesh_addvertex(mesh, buffer,  0.5f,  0.5f,  0.5f, 1, 0, 0, 1, 0);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f,  0.5f, 1, 0, 0, 1, 1);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 1);
    mesh_addtriangle(mesh, buffer, 4, 5, 6);
    mesh_addtriangle(mesh, buffer, 4, 6, 7);

    /* add back face */
    mesh_addvertex(mesh, buffer,  0.5f,  0.5f, 0.5f, 0, 0, 1, 0, 0);
    mesh_addvertex(mesh, buffer, -0.5f,  0.5f, 0.5f, 0, 0, 1, 1, 0);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 1);
    mesh_addtriangle(mesh, buffer, 8, 9, 10);
    mesh_addtriangle(mesh, buffer, 8, 10, 11);

    /* add left face */
    mesh_addvertex(mesh, buffer, -0.5f,  0.5f,  0.5f, -1, 0, 0, 0, 0);
    mesh_addvertex(mesh, buffer, -0.5f,  0.5f, -0.5f, -1, 0, 0, 1, 0);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f, -0.5f, -1, 0, 0, 1, 1);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f,  0.5f, -1, 0, 0, 0, 1);
    mesh_addtriangle(mesh, buffer, 12, 13, 14);
    mesh_addtriangle(mesh, buffer, 12, 14, 15);

    /* add top face */
    mesh_addvertex(mesh, buffer, -0.5f, 0.5f,  0.5f, 0, 1, 0, 0, 0);
    mesh_addvertex(mesh, buffer,  0.5f, 0.5f,  0.5f, 0, 1, 0, 1, 0);
    mesh_addvertex(mesh, buffer,  0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1);
    mesh_addvertex(mesh, buffer, -0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1);
    mesh_addtriangle(mesh, buffer, 16, 17, 18);
    mesh_addtriangle(mesh, buffer, 16, 18, 19);

    /* add bottom face */
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 0);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f,  0.5f, 0, -1, 0, 1, 1);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f,  0.5f, 0, -1, 0, 0, 1);
    mesh_addtriangle(mesh, buffer, 20, 21, 22);
    mesh_addtriangle(mesh, buffer, 20, 22, 23);
  }

  return object;
}

int object_createtriangle(int object)
{
  object = object_create(object);
  if ( object != -1 )
  {
    struct mesh_s* mesh;
    int buffer;

    mesh = _objects[object]->mesh;
    buffer = mesh_addbuffer(mesh);
    
    mesh_addvertex(mesh, buffer,     0,  0.5f, 0, 0, 0, -1, 0.5f, 1);
    mesh_addvertex(mesh, buffer,  0.5f, -0.5f, 0, 0, 0, -1, 1, 0);
    mesh_addvertex(mesh, buffer, -0.5f, -0.5f, 0, 0, 0, -1, 0, 0);
    mesh_addtriangle(mesh, buffer, 0, 1, 2);
  }

  return object;
}

int object_load(int object, const char* filename)
{
  struct mesh_s* mesh;

  /* delete previous object if it exists */
  if ( object_exists(object) ) object_delete(object);

  /* find available object index if -1 is received */
  if ( object == -1 )
  {
    int i;
    for ( i = 0; i < NUM_OBJECTS; ++i )
    {
      if ( !object_exists(i) )
      {
        object = i;
        break;
      }
    }
  }

  /* return if there are no more objects available */
  if ( object == -1 ) return -1;

  /* load mesh */
  mesh = mesh_load(filename);
  if ( !mesh ) return -1;

  /* create object */
  _objects[object] = (struct object_s*)malloc(sizeof(struct object_s));
  _objects[object]->mesh = mesh;
  object_setname(object, "");
  object_setvisible(object, TRUE);
  object_setposition(object, 0, 0, 0);
  object_setrotation(object, 0, 0, 0);
  object_setscale(object, 1, 1, 1);

  return object;
}

int object_instantiate(int object, int from)
{
  /* if source and destination are the same, we are done */
  if ( object == from ) return object;

  /* make sure from index is valid */
  if ( !object_exists(from) ) return -1;

  /* delete previous object if it exists */
  if ( object_exists(object) ) object_delete(object);

  /* find available object index if -1 is received */
  if ( object == -1 )
  {
    int i;
    for ( i = 0; i < NUM_OBJECTS; ++i )
    {
      if ( !object_exists(i) )
      {
        object = i;
        break;
      }
    }
  }

  /* return if there are no more objects available */
  if ( object == -1 ) return -1;

  /* create object */
  _objects[object] = (struct object_s*)malloc(sizeof(struct object_s));
  _objects[object]->mesh = _objects[from]->mesh;
  object_setname(object, _objects[from]->name);
  object_setvisible(object, _objects[from]->visible);
  object_setposition(object, _objects[from]->position.x, _objects[from]->position.y, _objects[from]->position.z);
  object_setrotation(object, _objects[from]->rotation.x, _objects[from]->rotation.y, _objects[from]->rotation.z);
  object_setscale(object, _objects[from]->scale.x, _objects[from]->scale.y, _objects[from]->scale.z);
  mesh_retain(_objects[object]->mesh);

  return object;
}

void object_delete(int object)
{
  if ( object_exists(object ) )
  {
    mesh_release(_objects[object]->mesh);
    free(_objects[object]);
    _objects[object] = NULL;
  }
}

bool_t object_exists(int object)
{
  if ( object < 0 || object >= NUM_OBJECTS )
  {
    return FALSE;
  }
  else
  {
    return _objects[object] != NULL;
  }
}

void object_setname(int object, const char* name)
{
  if ( object_exists(object ) )
  {
    strncpy(_objects[object]->name, name, STRING_SIZE);
  }
}

const char* object_name(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->name;
  }
  else
  {
    return 0;
  }
}

void object_setvisible(int object, bool_t visible)
{
  if ( object_exists(object ) )
  {
    _objects[object]->visible = visible;
  }
}

bool_t object_visible(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->visible;
  }
  else
  {
    return FALSE;
  }
}

void object_setposition(int object, float x, float y, float z)
{
  if ( object_exists(object ) )
  {
    _objects[object]->position = lvec3(x, y, z);
  }
}

float object_x(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->position.x;
  }
  else
  {
    return 0.0f;
  }
}
float object_y(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->position.y;
  }
  else
  {
    return 0.0f;
  }
}

float object_z(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->position.z;
  }
  else
  {
    return 0.0f;
  }
}

void object_setrotation(int object, float pitch, float yaw, float roll)
{
  if ( object_exists(object ) )
  {
    _objects[object]->rotation = lvec3(pitch, yaw, roll);
  }
}

float object_pitch(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->rotation.x;
  }
  else
  {
    return 0.0f;
  }
}

float object_yaw(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->rotation.y;
  }
  else
  {
    return 0.0f;
  }
}

float object_roll(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->rotation.z;
  }
  else
  {
    return 0.0f;
  }
}

void object_setscale(int object, float x, float y, float z)
{
  if ( object_exists(object ) )
  {
    _objects[object]->scale = lvec3(x, y, z);
  }
}

float object_scalex(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->scale.x;
  }
  else
  {
    return 1.0f;
  }
}

float object_scaley(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->scale.y;
  }
  else
  {
    return 0.0f;
  }
}

float object_scalez(int object)
{
  if ( object_exists(object ) )
  {
    return _objects[object]->scale.z;
  }
  else
  {
    return 0.0f;
  }
}

void object_move(int object, float x, float y, float z)
{
  if ( object_exists(object ) )
  {
    lvec3_t vec;
    lquat_t q;

    vec = lvec3_rad(&_objects[object]->rotation);
    q = lquat_fromeuler(&vec);
    vec = lvec3(x, y, z);
    vec = lquat_mulvec3(&q, &vec);
    _objects[object]->position = lvec3_add(&_objects[object]->position, &vec);
  }
}

void object_turn(int object, float pitch, float yaw, float roll)
{
  if ( object_exists(object) )
  {
    lvec3_t vec;
    
    vec = lvec3(pitch, yaw, roll);
    _objects[object]->rotation = lvec3_add(&_objects[object]->rotation, &vec);
  }
}

int object_numbuffers(int object)
{
  if ( object_exists(object) )
  {
    return mesh_numbuffers(_objects[object]->mesh);
  }
  else
  {
    return 0;
  }
}

void object_setblend(int object, int buffer, int mode)
{
  mode = clamp(mode, _BLEND_SOLID, _BLEND_MUL);
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setblend(_objects[object]->mesh, buffer, mode);
  }
}

int object_blend(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_blend(_objects[object]->mesh, buffer);
  }
  else
  {
    return 0;
  }
}

void object_setimage(int object, int buffer, int image)
{
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setimage(_objects[object]->mesh, buffer, image);
  }
}

int object_image(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_image(_objects[object]->mesh, buffer);
  }
  else
  {
    return -1;
  }
}

void object_setcolor(int object, int buffer, int color)
{
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setcolor(_objects[object]->mesh, buffer, color);
  }
}

int object_color(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_color(_objects[object]->mesh, buffer);
  }
  else
  {
    return 0;
  }
}

void object_setemissive(int object, int buffer, int color)
{
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setemissive(_objects[object]->mesh, buffer, color);
  }
}

int object_emissive(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_emissive(_objects[object]->mesh, buffer);
  }
  else
  {
    return 0;
  }
}

void object_setspecular(int object, int buffer, int color)
{
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setspecular(_objects[object]->mesh, buffer, color);
  }
}

int object_specular(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_specular(_objects[object]->mesh, buffer);
  }
  else
  {
    return 0;
  }
}

void object_setshininess(int object, int buffer, float shininess)
{
  if ( _object_bufferexists(object, buffer) )
  {
    mesh_setshininess(_objects[object]->mesh, buffer, clamp(shininess, 0, 1));
  }
}

float object_shininess(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return mesh_shininess(_objects[object]->mesh, buffer);
  }
  else
  {
    return 0;
  }
}

void object_setculling(int object, int buffer, bool_t culling)
{
  if ( _object_bufferexists(object, buffer) )
  {
    if ( culling )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) | _FLAG_CULL);
    }
    else if ( !culling && object_culling(object, buffer) )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) - _FLAG_CULL);
    }
  }
}

bool_t object_culling(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return (mesh_flags(_objects[object]->mesh, buffer) & _FLAG_CULL) == _FLAG_CULL;
  }
  else
  {
    return FALSE;
  }
}

void object_setdepthwriting(int object, int buffer, bool_t depthwriting)
{
  if ( _object_bufferexists(object, buffer) )
  {
    if ( depthwriting )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) | _FLAG_DEPTHWRITE);
    }
    else if ( !depthwriting && object_depthwriting(object, buffer) )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) - _FLAG_DEPTHWRITE);
    }
  }
}

bool_t object_depthwriting(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return (mesh_flags(_objects[object]->mesh, buffer) & _FLAG_DEPTHWRITE) == _FLAG_DEPTHWRITE;
  }
  else
  {
    return FALSE;
  }
}

void object_setlighting(int object, int buffer, bool_t lighting)
{
  if ( _object_bufferexists(object, buffer) )
  {
    if ( lighting )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) | _FLAG_LIGHTING);
    }
    else if ( !lighting && object_depthwriting(object, buffer) )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) - _FLAG_LIGHTING);
    }
  }
}

bool_t object_lighting(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return (mesh_flags(_objects[object]->mesh, buffer) & _FLAG_LIGHTING) == _FLAG_LIGHTING;
  }
  else
  {
    return FALSE;
  }
}

void object_setfog(int object, int buffer, bool_t fog)
{
  if ( _object_bufferexists(object, buffer) )
  {
    if ( fog )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) | _FLAG_FOG);
    }
    else if ( !fog && object_fog(object, buffer) )
    {
      mesh_setflags(
        _objects[object]->mesh,
        buffer,
        mesh_flags(_objects[object]->mesh, buffer) - _FLAG_FOG);
    }
  }
}

bool_t object_fog(int object, int buffer)
{
  if ( _object_bufferexists(object, buffer) )
  {
    return (mesh_flags(_objects[object]->mesh, buffer) & _FLAG_FOG) == _FLAG_FOG;
  }
  else
  {
    return FALSE;
  }
}

bool_t _object_bufferexists(int object, int buffer)
{
  if ( object_exists(object) )
  {
    return buffer >= 0 && buffer < mesh_numbuffers(_objects[object]->mesh);
  }
  else
  {
    return FALSE;
  }
}

void _object_draw(int object, const lmat4_t* view_matrix)
{
  if ( object_visible(object) )
  {
    lvec3_t radrot;
    lquat_t q;
    lmat4_t modelview;

    /* calculate modelview */
    radrot = lvec3_rad(&_objects[object]->rotation);
    q = lquat_fromeuler(&radrot);
    modelview = lmat4_transform(&_objects[object]->position, &q, &_objects[object]->scale);
    modelview = lmat4_mul(view_matrix, &modelview);
    lgfx_setmodelview(modelview.m);
    
    /* set properties & draw */
    _mesh_draw(_objects[object]->mesh);
  }
}
