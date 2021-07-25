//gcc -Wall hello6.c -o hello6 `pkg-config --cflags --libs xcb` -lX11 -lXext 
//gcc -Wall hello6.c -o hello6 -lxcb -lX11 -lXext 


#include <xcb/xcb.h>

int
main ()
{
  xcb_connection_t *c;
  xcb_screen_t     *screen;
  xcb_window_t      win;
  xcb_colormap_t    cmap

  /* Open the connection to the X server and get the first screen */
  c = xcb_connect (NULL, NULL);
  screen = xcb_setup_roots_iterator (xcb_get_setup (c)).data;

  /* We create the window win here*/

  cmap = xcb_generate_id (c);
  xcb_create_colormap (c, XCB_COLORMAP_ALLOC_NONE, cmap, win, screen->root_visual);

  return 0;
}
