// A simple text editor program made in FLTK, inspired by Blitz3D editor
// and based on the program described in Chapter 4 of the FLTK Programmer's Guide.

#include "common.h"
#include "editorwindow.h"

int main(int argc, char **argv)
{
  // Set FLTK Scheme (options: none, base, plastic, gtk+, gleam)
  Fl::scheme("gtk+");

  // Create editor window
  editor_window_t* window = new editor_window_t(WIN_WIDTH, WIN_HEIGHT);
  window->show(1, argv);

  if ( argc > 1 )
  {
    window->label(" "); // Prevent from displaying "Untitled.txt" before its time...
    window->load_file(argv[1], -1);
  }

  return Fl::run();
}
