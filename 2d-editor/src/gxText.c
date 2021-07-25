/**
 ** 2D Graphical Editor (2d-gx)
 **
 ** gxText.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Command.h>

#include "gxGraphics.h"
#include "gxProtos.h"

#include "vfonts/simplex.h"
#include "vfonts/script_simplex.h"
#include "vfonts/triplex_bold.h"
#include "vfonts/triplex_bold_italic.h"
#include "vfonts/marker.h"

/* character spacing */
#define SPC(w) (w + 3) /* just a wee gap between chars */

static void create_text( GXObjPtr, GXTextPtr );
static GXTextPtr copy_gxtext( GXTextPtr, int, int );
static void reset_pts( GXTextPtr, int, int, int, int );

/*
 */
static void apply_scale_top( GXTextPtr text, int dx, int dy )
{
  text->y  += dy;
  text->dy += (dy * -1);
}
static void apply_scale_left( GXTextPtr text, int dx, int dy )
{
  text->x  += dx;
  text->dx += (dx * -1);
}
static void apply_scale_right( GXTextPtr text, int dx, int dy )
{
  text->dx += dx;
}
static void apply_scale_bottom( GXTextPtr text, int dx, int dy )
{
  text->dy += dy;
}

/*
 */
static void calc_apply_scale( GXTextPtr text, int dx, int dy )
{
  switch( GxActiveHandle ) {
  case 0:
    apply_scale_top ( text, dx, dy );
    apply_scale_left( text, dx, dy );
    break;

  case 1:
    apply_scale_top( text, dx, dy );
    break;

  case 2:
    apply_scale_top  ( text, dx, dy );
    apply_scale_right( text, dx, dy );
    break;

  case 3:
    apply_scale_right( text, dx, dy );
    break;

  case 4:
    apply_scale_right ( text, dx, dy );
    apply_scale_bottom( text, dx, dy );
    break;

  case 5:
    apply_scale_bottom( text, dx, dy );
    break;

  case 6:
    apply_scale_bottom( text, dx, dy );
    apply_scale_left  ( text, dx, dy );
    break;

  case 7:
    apply_scale_left( text, dx, dy );
    break;

  default:
    setStatus( "TEXT: The end is nigh!" );
  }

  /* resetting to the original points insures that we don't */
  /* compound rounding errors for points that have already  */
  /* been scaled                                            */
  reset_pts( text, text->x, text->y, text->dx, text->dy );
}

/*
 * GXDrawText
 */
static void GXDrawText( GXTextPtr text, GC gc )
{
  char *txt = text->text;
  int  c, chr, nsegs, num_pts;

  for( c = 0; c < text->len; c++, txt++ ) {
    chr = *txt - ' ';
    nsegs = 0;

    while( text->font[chr][nsegs] != NULL ) {
      num_pts = text->fontp[chr][nsegs];

      if( num_pts > 0 ) {
        XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), gc, 
                    text->vpts[c][nsegs], num_pts, CoordModeOrigin );
      }
      nsegs++;
    }
  }
}

/*
 * freeGXText
 */
static void freeGXText( GXTextPtr text_data )
{
  int       c, chr, nsegs;
  char      *text = text_data->text;
 
  for( c = 0; c < text_data->len; c++, text++ ) {
    chr = *text - ' ';

    nsegs = 0;
    while( text_data->font[chr][nsegs] != NULL ) {

      XtFree( (char *)text_data->vpts[c][nsegs] );
      nsegs++;
    }
    XtFree( (char *)text_data->vpts[c] );
  }
  XtFree( (char *)text_data->vpts );
  XtFree( (char *)text_data->text );
}

/*
 * close_dialog
 */
static void close_dialog( Widget w, XtPointer cdata, XtPointer cbs )
{
  Widget dialog = (Widget)cdata;

  if( dialog ) XtUnmanageChild( dialog );
}

/*
 * get_creation_text
 */
