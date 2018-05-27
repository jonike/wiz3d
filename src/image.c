#include "../lib/litelibs/litegfx.h"
#include "../lib/stb_image.h"
#include "image.h"

static ltex_t* _images[NUM_IMAGES] = { NULL };
static bool_t _image_filtering = TRUE;

int image_create(int image, int width, int height)
{
  /* delete previous image if it exists */
  if ( image_exists(image) ) image_delete(image);

  /* find available image index if -1 is received */
  if ( image == -1 )
  {
    int i;
    for ( i = 0; i < NUM_IMAGES; ++i )
    {
      if ( !image_exists(i) )
      {
        image = i;
        break;
      }
    }
  }

  /* return if there are no more images available */
  if ( image == -1 ) return -1;

  /* create image */
  _images[image] = ltex_alloc(width, height, _image_filtering);

  return image;
}

int image_load(int image, const char* filename)
{
  unsigned char* buffer;
  int w, h;

  /* load buffer */
  buffer = stbi_load(filename, &w, &h, NULL, 4);
  if ( !buffer ) return -1;

  /* create image */
  image = image_create(image, w, h);
  if ( image != -1 ) ltex_setpixels(_images[image], buffer);

  /* delete buffer */
  stbi_image_free(buffer);

  return image;
}

void image_delete(int image)
{
  if ( image_exists(image) )
  {
    ltex_free(_images[image]);
    _images[image] = NULL;
  }
}

bool_t image_exists(int image)
{
  if ( image < 0 || image >= NUM_IMAGES )
  {
    return FALSE;
  }
  else
  {
    return _images[image] != NULL;
  }
}

int image_width(int image)
{
  if ( image_exists(image) )
  {
    return _images[image]->width;
  }
  else
  {
    return 0;
  }
}

int image_height(int image)
{
  if ( image_exists(image) )
  {
    return _images[image]->height;
  }
  else
  {
    return 0;
  }
}

void image_setfilter(bool_t filter)
{
  _image_filtering = filter;
}

const void* _image_ptr(int image)
{
  if ( image_exists(image) )
  {
    return _images[image];
  }
  else
  {
    return NULL;
  }
}
