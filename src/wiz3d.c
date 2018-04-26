#include "platform.h"
#include "screen.h"
#include "wiz3d.h"
#define LITE_GFX_IMPLEMENTATION
#define LITE_MATH3D_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "../lib/litelibs/litegfx.h"
#include "../lib/litelibs/litemath3d.h"
#include "../lib/stb_image.h"
#include "../lib/stb_truetype.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool_t wiz3d_init()
{
  if ( p_init() )
  {
    _physics_init();
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

void wiz3d_finish()
{
  if ( _screen_pointer() ) p_close_screen(_screen_pointer());
  _physics_finish();
  p_shutdown();
}

#ifdef __cplusplus
} /* extern "C" */
#endif
