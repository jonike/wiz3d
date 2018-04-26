#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include "tinyfiledialogs/tinyfiledialogs.h"

class dialog_t
{
public:
    static bool confirm(const char* title, const char* text, bool serious = false);
    static void notify(const char* title, const char* text, bool serious = false);
    static int proceed(const char* title, const char* text, bool serious = false);
    static bool color(const char* title, unsigned char* r, unsigned char* g, unsigned char* b);
    static const char* dir(const char* title, const char* dir = "");
    static const char* file(const char* title, int numfilters, const char** filters = 0, bool save = false, const char* file = "");
    static const char* input(const char* title, const char* text, const char* def = "", bool password = false);
private:
    dialog_t() {}
    ~dialog_t() {}
};

bool dialog_t::confirm(const char* title, const char* text, bool serious)
{
  return tinyfd_messageBox(title, text, "yesno", serious ? "error" : "question", 1) == 1;
}

void dialog_t::notify(const char* title, const char* text, bool serious) {
  tinyfd_messageBox(title, text, "ok", serious ? "error" : "info", 0);
}

int dialog_t::proceed(const char* title, const char* text, bool serious)
{
  return tinyfd_messageBox(title, text, "yesnocancel", serious ? "error" : "question", 0);
}

bool dialog_t::color(const char* title, unsigned char* r, unsigned char* g, unsigned char* b)
{
  unsigned char c[3];
  c[0] = *r;
  c[1] = *g;
  c[2] = *b;
  
  if ( tinyfd_colorChooser(title, 0, c, c) )
  {
    *r = c[0];
    *g = c[1];
    *b = c[2];
    return true;
  }

  return false;
}

const char* dialog_t::dir(const char* title, const char* dir)
{
  return tinyfd_selectFolderDialog(title, dir);
}

const char* dialog_t::file(const char* title, int numfilters, const char** filters, bool save, const char* file)
{
  if ( !save ) {
    return tinyfd_openFileDialog(title, file, numfilters, filters, 0, 0);
  } else {
    return tinyfd_saveFileDialog(title, file, numfilters, filters, 0);
  }
}

const char* dialog_t::input(const char* title, const char* text, const char* def, bool password)
{
  return tinyfd_inputBox(title, text, password ? (const char*)0 : def);
}

#endif // DIALOG_H_INCLUDED
