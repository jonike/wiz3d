#ifndef EDITORWINDOW_H_INCLUDED
#define EDITORWINDOW_H_INCLUDED

#include "common.h"
#include "dialog.h"
#include "style.h"

class editor_window_t : public Fl_Window
{
public:
  editor_window_t(int w, int h);
  ~editor_window_t();

  void load_file(const char *newfile, int ipos);
protected:
  void set_title();

  int check_save();
  void save_file(const char *newfile);

  void new_file();
  void open();
  void save();
  void saveas();
  void quit();

  void cut();
  void copy();
  void paste();
  void del();

  void find();
  void find_next();

  void replace();
  void replace_next();
  void replace_all();
  void replace_cancel();

  void modified(int nInserted, int nDeleted);
  void insert();
  void close();

  Fl_Window          *replace_dlg;
  Fl_Input           *replace_find;
  Fl_Input           *replace_with;
  Fl_Button          *m_replace_all;
  Fl_Return_Button   *m_replace_next;
  Fl_Button          *m_replace_cancel;

  Fl_Text_Buffer*     m_text_buffer;
  Fl_Text_Editor*     m_editor;
  char                search[256];

  bool                m_changed;
  bool                m_loading;
  char                m_filename[256];
  char                m_title[256];
private:
  static void new_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->new_file(); }
  static void open_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->open(); }
  static void save_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->save(); }
  static void saveas_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->saveas(); }
  static void quit_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->quit(); }
  static void insert_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->insert(); }
  static void close_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->close(); }
  static void cut_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->cut(); }
  static void copy_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->copy(); }
  static void paste_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->paste(); }
  static void delete_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->del(); }
  static void find_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->find(); }
  static void find_next_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->find_next(); }
  static void replace_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->replace(); }
  static void replace_next_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->replace_next(); }
  static void replace_all_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->replace_all(); }
  static void replace_cancel_callback(Fl_Widget* w, void* v) { ((editor_window_t*)v)->replace_cancel(); }
  static void modified_callback(int, int nInserted, int nDeleted, int, const char*, void* v) { ((editor_window_t*)v)->modified(nInserted, nDeleted); }
};

inline editor_window_t::editor_window_t(int w, int h)
:
  Fl_Window(w, h, ""),
  m_changed(false),
  m_loading(false),
  m_filename(""),
  m_title("")
{
  replace_dlg = new Fl_Window(300, 105, "Replace");
  replace_dlg->begin();
  replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
  replace_find->align(FL_ALIGN_LEFT);

  replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
  replace_with->align(FL_ALIGN_LEFT);

  m_replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
  m_replace_all->callback(replace_all_callback, this);

  m_replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
  m_replace_next->callback(replace_next_callback, this);

  m_replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
  m_replace_cancel->callback((Fl_Callback *)replace_cancel_callback, this);
  replace_dlg->end();
  replace_dlg->set_non_modal();

  *search = '\0';

  begin();
  
  Fl_Menu_Item menu_items[] =
  {
    {"&File", 0, 0, 0, FL_SUBMENU},
      {"&New File",         0, (Fl_Callback *)new_callback},
      {"&Open File...",     FL_COMMAND + 'O', (Fl_Callback *)open_callback},
      {"&Insert File...",   FL_COMMAND + 'I', (Fl_Callback *)insert_callback, 0, FL_MENU_DIVIDER},
      {"&Save File",        FL_COMMAND + 'S', (Fl_Callback *)save_callback},
      {"Save File &As...",  FL_COMMAND + FL_SHIFT + 'S', (Fl_Callback *)saveas_callback, 0, FL_MENU_DIVIDER},
      //{"New &View",         FL_COMMAND + 'N', (Fl_Callback *)view_cb, 0},
      //{"&Close View",       FL_COMMAND + 'W', (Fl_Callback *)close_callback, 0, FL_MENU_DIVIDER},
      {"E&xit",             FL_COMMAND + 'Q', (Fl_Callback *)quit_callback, 0},
      {0},
    {"&Edit", 0, 0, 0, FL_SUBMENU},
      {"Cu&t",    FL_COMMAND + 'X', cut_callback},
      {"&Copy",   FL_COMMAND + 'C', copy_callback},
      {"&Paste",  FL_COMMAND + 'V', paste_callback, 0, FL_MENU_DIVIDER},
      {"&Delete", 0, (Fl_Callback *)delete_callback, 0, FL_MENU_DIVIDER},
      {"&Find...",        FL_COMMAND + 'F', find_callback},
      {"F&ind Next",     FL_COMMAND + 'G', find_next_callback},
      {"&Replace...",     FL_COMMAND + 'R', replace_callback},
      {"Re&place Next",  FL_COMMAND + 'T', replace_next_callback},
      {0},
    {"&Program", 0, 0, 0, FL_SUBMENU},
      {"Run"},
      {0},
    {"&Help", 0, 0, 0, FL_SUBMENU},
      {"Home"},
      {"About"},
      {0},
    {0}
  };
  Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, WIN_WIDTH, 21);
  m->copy(menu_items, this);
  
  m_text_buffer = new Fl_Text_Buffer(0);
  style_init(m_text_buffer);

  m_editor = new Fl_Text_Editor(0, 21, WIN_WIDTH, WIN_HEIGHT - 21);
  m_editor->color(DARKCOLOR2);
  m_editor->buffer(m_text_buffer);
  m_editor->highlight_data(stylebuf, styletable, sizeof(styletable) / sizeof(styletable[0]), 'A', style_unfinished_cb, 0);
  m_editor->textfont(FL_COURIER);
  
  end();
  
  resizable(m_editor);
  callback(close_callback, this);

  //m_editor->linenumber_width(40);
  m_editor->wrap_mode(Fl_Text_Display::WRAP_NONE, 0);
  //m_editor->cursor_style(Fl_Text_Display::BLOCK_CURSOR);
  m_editor->cursor_color(WARNINGCOLOR2);
  // m_editor->insert_mode(false);

  m_text_buffer->add_modify_callback(style_update, m_editor);
  m_text_buffer->add_modify_callback(modified_callback, this);
  m_text_buffer->call_modify_callbacks();
}

