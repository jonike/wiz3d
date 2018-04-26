#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include "common.h"

// Define editor colors
#define DARKCOLOR1 fl_rgb_color(52, 73, 94)
#define DARKCOLOR2 fl_rgb_color(44, 62, 80)
#define LIGHTCOLOR1 fl_rgb_color(236, 240, 241)
#define LIGHTCOLOR2 fl_rgb_color(46, 204, 113)
#define WARNINGCOLOR1 fl_rgb_color(230, 126, 34)
#define WARNINGCOLOR2 fl_rgb_color(241, 196, 15)

// Syntax highlighting stuff...
Fl_Text_Buffer*                     stylebuf = 0;

// Style table
Fl_Text_Display::Style_Table_Entry  styletable[] =
{
  { LIGHTCOLOR1,    FL_COURIER, 16 }, // A - Plain
  { LIGHTCOLOR2,    FL_COURIER, 16 }, // B - Line comments
  { LIGHTCOLOR2,    FL_COURIER, 16 }, // C - Block comments
  { WARNINGCOLOR2,  FL_COURIER, 16 }, // D - Strings
  { WARNINGCOLOR2,  FL_COURIER, 16 }, // E - Directives
  { WARNINGCOLOR1,  FL_COURIER, 16 }, // F - Types
  { WARNINGCOLOR1,  FL_COURIER, 16 /*, Fl_Text_Display::ATTR_UNDERLINE*/ }  // G - Keywords
};

// List of Lua keywords...
const char* code_keywords[] =
{
  "and",
  "break",
  "do",
  "else",
  "elseif",
  "end",
  "false",
  "for",
  "function",
  "if",
  "in",
  "local",
  "nil",
  "not",
  "or",
  "repeat",
  "return",
  "then",
  "true",
  "until",
  "while"
};

// List of known Lua types...
const char* code_types[] =
{
  "self"
};

//
// 'compare_keywords()' - Compare two keywords...
//

int compare_keywords(const void *a, const void *b)
{
  return (strcmp(*((const char **)a), *((const char **)b)));
}


//
// 'style_parse()' - Parse text and produce style data.
//

void style_parse(const char *text, char *style, int length)
{
  char        current;
  int         col;
  int         last;
  char        buf[255],
  *bufptr;
  const char* temp;

  // Style letters:
  //
  // A - Plain
  // B - Line comments
  // C - Block comments
  // D - Strings
  // E - Directives
  // F - Types
  // G - Keywords

  for ( current = *style, col = 0, last = 0; length > 0; length--, text++ )
  {
    if ( current == 'B' || current == 'F' || current == 'G' ) current = 'A';
    if ( current == 'A' )
    {
      // Check for directives, comments, strings, and keywords...
      /*if ( col == 0 && *text == '#' )
      {
        current = 'E';  // Set style to directive
      }
      */
      if ( strncmp(text, "--[[", 4) == 0 )
      {
        current = 'C';
      }
      else if ( strncmp(text, "--", 2) == 0 )
      {
        current = 'B';
        for ( ; length > 0 && *text != '\n'; length--, text++ ) *style++ = 'B';

        if ( length == 0 ) break;
      }
      else if ( strncmp(text, "\\\"", 2) == 0 )
      {
        // Quoted quote...
        *style++ = current;
        *style++ = current;
        text++;
        length--;
        col += 2;
        continue;
      }
      else if ( *text == '\"' )
      {
        current = 'D';
      }
      else if ( !last && (islower(*text) || *text == '_') )
      {
        // Might be a keyword...
        for ( temp = text, bufptr = buf; (islower(*temp) || *temp == '_') && bufptr < (buf + sizeof(buf) - 1); *bufptr++ = *temp++ );

        if ( !islower(*temp) && *temp != '_' )
        {
          *bufptr = '\0';

          bufptr = buf;

          if ( bsearch(&bufptr, code_types, sizeof(code_types) / sizeof(code_types[0]), sizeof(code_types[0]), compare_keywords) )
          {
            while ( text < temp )
            {
              *style++ = 'F';
              text++;
              length--;
              col++;
            }

            text--;
            length++;
            last = 1;
            continue;
          }
          else if ( bsearch(&bufptr, code_keywords, sizeof(code_keywords) / sizeof(code_keywords[0]), sizeof(code_keywords[0]), compare_keywords))
          {
            while ( text < temp )
            {
              *style++ = 'G';
              text++;
              length--;
              col++;
            }

            text--;
            length++;
            last = 1;
            continue;
          }
        }
      }
    }
    else if ( current == 'C' && strncmp(text, "]]", 2) == 0 )
    {
      // Close a C comment...
      *style++ = current;
      *style++ = current;
      text ++;
      length --;
      current = 'A';
      col += 2;
      continue;
    }
    else if (current == 'D')
    {
      // Continuing in string...
      if ( strncmp(text, "\\\"", 2) == 0 )
      {
        // Quoted end quote...
        *style++ = current;
        *style++ = current;
        text++;
        length--;
        col += 2;
        continue;
      }
      else if ( *text == '\"'  )
      {
        // End quote...
        *style++ = current;
        col++;
        current = 'A';
        continue;
      }
    }

    // Copy style info...
    if ( current == 'A' && (*text == '{' || *text == '}') ) *style++ = 'G';
    else *style++ = current;
    col ++;

    last = isalnum(*text) || *text == '_' || *text == '.';

    if ( *text == '\n' )
    {
      // Reset column and possibly reset the style
      col = 0;
      if ( current == 'B' || current == 'E' ) current = 'A';
    }
  }
}