static char *get_creation_text( void ) 
{
  XtAppContext app;
  XEvent       event;

  Widget dialog;
  char *str = NULL;

  dialog = XtVaCreateManagedWidget( "Text Entry Box", 
                                    dialogWidgetClass, 
                                    GxDrawArea,
                                    XtNwidth,  115,
                                    XtNheight, 70,
                                    XtNlabel,  "Enter Text:",
                                    XtNvalue,  "",
                                    NULL );

  XawDialogAddButton( dialog, " Ok ", close_dialog, dialog );

  app = XtWidgetToApplicationContext( GxDrawArea );

  while( XtIsManaged(dialog)) {
    XtAppNextEvent( app, &event );
    XtDispatchEvent( &event );
  }

  str = XawDialogGetValueString( dialog );
  XtDestroyWidget( dialog );

  /*
   * look for 'illegal' characters
   */
  {
    int c, indx = 0;
    char illegal_chars[] = { '\n', 'z' };

    while( (c = (int)str[indx]) != '\0' ) {
      if( strchr( illegal_chars, c ) != NULL ) str[indx] = ' ';
      indx++;
    }

    /*
     * remove leading zeros
     */
    while( *str && *str == ' ' ) str++;
  }

  if( str && *str )
    return XtNewString(str);
  else
    return NULL;
}

/*
 * get_bounds
 */
void get_bounds( XPoint *pts, int num_pts, 
                 int *x1, int *y1, int *x2, int *y2 )
{
  int i;

  *x1 = *y1 = SHRT_MAX;
  *x2 = *y2 = 0;

  for( i = 0; i < num_pts; i++ ) {
    *x1 = min( pts[i].x, *x1 );
    *y1 = min( pts[i].y, *y1 );

    *x2 = max( pts[i].x, *x2 );
    *y2 = max( pts[i].y, *y2 );
  }
}

/*
 * get_extents
 */
static void get_extents( GXTextPtr text, 
                         int *minx, int *miny, int *maxx, int *maxy )
{
  char *txt = text->text;
  int  c, chr, nsegs, x1, y1, x2, y2;
 
  *minx = *miny = 9999;
  *maxx = *maxy = -9999;

  for( c = 0; c < text->len; c ++, txt ++ ) {
    chr = *txt - ' ';
   
    nsegs = 0; 
    while( text->font[chr][nsegs] != NULL ) {

      get_bounds( text->vpts[c][nsegs], text->fontp[chr][nsegs],
                  &x1, &y1, &x2, &y2 );

      *minx = min( x1, *minx );
      *miny = min( y1, *miny );
      *maxx = max( x2, *maxx );
      *maxy = max( y2, *maxy );

      nsegs++;
    }
  }
}

/*
 * next_char_min
 */
static int next_char_min( GXTextPtr text, char c )
{
  int i, minx = 9999, num_pts, nsegs = 0, coff = 0;

  int _c = c - ' ';

  nsegs = 0;

  while( text->font[(int)_c][nsegs] != NULL ) {
    num_pts = text->fontp[(int)_c][nsegs];

    for( i = 0; i < num_pts; i++ ) {
      minx = min(text->font[(int)_c][nsegs][i].x, minx);
    }
    nsegs++;
  }

  /* approx fixed with of a character */
  /* will be used to represent space  */
  if( c == ' ' )
    coff = 12;
  else if( minx < 0 )
    coff = abs(minx);

  return coff;
}

/*
 * reset_font_pts
 */
static void reset_font_pts( GXTextPtr text, int x, int y )
{
  char *txt = text->text;
  int i, c,                /* counters for looping           */
      chr,                 /* index to character in font set */
      nsegs,               /* number of segments for chr     */
      num_pts,             /* number of pts for given seg    */
      maxx,                /* max extention for a chr        */
      orig_x = x,          /* where this chr begins          */
      c_off  = 0;          /* offset from previous chr       */

  text->x = x;
  text->y = y;

  maxx = -9999;
  for( c = 0; c < text->len; c++, txt++ ) {
    chr = *txt - ' ';

    nsegs = 0;
    while( text->font[chr][nsegs] != NULL ) {
      num_pts = text->fontp[chr][nsegs];

      for( i = 0; i < num_pts; i++ ) {
        /*
         * reset our vpts from our font relative the origin of this
         * character applying screen scale along the way
         */
        text->vpts[c][nsegs][i].x =
          (text->font[chr][nsegs][i].x + c_off) + orig_x;
        text->vpts[c][nsegs][i].y =
          text->font[chr][nsegs][i].y + y;

        maxx = max( maxx, text->vpts[c][nsegs][i].x );
      } 
      nsegs++;
    }

    /*
     * look ahead at the width of the next character setting orig_x
     * to insure it will not overlap with the current character
     */
    if( (c + 1) < text->len )
      c_off = next_char_min( text, *(txt + 1) );

    orig_x  = SPC(maxx);
  }
}

/*
 * apply_delta
 */