inline editor_window_t::~editor_window_t()
{
  delete replace_dlg;
}

inline void editor_window_t::load_file(const char *newfile, int ipos)
{
  m_loading = true;
  int insert = (ipos != -1);
  m_changed = insert;
  if ( !insert ) strcpy(m_filename, "");
  int r;
  if ( !insert ) r = m_text_buffer->loadfile(newfile);
  else r = m_text_buffer->insertfile(newfile, ipos);
  if ( r )
  {
    // fl_ask("File '%s' does not exit. Do you want to create one?", newfile)
    char msg[1024];
    sprintf(msg, "File '%s' does not exit. Do you want to create one?", newfile);
    if ( dialog_t::confirm("Open File", msg) ) strcpy(m_filename, newfile);
    else strcpy(m_filename, "");
  }
  else if ( !insert )
  {
    strcpy(m_filename, newfile);
  }
  m_loading = false;
  m_text_buffer->call_modify_callbacks();
}

inline void editor_window_t::set_title()
{
  if (m_filename[0] == '\0')
  {
    strcpy(m_title, "<untitled>");
  }
  else
  {
    char *slash;
    slash = strrchr(m_filename, '/');
#ifdef WIN32
    if ( slash == NULL ) slash = strrchr(m_filename, '\\');
#endif
    if ( slash != NULL ) strcpy(m_title, slash + 1);
    else strcpy(m_title, m_filename);
  }

  if ( m_changed ) strcat(m_title, " *");

  label(m_title);
}

inline int editor_window_t::check_save()
{
  if ( !m_changed ) return 1;

  //int r = fl_choice("The current file has not been saved.\nWould you like to save it now?", "Cancel", "Save", "Don't Save");
  int r = dialog_t::proceed("Save File", "The current file has not been saved.\nWould you like to save it now?");

  if ( r == 1 )
  {
    save(); // Save the file...
    return !m_changed;
  }

  return ( r == 2 ) ? 1 : 0;
  return 0;
}

