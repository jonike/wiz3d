#include "../lib/litelibs/litegfx.h"
#include "color.h"
#include "light.h"
#include "util.h"

struct light_s
{
  int     type;
  bool_t  visible;
  lvec3_t position;
  lvec3_t rotation;
  int     color;
  float   att;
};

static struct light_s* _lights[NUM_LIGHTS] = { NULL };

int light_create(int light, int type)
{
  /* make sure that light type is in range */
  type = clamp(type, _LIGHT_DIRECTIONAL, _LIGHT_POINT);

  /* delete previous light if it exists */
  if ( light_exists(light) ) light_delete(light);

  /* find available light index if -1 is received */
  if ( light == -1 )
  {
    int i;
    for ( i = 0; i < NUM_LIGHTS; ++i )
    {
      if ( !light_exists(i) )
      {
        light = i;
        break;
      }
    }
  }

  /* return if there are no more lights available */
  if ( light == -1 ) return -1;

  /* create light */
  _lights[light] = (struct light_s*)malloc(sizeof(struct light_s));
  _lights[light]->type = type;
  light_setvisible(light, TRUE);
  light_setposition(light, 0, 0, 0);
  light_setrotation(light, 0, 0);
  light_setcolor(light, _COLOR_WHITE);
  light_setattenuation(light, 0);

  return light;
}

void light_delete(int light)
{
  if ( light_exists(light) )
  {
    free(_lights[light]);
    _lights[light] = NULL;
  }
}

bool_t light_exists(int light)
{
  if ( light < 0 || light >= NUM_LIGHTS )
  {
    return FALSE;
  }
  else
  {
    return _lights[light] != NULL;
  }
}

void light_setvisible(int light, bool_t visible)
{
  if ( light_exists(light) )
  {
    _lights[light]->visible = visible;
  }
}

bool_t light_visible(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->visible;
  }
  else
  {
    return FALSE;
  }
}

void light_setposition(int light, float x, float y, float z)
{
  if ( light_exists(light) )
  {
    _lights[light]->position = lvec3(x, y, z);
  }
}

float light_x(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->position.x;
  }
  else
  {
    return 0;
  }
}

float light_y(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->position.y;
  }
  else
  {
    return 0;
  }
}

float light_z(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->position.z;
  }
  else
  {
    return 0;
  }
}

void light_setrotation(int light, float pitch, float yaw)
{
  if ( light_exists(light) )
  {
    _lights[light]->rotation = lvec3(pitch, yaw, 0);
  }
}

float light_pitch(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->rotation.x;
  }
  else
  {
    return 0;
  }
}

float light_yaw(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->rotation.y;
  }
  else
  {
    return 0;
  }
}

void light_setcolor(int light, int color)
{
  if ( light_exists(light) )
  {
    _lights[light]->color = color;
  }
}

int light_color(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->color;
  }
  else
  {
    return 0;
  }
}

void light_setattenuation(int light, float att)
{
  if ( light_exists(light) )
  {
    _lights[light]->att = att;
  }
}

float light_attenuation(int light)
{
  if ( light_exists(light) )
  {
    return _lights[light]->att;
  }
  else
  {
    return 0;
  }
}

void light_move(int light, float x, float y, float z)
{
  if ( light_exists(light ) )
  {
    lvec3_t vec;
    lquat_t q;

    vec = lvec3_rad(&_lights[light]->rotation);
    q = lquat_fromeuler(&vec);
    vec = lvec3(x, y, z);
    vec = lquat_mulvec3(&q, &vec);
    vec = lvec3_add(&_lights[light]->position, &vec);
  }
}

void light_turn(int light, float pitch, float yaw, float roll)
{
  if ( light_exists(light) )
  {
    lvec3_t vec;
    
    vec = lvec3(pitch, yaw, roll);
    _lights[light]->rotation = lvec3_add(&_lights[light]->rotation, &vec);
  }
}

bool_t _light_prepare(int light, const lmat4_t* view_matrix)
{
  if ( light_visible(light) )
  {
    lvec3_t pos;
    
    /* get position in viewer space */
    if ( _lights[light]->type == _LIGHT_DIRECTIONAL ) {
      lvec3_t rot;
      lquat_t q;

      rot = lvec3(_lights[light]->rotation.x, _lights[light]->rotation.y, _lights[light]->rotation.z);
      rot = lvec3_rad(&rot);
      q = lquat_fromeuler(&rot);
      pos = lvec3(0, 0, -1);
      pos = lquat_mulvec3(&q, &pos);
    }
    else
    {
      pos = lvec3(
        _lights[light]->position.x,
        _lights[light]->position.y,
        _lights[light]->position.z);
    }
    pos = lmat4_mulvec3(view_matrix, &pos, 0);

    /* set light data */
    lgfx_setlight(
      light,
      pos.x,
      pos.y,
      pos.z,
      _lights[light]->type,
      color_red(_lights[light]->color) / 255.0f,
      color_green(_lights[light]->color) / 255.0f,
      color_blue(_lights[light]->color) / 255.0f,
      _lights[light]->att);
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}