//
// 'style_init()' - Initialize the style buffer...
//

void style_init(Fl_Text_Buffer* textbuf)
{
  char *style = new char[textbuf->length() + 1];
  const char *text = textbuf->text();

  memset(style, 'A', textbuf->length());
  style[textbuf->length()] = '\0';

  if ( !stylebuf ) stylebuf = new Fl_Text_Buffer(textbuf->length());

  textbuf->tab_distance(2);
  stylebuf->tab_distance(2);

  style_parse(text, style, textbuf->length());

  stylebuf->text(style);
  delete[] style;
}


//
// 'style_unfinished_cb()' - Update unfinished styles.
//

void style_unfinished_cb(int, void*)
{
}


//
// 'style_update()' - Update the style buffer...
//

void style_update(
  int         pos,              // I - Position of update
  int         nInserted,        // I - Number of inserted chars
  int         nDeleted,         // I - Number of deleted chars
  int         /*nRestyled*/,    // I - Number of restyled chars
  const char* /*deletedText*/,  // I - Text that was deleted
  void*       cbArg)            // I - Callback data
{
  int   start;  // Start of text
  int   end;    // End of text
  char  last;   // Last style on line
  char* style;  // Style data
  char* text;   // Text data


  // If this is just a selection change, just unselect the style buffer...
  if ( nInserted == 0 && nDeleted == 0 )
  {
    stylebuf->unselect();
    return;
  }

  // Track changes in the text buffer...
  if ( nInserted > 0 )
  {
    // Insert characters into the style buffer...
    style = new char[nInserted + 1];
    memset(style, 'A', nInserted);
    style[nInserted] = '\0';

    stylebuf->replace(pos, pos + nDeleted, style);
    delete[] style;
  } else {
    // Just delete characters in the style buffer...
    stylebuf->remove(pos, pos + nDeleted);
  }

  // Select the area that was just updated to avoid unnecessary callbacks...
  stylebuf->select(pos, pos + nInserted - nDeleted);

  // Re-parse the changed region; we do this by parsing from the
  // beginning of the previous line of the changed region to the end of
  // the line of the changed region...  Then we check the last
  // style character and keep updating if we have a multi-line
  // comment character...
  start = ((Fl_Text_Editor *)cbArg)->buffer()->line_start(pos);
  //  if (start > 0) start = (Fl_Text_Editor *)cbArg)->buffer()->line_start(start - 1);
  end   = ((Fl_Text_Editor *)cbArg)->buffer()->line_end(pos + nInserted);
  text  = ((Fl_Text_Editor *)cbArg)->buffer()->text_range(start, end);
  style = stylebuf->text_range(start, end);
  if ( start == end ) last = 0;
  else last  = style[end - start - 1];

  //printf("start = %d, end = %d, text = \"%s\", style = \"%s\", last='%c'...\n", start, end, text, style, last);

  style_parse(text, style, end - start);

  //printf("new style = \"%s\", new last='%c'...\n", style, style[end - start - 1]);

  stylebuf->replace(start, end, style);
  ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);

  if ( start==end || last != style[end - start - 1] )
  {
    // printf("Recalculate the rest of the buffer style\n");
    // Either the user deleted some text, or the last character
    // on the line changed styles, so reparse the
    // remainder of the buffer...
    free(text);
    free(style);

    end   = ((Fl_Text_Editor *)cbArg)->buffer()->length();
    text  = ((Fl_Text_Editor *)cbArg)->buffer()->text_range(start, end);
    style = stylebuf->text_range(start, end);

    style_parse(text, style, end - start);

    stylebuf->replace(start, end, style);
    ((Fl_Text_Editor *)cbArg)->redisplay_range(start, end);
  }

  free(text);
  free(style);
}

#endif // STYLE_H_INCLUDED
