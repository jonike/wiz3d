#include "script.h"
#include "util.h"
#include "wiz3d.h"
#include <string.h>

int main(int argc, char* argv[])
{
  char working_dir[256];
  char script_file[256];

  /* get program path */
  dir_extract(argv[0], working_dir, sizeof(working_dir) / sizeof(working_dir[0]));

  /* get script filename */
  strncpy(script_file, argc > 1 ? argv[1] : "data/main.lua", sizeof(script_file) / sizeof(script_file[0]));
  script_file[sizeof(script_file) / sizeof(script_file[0]) - 1] = 0;

  /*  if argv[0] begins with / on macOS, we'll assume it has been launched
      by double clicking on the Finder, and we'll move to the app dir */
#ifdef __APPLE__
  if ( argv[0][0] == '/' ) dir_change(working_dir);
#endif

  /* get working dir */
  dir_extract(script_file, working_dir, sizeof(working_dir) / sizeof(working_dir[0]));

  /* change to working directory */
  dir_change(working_dir);
  dir_current(working_dir, sizeof(working_dir) / sizeof(working_dir[0]));

  /* strip dir from script */
  dir_strip(script_file, script_file, sizeof(script_file) / sizeof(script_file[0]));

  /* setup */
  wiz3d_init();
  screen_set(640, 480, FALSE, FALSE);

  /* create default camera */
  camera_create(0);
  camera_setposition(0, 0, 10, -10);
  camera_setrotation(0, 45, 0, 0);

  /* create default light */
  light_create(0, _LIGHT_DIRECTIONAL);
  light_setrotation(0, 45, -45);

  /* run script */
  if ( !script_init() ) log_error(script_error());
  if ( !script_load(script_file) ) log_error(script_error());

  /* keep running if screen did not sync */
  if ( !_screen_synced() )
  {
    while ( screen_opened() && !input_keydown(_KEY_ESC) )
    {
      screen_sync();
    }
  }

  /* shutdown */
  script_finish();
  wiz3d_finish();
  
  return 0;
}