inline void editor_window_t::save_file(const char* newfile)
{
  if ( m_text_buffer->savefile(newfile) )
  {
    //fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    char msg[1024];
    sprintf(msg, "Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    dialog_t::notify("Error", msg, true);
  }
  else
  {
    strcpy(m_filename, newfile);
  }
  m_changed = 0;
  m_text_buffer->call_modify_callbacks();
}

inline void editor_window_t::new_file()
{
  if ( !check_save() ) return;

  m_filename[0] = '\0';
  m_text_buffer->select(0, m_text_buffer->length());
  m_text_buffer->remove_selection();
  m_changed = 0;
  m_text_buffer->call_modify_callbacks();
}

inline void editor_window_t::open() {
  if ( !check_save() ) return;

  const char* filters[] = {"*.lua"};
  const char* newfile = dialog_t::file("Open File", 1, filters, false, m_filename);
  if ( newfile ) load_file(newfile, -1);
}

inline void editor_window_t::save() {
  if ( m_filename[0] == '\0' )
  {
    // No filename - get one!
    saveas();
    return;
  }
  else
  {
    save_file(m_filename);
  }
}

inline void editor_window_t::saveas() {
  //const char *newfile = fl_file_chooser("Save File As?", "*", m_filename);
  const char* filters[] = {"*.lua"};
  const char* newfile = dialog_t::file("Save File As", 1, filters, true, m_filename);
  if ( newfile ) save_file(newfile);
}

inline void editor_window_t::quit() {
  if ( m_changed && !check_save() ) return;
  exit(0);
}

inline void editor_window_t::cut()
{
  Fl_Text_Editor::kf_cut(0, m_editor);
}

inline void editor_window_t::copy()
{
  Fl_Text_Editor::kf_copy(0, m_editor);
}

inline void editor_window_t::paste()
{
  Fl_Text_Editor::kf_paste(0, m_editor);
}

inline void editor_window_t::del()
{
  m_text_buffer->remove_selection();
}

inline void editor_window_t::find()
{
  const char *val;
  //val = fl_input("Search String:", search);
  val = dialog_t::input("Search", "String to find:", search);
  if ( val != NULL )
  {
    // User entered a string - go find it!
    strcpy(search, val);
    find_next();
  }
}

inline void editor_window_t::find_next()
{
  if ( search[0] == '\0' )
  {
    // Search string is blank; get a new one...
    find();
    return;
  }

  int pos = m_editor->insert_position();
  int found = m_text_buffer->search_forward(pos, search, &pos);
  if ( found )
  {
    // Found a match; select and update the position...
    m_text_buffer->select(pos, pos+strlen(search));
    m_editor->insert_position(pos+strlen(search));
    m_editor->show_insert_position();
  }
  else
  {
    //fl_alert("No occurrences of \'%s\' found!", search);
    char msg[1024];
    sprintf(msg, "No occurrences of \'%s\' found!", search);
    dialog_t::notify("Search", msg);
  }
}

inline void editor_window_t::replace()
{
  replace_dlg->show();
}

inline void editor_window_t::replace_next()
{
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  if ( find[0] == '\0' )
  {
    // Search string is blank; get a new one...
    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  int pos = m_editor->insert_position();
  int found = m_text_buffer->search_forward(pos, find, &pos);

  if ( found )
  {
    // Found a match; update the position and replace text...
    m_text_buffer->select(pos, pos+strlen(find));
    m_text_buffer->remove_selection();
    m_text_buffer->insert(pos, replace);
    m_text_buffer->select(pos, pos+strlen(replace));
    m_editor->insert_position(pos+strlen(replace));
    m_editor->show_insert_position();
  }
  else
  {
    //fl_alert("No occurrences of \'%s\' found!", find);
    char msg[1024];
    sprintf(msg, "No occurrences of \'%s\' found!", find);
    dialog_t::notify("Replace", msg);
  }
}

inline void editor_window_t::replace_all()
{
  const char *find = replace_find->value();
  const char *replace = replace_with->value();

  find = replace_find->value();
  if ( find[0] == '\0' )
  {
    // Search string is blank; get a new one...
    replace_dlg->show();
    return;
  }

  replace_dlg->hide();

  m_editor->insert_position(0);
  int times = 0;

  // Loop through the whole string
  for ( int found = 1; found; )
  {
    int pos = m_editor->insert_position();
    found = m_text_buffer->search_forward(pos, find, &pos);

    if ( found )
    {
      // Found a match; update the position and replace text...
      m_text_buffer->select(pos, pos+strlen(find));
      m_text_buffer->remove_selection();
      m_text_buffer->insert(pos, replace);
      m_editor->insert_position(pos+strlen(replace));
      m_editor->show_insert_position();
      times++;
    }
  }

  if ( times )
  {
    //fl_message("Replaced %d occurrences.", times);
    char msg[1024];
    sprintf(msg, "Replaced %d occurrences.", times);
    dialog_t::notify("Replace All", msg);
  }
  else
  {
    //fl_alert("No occurrences of \'%s\' found!", find);
    char msg[1024];
    sprintf(msg, "No occurrences of \'%s\' found!", find);
    dialog_t::notify("Replace All", msg);
  }
}

inline void editor_window_t::replace_cancel()
{
  replace_dlg->hide();
}

inline void editor_window_t::modified(int nInserted, int nDeleted)
{
  if ( (nInserted || nDeleted) && !m_loading ) m_changed = true;
  set_title();
  if ( m_loading ) m_editor->show_insert_position();
}

inline void editor_window_t::insert()
{
  //const char *newfile = fl_file_chooser("Insert File?", "*", m_filename);
  const char* filters[] = {"*.lua"};
  const char* newfile = dialog_t::file("Insert File", 1, filters, false, m_filename);
  if ( newfile != NULL ) load_file(newfile, m_editor->insert_position());
}

inline void editor_window_t::close()
{
  if ( !check_save() ) return;
  hide();
  m_text_buffer->remove_modify_callback(modified_callback, this);
  delete this;
  exit(0);
}

#endif // EDITORWINDOW_H_INCLUDED