static void apply_delta( GXTextPtr text, int dx, int dy )
{
  char *txt = text->text;
  int  i, c, chr, nsegs, num_pts;

  text->x += dx;
  text->y += dy;

  for( c = 0; c < text->len; c++, txt++ ) {
    chr = *txt - ' ';

    nsegs = 0;
    while( text->font[chr][nsegs] != NULL ) {
      num_pts = text->fontp[chr][nsegs];

      for( i = 0; i < num_pts; i++ ) {

        text->vpts[c][nsegs][i].x += dx;
        text->vpts[c][nsegs][i].y += dy;
      }
      nsegs++;
    }
  }
}

/*
 * apply_scale
 */
static void apply_scale( GXTextPtr text, int dx, int dy )
{
  char *txt = text->text;

  int x, y;                                  /* for transformation */
  int i, c, chr, nsegs, num_pts;             /* for traversing */
  int minx, miny, maxx, maxy, width, height; /* for scaling */

  get_extents( text, &minx, &miny, &maxx, &maxy );

  width  = maxx - minx;
  height = maxy - miny;

  x = text->x - minx;
  y = text->y - miny;

  for( c = 0; c < text->len; c++, txt++ ) {
    chr = *txt - ' ';

    nsegs = 0; 
    while( text->font[chr][nsegs] != NULL ) {
      num_pts = text->fontp[chr][nsegs];

      for( i = 0; i < num_pts; i++ ) {
        text->vpts[c][nsegs][i].x += (int)(dx *
            ((float)(text->vpts[c][nsegs][i].x - minx)/(float)width)) + x;

        text->vpts[c][nsegs][i].y += (int)(dy *
            ((float)(text->vpts[c][nsegs][i].y - miny)/(float)height)) + y;
      } 
      nsegs++;
    }
  }
}

/*
 * reset_pts
 */
static void reset_pts( GXTextPtr text, int x, int y, int dx, int dy )
{
  reset_font_pts( text, x, y );
  apply_scale( text, dx, dy );
}

/*
 * create_gxtext
 */
GXTextPtr create_gxtext( char *text, int x, int y, GXFont fnt, GXFontP fntp )
{
  GXTextPtr text_data;
  int c, chr, s, num_pts, nsegs;
 
  text_data = (void *)XtNew( GXText );

  text_data->x = x;
  text_data->y = y;

  text_data->dx = 0;
  text_data->dy = 0;

  text_data->text = XtNewString( text );
  text_data->len  = strlen( text );

  text_data->font  = fnt;
  text_data->fontp = fntp;

  text_data->vpts = (XPoint ***)
       XtMalloc( sizeof( XPoint ** ) * text_data->len );

  for( c = 0; c < text_data->len; c++, text++ ) {
    chr = *text - ' ';

    nsegs = 0;
    while( fnt[chr][nsegs] != NULL ) {
      nsegs++;
    }

    text_data->vpts[c] = (XPoint **)
      XtMalloc( sizeof( XPoint * ) * nsegs );

    for( s = 0; s < nsegs; s++ ) {
      num_pts = fntp[chr][s];

      text_data->vpts[c][s] = (XPoint *)
        XtMalloc( sizeof( XPoint ) * num_pts );
    }
  }

  reset_pts( text_data, 
             text_data->x, text_data->y, 
             text_data->dx, text_data->dy );
  return text_data;
}

/*
 * update_gxtext
 */
static GXTextPtr update_gxtext( XEvent *xe, char *str, GXTextPtr upd )
{
   GXTextPtr text = NULL;

   if( upd ) {
     reset_pts( upd, xe->xbutton.x, xe->xbutton.y, 0, 0 );
   } else {
     text = create_gxtext( str, xe->xbutton.x, xe->xbutton.y,
                           plain_simplex, plain_simplex_p );
   }

   return text;
}

/*
 * text_find
 */
static Boolean text_find( GXObjPtr obj, XEvent *event )
{
  GXTextPtr text = obj->data;

  char *txt = text->text;

  int  i, c, chr, nsegs, num_pts, found = False;

  for( c = 0; (c < text->len) && !found; c++, txt++ ) {
    chr = *txt - ' ';

    nsegs = 0;
    while( (text->font[chr][nsegs] != NULL) && !found ) {
      num_pts = text->fontp[chr][nsegs];

      for( i = 0; (i < (num_pts - 1)) && !found; i++ ) {
        found = near_segment( text->vpts[c][nsegs][i  ].x,
                              text->vpts[c][nsegs][i  ].y,
                              text->vpts[c][nsegs][i+1].x,
                              text->vpts[c][nsegs][i+1].y,
                              event->xbutton.x,
                              event->xbutton.y );
      }
      nsegs++;
    }
  }

  return found;
}

/*
 * text_move
 */
