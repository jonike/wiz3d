#include "../lib/litelibs/litegfx.h"
#include "camera.h"
#include "color.h"
#include "screen.h"
#include <math.h>

struct camera_s
{
  bool_t  visible;
  lvec3_t position;
  lvec3_t rotation;
  int     viewport[4];
  int     clear_color;
  bool_t  ortho;
  float   fov;
  float   range[2];
};

static struct camera_s* _cameras[NUM_CAMERAS] = { NULL };

int camera_create(int camera)
{
  /* delete previous camera if it exists */
  if ( camera_exists(camera) ) camera_delete(camera);

  /* find available camera index if -1 is received */
  if ( camera == -1 )
  {
    int i;
    for ( i = 0; i < NUM_CAMERAS; ++i )
    {
      if ( !camera_exists(i) )
      {
        camera = i;
        break;
      }
    }
  }

  /* return if there are no more cameras available */
  if ( camera == -1 ) return -1;

  /* create camera */
  _cameras[camera] = (struct camera_s*)malloc(sizeof(struct camera_s));
  camera_setvisible(camera, TRUE);
  camera_setviewport(camera, 0, 0, -1, -1);
  camera_setclearcolor(camera, color_rgb(52, 73, 94));
  camera_setortho(camera, FALSE);
  camera_setfov(camera, 60);
  camera_setrange(camera, 1, 1000);
  camera_setposition(camera, 0, 0, 0);
  camera_setrotation(camera, 0, 0, 0);

  return camera;
}

void camera_delete(int camera)
{
  if ( camera_exists(camera) )
  {
    free(_cameras[camera]);
    _cameras[camera] = NULL;
  }
}

bool_t camera_exists(int camera) {
  if ( camera < 0 || camera >= NUM_CAMERAS )
  {
    return FALSE;
  }
  else
  {
    return _cameras[camera] != NULL;
  }
}

void camera_setvisible(int camera, bool_t visible)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->visible = visible;
  }
}

bool_t camera_visible(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->visible;
  }
  else
  {
    return FALSE;
  }
}

void camera_setviewport(int camera, int x, int y, int width, int height)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->viewport[0] = x;
    _cameras[camera]->viewport[1] = y;
    _cameras[camera]->viewport[2] = width;
    _cameras[camera]->viewport[3] = height;
  }
}

int camera_viewportx(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->viewport[0];
  }
  else
  {
    return 0;
  }
}

int camera_viewporty(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->viewport[1];
  }
  else
  {
    return 0;
  }
}

int camera_viewportwidth(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->viewport[2] > -1 ? _cameras[camera]->viewport[2] : screen_width();
  }
  else
  {
    return 0;
  }
}

int camera_viewportheight(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->viewport[3] > -1 ? _cameras[camera]->viewport[3] : screen_height();
  }
  else
  {
    return 0;
  }
}

void camera_setclearcolor(int camera, int color)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->clear_color = color;
  }
}

int camera_clearcolor(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->clear_color;
  }
  else
  {
    return 0;
  }
}

void camera_setortho(int camera, bool_t ortho)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->ortho = ortho;
  }
}

bool_t camera_ortho(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->ortho;
  }
  else
  {
    return FALSE;
  }
}

void camera_setfov(int camera, float fov)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->fov = fov;
  }
}

float camera_fov(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->fov;
  }
  else
  {
    return 0;
  }
}

void camera_setrange(int camera, float near_, float far_)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->range[0] = near_;
    _cameras[camera]->range[1] = far_;
  }
}

float camera_rangenear(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->range[0];
  }
  else
  {
    return 0;
  }
}

float camera_rangefar(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->range[1];
  }
  else
  {
    return 0;
  }
}

void camera_setposition(int camera, float x, float y, float z)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->position = lvec3(x, y, z);
  }
}

float camera_x(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->position.x;
  }
  else
  {
    return 0;
  }
}

float camera_y(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->position.y;
  }
  else
  {
    return 0;
  }
}

float camera_z(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->position.z;
  }
  else
  {
    return 0;
  }
}

void camera_setrotation(int camera, float pitch, float yaw, float roll)
{
  if ( camera_exists(camera) )
  {
    _cameras[camera]->rotation = lvec3(pitch, yaw, roll);
  }
}

float camera_pitch(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->rotation.x;
  }
  else
  {
    return 0;
  }
}

float camera_yaw(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->rotation.y;
  }
  else
  {
    return 0;
  }
}

float camera_roll(int camera)
{
  if ( camera_exists(camera) )
  {
    return _cameras[camera]->rotation.z;
  }
  else
  {
    return 0;
  }
}

void camera_move(int camera, float x, float y, float z)
{
  if ( camera_exists(camera) )
  {
    lvec3_t vec;
    lquat_t q;

    vec = lvec3_rad(&_cameras[camera]->rotation);
    q = lquat_fromeuler(&vec);
    vec = lvec3(x, y, z);
    vec = lquat_mulvec3(&q, &vec);
    vec = lvec3_add(&_cameras[camera]->position, &vec);
  }
}

void camera_turn(int camera, float pitch, float yaw, float roll)
{
  if ( camera_exists(camera) )
  {
    lvec3_t vec;
    
    vec = lvec3(pitch, yaw, roll);
    _cameras[camera]->rotation = lvec3_add(&_cameras[camera]->rotation, &vec);
  }
}

lmat4_t _camera_prepare(int camera)
{
  float ratio;
  lmat4_t proj;
  lmat4_t view;
  lvec3_t negpos;
  lvec3_t radrot;
  lvec3_t axis;
  lquat_t q;

  /* set viewport (must be done before setting projection) */
  lgfx_setviewport(camera_viewportx(camera), camera_viewporty(camera), camera_viewportwidth(camera), camera_viewportheight(camera));

  /* prepare projection */
  ratio = camera_viewportwidth(camera) / (float)camera_viewportheight(camera);
  if ( _cameras[camera]->ortho )
  {
    float width, height;
    height = _cameras[camera]->range[0] * tan(lm_deg2rad(_cameras[camera]->fov)) * 2;
    width = height * ratio;
    proj = lmat4_ortholh(-width, width, -height, height, _cameras[camera]->range[0], _cameras[camera]->range[1]);
  }
  else
  {
    proj = lmat4_perspectivelh(lm_deg2rad(_cameras[camera]->fov), ratio, _cameras[camera]->range[0], _cameras[camera]->range[1]);
  }
  lgfx_setprojection(proj.m);

  /* prepare view */
  negpos = lvec3(-_cameras[camera]->position.x, -_cameras[camera]->position.y, -_cameras[camera]->position.z);
  radrot = lvec3_rad(&_cameras[camera]->rotation);
  q = lquat_fromeuler(&radrot);
  axis = lquat_axis(&q);
  view = lmat4_identity();
  view = lmat4_rotate(&view, -lquat_angle(&q), &axis);
  view = lmat4_translate(&view, &negpos);

  /* clear buffers */
  lgfx_setdepthwrite(TRUE);
  lgfx_clearcolorbuffer(color_red(_cameras[camera]->clear_color) / 255.0f, color_green(_cameras[camera]->clear_color) / 255.0f, color_blue(_cameras[camera]->clear_color) / 255.0f);
  lgfx_cleardepthbuffer();

  return view;
}
