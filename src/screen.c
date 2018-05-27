#include "../lib/litelibs/litegfx.h"
#include "../lib/litelibs/litemath3d.h"
#include "../lib/stretchy_buffer.h"
#include "camera.h"
#include "color.h"
#include "default_font.h"
#include "font.h"
#include "light.h"
#include "object.h"
#include "physics.h"
#include "platform.h"
#include "screen.h"
#include <string.h>

struct loadedfont_s
{
  char          name[STRING_SIZE];
  float           height;
  struct font_s*  font;
};

struct drawdata_s
{
  union
  {
    struct font_s*  font;
    ltex_t*         tex;
  };
  char* text;
  int   color;
  float x, y, w, h;
  void  (* func)(struct drawdata_s*);
};

struct printdata_s
{
  char* text;
  int   color;
};

static void* _screen_ptr = NULL;
static float _screen_delta = 0;
static float _screen_lasttime = 0;
static int _screen_fps = 0;
static int _screen_fpscounter = 0;
static float _screen_fpstime = 0;
static int _screen_color = 0xffffffff;
static struct loadedfont_s* _screen_loadedfonts = NULL;
static struct font_s* _default_font = NULL;
static struct font_s* _screen_font = NULL;
static struct drawdata_s* _screen_draws = NULL;
static struct printdata_s* _screen_prints = NULL;
static bool_t _screen_didsync = FALSE;
static int _screen_numlights = 0;

static void _draw_text(struct drawdata_s* dd)
{
  lgfx_setcolor(color_red(dd->color) / 255.0f, color_green(dd->color) / 255.0f, color_blue(dd->color) / 255.0f, color_alpha(dd->color) / 255.0f);
  font_draw(dd->font, dd->text, dd->x, dd->y);
  free(dd->text);
}

void screen_set(int width, int height, bool_t fullscreen, bool_t resizable)
{
  if ( _screen_ptr ) p_close_screen(_screen_ptr);
  _screen_ptr = p_open_screen(width, height, fullscreen, 0, TRUE, resizable);
  if ( _default_font ) font_release(_default_font);
  _default_font = _font_loadbase64(DEFAULT_FONT, DEFAULT_FONT_BLOCKSIZE, 16);
  _screen_font = _default_font;
}

void screen_sync()
{
  lmat4_t view_matrix;
  int v, i;
  float print_y;

  /* report that screen was synced while running script */
  _screen_didsync = TRUE;

  /* update physics */
  _physics_update(_screen_delta);

  /* draw 3d scene */
  lgfx_setup3d(0, 0);
  for ( v = 0; v < NUM_CAMERAS; ++v )
  {
    if ( camera_visible(v) )
    {
      _screen_numlights = 0;

      view_matrix = _camera_prepare(v);

      for ( i = 0; i < NUM_LIGHTS; ++i )
      {
        _screen_numlights += _light_prepare(i, &view_matrix);
      }

      for ( i = 0; i < NUM_OBJECTS; ++i )
      {
        _object_draw(i, &view_matrix);
      }
    }
  }

  /* draw 2d stuff */
  lgfx_setup2d(screen_width(), screen_height());
  for ( i = 0; i < sb_count(_screen_draws); ++i )
  {
    _screen_draws[i].func(&_screen_draws[i]);
  }
  sb_free(_screen_draws);
  _screen_draws = NULL;

  /* draw prints */
  print_y = 2;
  for ( i = 0; i < sb_count(_screen_prints); ++i )
  {
    lgfx_setcolor(
      color_red(_screen_prints[i].color) / 255.0f,
      color_green(_screen_prints[i].color) / 255.0f,
      color_blue(_screen_prints[i].color) / 255.0f,
      color_alpha(_screen_prints[i].color) / 255.0f);
    font_draw(_default_font, _screen_prints[i].text, 2, print_y);
    print_y += font_height(_default_font) + 2;
  }

  /* refresh screen */
  p_refresh_screen(_screen_ptr);

  /* update delta time */
  _screen_delta = p_get_time() - _screen_lasttime;
  _screen_lasttime = p_get_time();

  /* update fps */
  ++_screen_fpscounter;
  _screen_fpstime += _screen_delta;
  if ( _screen_fpstime >= 1 )
  {
    _screen_fps = _screen_fpscounter;
    _screen_fpscounter = 0;
    _screen_fpstime -= 1;
  }
}

void screen_settitle(const char* title)
{
  p_set_screen_title(_screen_ptr, title);
}

void screen_setdrawcolor(int color)
{
  _screen_color = color;
}

void screen_setdrawfont(const char* filename, float height)
{
  struct font_s* font = NULL;
  int i;

  /* search for already loaded font */
  for ( i = 0; i < sb_count(_screen_loadedfonts); ++i )
  {
    if ( strcmp(_screen_loadedfonts[i].name, filename) == 0 && _screen_loadedfonts[i].height == height )
    {
      _screen_font = _screen_loadedfonts[i].font;
      return;
    }
  }

  /* load font */
  font = font_load(filename, height);
  if ( font )
  {
    struct loadedfont_s data;

    strncpy(data.name, filename, STRING_SIZE);
    data.name[STRING_SIZE-1] = 0;
    data.height = height;
    data.font = font;
    sb_push(_screen_loadedfonts, data);
    _screen_font = font;
  }
}

void screen_setdrawfontdefault()
{
  _screen_font = _default_font;
}

void screen_drawtext(const char* text, float x, float y)
{
  struct drawdata_s dd;
  size_t len;

  len = strlen(text);
  if ( len > 0 )
  {
    dd.font = _screen_font;
    dd.text = (char*)malloc(len+1);
    dd.color = _screen_color;
    dd.x = x;
    dd.y = y;
    dd.func = _draw_text;
    strcpy(dd.text, text);
    sb_push(_screen_draws, dd);
  }
}

void screen_print(const char* text)
{
  struct printdata_s pd;
  size_t len;

  len = strlen(text);
  pd.text = (char*)malloc(len+1);
  pd.color = _screen_color;
  strcpy(pd.text, text);
  sb_push(_screen_prints, pd);
}

const char* screen_input(const char* prompt)
{

}

void screen_clearprints()
{
  int i;
  for ( i = 0; i < sb_count(_screen_prints); ++i ) free(_screen_prints[i].text);
  sb_free(_screen_prints);
  _screen_prints = NULL;
}

int screen_width()
{
  return p_screen_width(_screen_ptr);
}

int screen_height()
{
  return p_screen_height(_screen_ptr);
}

float screen_delta()
{
  return _screen_delta;
}

int screen_fps()
{
  return _screen_fps;
}

bool_t screen_opened()
{
  return p_screen_opened(_screen_ptr);
}

int screen_desktopwidth()
{
  return p_desktop_width();
}

int screen_desktopheight()
{
  return p_desktop_height();
}

float screen_textwidth(const char* text)
{
  return font_textwidth(_screen_font, text);
}

float screen_textheight(const char* text)
{
  return font_textheight(_screen_font, text);
}

void* _screen_pointer()
{
  return _screen_ptr;
}

bool_t _screen_synced()
{
  return _screen_didsync;
}

void _screen_setlighting()
{
  lgfx_setlighting(_screen_numlights > 0, _screen_numlights);
}