static void text_move( GXObjPtr text, XEvent *event )
{
  static int x = 0, y = 0;

  if( x && y ) {
    GXDrawText( (GXTextPtr)text->data, rubberGC );
  } else {
    (*text->erase)( text );

    x = event ? event->xbutton.x : 0;
    y = event ? event->xbutton.y : 0;
  }

  if( event ) {
    apply_delta( text->data,
                 event->xbutton.x - x,
                 event->xbutton.y - y );

    GXDrawText( text->data, rubberGC );
    x = event->xbutton.x;
    y = event->xbutton.y;
  } else {
    x = 0;
    y = 0;
  }
}

/*
 * text_copy
 */
static void text_copy( GXObjPtr obj )
{
  GXTextPtr temp_data;

  (*obj->deselect)( obj );

  temp_data = copy_gxtext( (GXTextPtr)obj->data, OFFSET, OFFSET );
  create_text( NULL, temp_data );

  freeGXText(temp_data);
  XtFree((char *)temp_data);
}

/*
 * text_scale
 */
static void text_scale( GXObjPtr text, XEvent *event )
{
  static int x = 0, y = 0;

  if( x && y ) {
    GXDrawText( text->data, rubberGC );

  } else {
    (*text->erase)( text );

    x = event ? event->xbutton.x : 0;
    y = event ? event->xbutton.y : 0;
  }

  if( event ) {
    calc_apply_scale( text->data, 
                 event->xbutton.x - x, event->xbutton.y - y );
    x = event->xbutton.x;
    y = event->xbutton.y;

    GXDrawText( text->data, rubberGC );
  } else {
    x = 0;
    y = 0;
  }
}

/*
 * draw_erase
 */
static void draw_erase( GXObjPtr text, Boolean tile )
{
  GC gc;

  gc = gx_allocate_gc( text, tile );
  GXDrawText( (GXTextPtr)text->data, gc );

  XtReleaseGC( GxDrawArea, gc );
}
 
/*
 * text_draw
 */
static void text_draw( GXObjPtr obj )
{
  draw_erase( obj, False );
}

/* 
 * text_erase
 */
static void text_erase( GXObjPtr obj )
{
  draw_erase( obj, True );
}
 
/*
 * copy_gxtext
 */
static GXTextPtr copy_gxtext( GXTextPtr text, int off_x, int off_y )
{
  GXTextPtr text_data;
  int       i, c, chr, num_pts, nsegs;
  char      *txt;
 
  text_data =
    create_gxtext( text->text, text->x + off_x, text->y + off_y,
                   text->font, text->fontp );

  text_data->dx = text->dx;
  text_data->dy = text->dy;

  txt = text_data->text;
  for( c = 0; c < text_data->len; c++, txt++ ) {
    chr = *txt - ' ';

    nsegs = 0;
    while( text_data->font[chr][nsegs] != NULL ) {

      num_pts = text_data->fontp[chr][nsegs];

      for( i = 0; i < num_pts; i++ ) {

        text_data->vpts[c][nsegs][i].x =
          text->vpts[c][nsegs][i].x + off_x;
        text_data->vpts[c][nsegs][i].y =
          text->vpts[c][nsegs][i].y + off_y;
      }
      nsegs++;
    }
  }

  return text_data;
}

/*
 * text_bounding_handles
 */
static void text_bounding_handles( GXObjPtr gx_text )
{
  GXTextPtr text = gx_text->data;
   
  int i, minx, miny, maxx, maxy, width, height;
 
  gx_text->handles     = (XRectangle *)XtMalloc( sizeof(XRectangle) * 8 );
  gx_text->num_handles = 8;
 
  if( gx_text->handles == NULL ) {
    perror( "Alloc failed for text handles" );
    gx_text->num_handles = 0;
    return;
  }

  for( i = 0; i < 8; i++ ) {
    gx_text->handles[i].width  = HNDL_SIZE;
    gx_text->handles[i].height = HNDL_SIZE;
  }

  get_extents(text, &minx, &miny, &maxx, &maxy);
  width = maxx - minx; height = maxy - miny;
 
  gx_text->handles[0].x = minx - HNDL_OFFSET - HNDL_SIZE;
  gx_text->handles[0].y = miny - HNDL_OFFSET - HNDL_SIZE;
 
  gx_text->handles[1].x = minx + (width/2) - HNDL_OFFSET;
  gx_text->handles[1].y = miny - HNDL_SIZE - HNDL_OFFSET;
 
  gx_text->handles[2].x = maxx + HNDL_OFFSET;
  gx_text->handles[2].y = miny - HNDL_SIZE - HNDL_OFFSET;
 
  gx_text->handles[3].x = maxx + HNDL_OFFSET;
  gx_text->handles[3].y = miny + (height/2) - HNDL_OFFSET;
 
  gx_text->handles[4].x = maxx + HNDL_OFFSET;
  gx_text->handles[4].y = maxy + HNDL_OFFSET;
 
  gx_text->handles[5].x = minx + (width/2) - HNDL_OFFSET;
  gx_text->handles[5].y = maxy + HNDL_OFFSET;
 
  gx_text->handles[6].x = minx - HNDL_OFFSET - HNDL_SIZE;
  gx_text->handles[6].y = maxy + HNDL_OFFSET;
 
  gx_text->handles[7].x = minx - HNDL_OFFSET - HNDL_SIZE;
  gx_text->handles[7].y = miny + (height/2) - HNDL_OFFSET;
}

/* 
 * text_select
 */
static void text_select( GXObjPtr text )
{
  text_bounding_handles( text );
  gx_draw_handles( text );
}

/* 
 * text_deselect
 */
static void text_deselect( GXObjPtr text )
{
  if( text->handles && text->num_handles > 0 ) {
    gx_erase_handles( text );

    XtFree((char *)text->handles );

    text->handles = NULL;
    text->num_handles = 0;
  }
}

/*
 * gxLoadText
 */
void gxTextLoad( FILE *fp, GXObjPtr obj )
{
    char text[256], *ptr;

    GXTextPtr data;
    int x, y;

    fgets( text, 256, fp );
    if( (ptr = strchr( text, '\n' )) != NULL ) {
        *ptr = '\0';
    }
    fscanf( fp, "%d %d\n", &x, &y );

    data = create_gxtext( text, x, y,
                          plain_simplex, plain_simplex_p );
    
    create_text( obj, data );
}

/*
 * text_save
 */
static void text_save( FILE *fp, GXObjPtr obj )
{
    GXTextPtr text = (GXTextPtr)obj->data;

    fprintf( fp, "TEXT [ str x y ]\n" );
    fprintf( fp, "%s\n %d %d\n", text->text, text->x, text->y );
}

/*
 * create_text
 */
static void create_text( GXObjPtr _obj, GXTextPtr text )
{
  /*
   * create the template object
   */
  GXObjPtr obj = _obj;

  if( obj == NULL ) {
      obj = gx_create_obj();
  }

  obj->data = copy_gxtext( text, 0, 0 );

  obj->draw     = text_draw;
  obj->erase    = text_erase;
  obj->find     = text_find;
  obj->move     = text_move;
  obj->scale    = text_scale;
  obj->copy     = text_copy;
  obj->select   = text_select;
  obj->deselect = text_deselect;

  obj->save = text_save;

  gx_add_obj( obj );
}

/*
 * place_creation_text
 */
static void place_creation_text( XEvent *event, char **_text )
{
  static GXTextPtr rubber_text = NULL;
  char *text = NULL;

  if( event == NULL ) {
    rubber_text = NULL;
  } else {
    if( _text ) text = *_text;

    if( rubber_text ) {
      GXDrawText( rubber_text, rubberGC );
    }
   
    switch( event->type ) {
      case ButtonPress:
        if( rubber_text && text ) {
          create_text( NULL, rubber_text );
          gx_refresh();

          freeGXText( rubber_text );
          rubber_text = NULL;
       
          free( text ); 
          *_text = NULL;

          set_cursor( NORMAL_MODE );
        }
        break;

      case MotionNotify:
        if( text ) {
          if( rubber_text == NULL ) {
            rubber_text = update_gxtext( event, text, NULL );
          } else {
            (void)update_gxtext( event, text, rubber_text );

          }
          GXDrawText( rubber_text, rubberGC );
        }
        break;
    }
  }
}

/*
 * gx_text
 */
void gx_text( XEvent *event )
{
  static char *creation_text = NULL;

  if( event == NULL ) {
    creation_text = NULL;
    place_creation_text( NULL, NULL );

    /*
     * we have to prompt for a string
     */
    creation_text = get_creation_text();

    if( creation_text != NULL )
      set_cursor( TEXT_MODE );

  } else {

    /* 
     * If we have a string, place it!
     */
    if( creation_text ) {
      /* adjust for the hotspot in our cursor */
      event->xbutton.y -= 10; 
      event->xbutton.x += 10;

      place_creation_text( event, &creation_text );
    }
  }
}

/**
 ** end of gxText.c
 */
