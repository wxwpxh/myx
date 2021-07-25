/**
 ** 2D Graphical Editor (2d-gx)
 ** 
 ** gxGx.c
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Command.h>

#include <X11/Xaw/Label.h>

#include "gxGraphics.h"
#include "gxProtos.h"

static void (*draw_mgr_func)( XEvent * ) = NULL;
static void deactivate_objs( void );

/*
 * gx_exit
 */
void gx_exit( Widget w, XtPointer cd, XtPointer cbs )
{
  exit(0);
}

/*
 * gx_copy
 */
void gx_copy( void )
{
  if( gxObjCurrent ) {
    (*gxObjCurrent->copy)( gxObjCurrent );
    gx_refresh();
  } else
    setStatus( "Select an object to copy!" );
}

/*
 * gx_delete
 */
void gx_delete( void )
{
  GXObjPtr gx_objs;
  Boolean  found;

  if( gxObjCurrent ) {

    (*gxObjCurrent->deselect)( gxObjCurrent );
    (*gxObjCurrent->erase)( gxObjCurrent );

    if( gxObjCurrent->data ) {
      XtFree((char *)gxObjCurrent->data );
      gxObjCurrent->data = NULL;
    }

    found = False;
    gx_objs = gxObjHeader;

    /*
     * find this object in the list and remove it
     */
    while( gx_objs->next && (!found)) {

      /* we want the reference just before us... */
      found = (gx_objs->next == gxObjCurrent );

      /* ...now instead of pointing at us,
       * point to hwat we point to
       */
      if( found ) {
        gx_objs->next = gxObjCurrent->next;
        gxObjCurrent->next = NULL;
      } else {
        gx_objs = gx_objs->next;
      }
    }
  
    /*
     * if we didn't find ourselves in the list, we are the list!
     */
    if( found == False ) {
      if( gxObjHeader == gxObjCurrent )
        gxObjHeader = gxObjCurrent->next;
      else
        setStatus( "Panic: object to delete not found list!" );
    }

    /* do some house cleaning */
    XtFree((char *)gxObjCurrent );
    gxObjCurrent = NULL;
    
    /* redraw remaining objects (if any) */
    if( gxObjHeader ) gx_refresh();
  } else 
    setStatus( "Select an object to delete!" );
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
 * getFileName
 */
static char *gxGetFileName( void ) 
{
  XtAppContext app;
  XEvent       event;

  Widget dialog;
  char *str = NULL;

  dialog = XtVaCreateManagedWidget( "Filename...",
                                    dialogWidgetClass, 
                                    GxDrawArea,
                                    XtNwidth,  115,
                                    XtNheight, 70,
                                    XtNlabel,  "Enter File:",
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
      if( strchr( illegal_chars, c ) != NULL ) {
        str[indx] = '\0';
        break;
      }
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

static void gxSaveCommon( FILE *fp, GXObjPtr obj )
{
   fprintf( fp, "OBJ - fg bg ls lw\n" );
   fprintf( fp, " %ld %ld %d %d\n",  
            obj->fg, obj->bg, obj->ls, obj->lw );
}

static void gxSaveObjs( FILE *fp, GXObjPtr obj )
{
    if( obj ) {
        gxSaveCommon( fp, obj );
        (*obj->save)( fp, obj );

        gxSaveObjs( fp, obj->next );
    }
}

/*
 * gx_save
 */
void gx_save( void )
{
    FILE *fp;

    char *filename = gxGetFileName();

    setStatus( "Saving objects..." );
    if( filename ) {
        fp = fopen( filename, "w+" );

        if( fp == NULL ) {
          perror( "Failed to open file: " );
        } else {
            gxSaveObjs( fp, gxObjHeader );
            fclose( fp );

        }
    }
    setStatus( "Saving objects... done!" );
}

/*
 * gxLoadCommon
 */
static void gxLoadCommon( FILE *fp, GXObjPtr obj )
{
   fscanf( fp, " %ld %ld %d %d\n",  
            &obj->fg, &obj->bg, &obj->ls, &obj->lw );
}

/*
 * gxLoadObjs
 */
static void gxLoadObjs( FILE *fp )
{
    char objForm[128];
    GXObjPtr obj;
 
    while( fgets( objForm, 128, fp ) != NULL ) {
        obj = gx_create_obj();
        gxLoadCommon( fp, obj );        

	fgets( objForm, 128, fp );
        switch( *objForm ) {
          case 'A':
            gxArcLoad( fp, obj );
            break;

          case 'L':
            gxLineLoad( fp, obj );
            break;

          case 'T':
            gxTextLoad( fp, obj );
            break;
        }
    }
}

/*
 * gx_load
 */
void gx_load( void )
{
    FILE *fp;

    char *filename = gxGetFileName();

    setStatus( "Loading objects..." );
    if( filename ) {
      fp = fopen( filename, "r" );
      
      if( fp == NULL ) {
        perror( "Failed to open file: " );
      } else {
          gxLoadObjs( fp );
          fclose( fp );
      }
    }
    setStatus( "Loading object... done!" );
}

/*
 * gx_export
 */
void gx_export( void )
{
}

static void get_image( Widget w, XImage **xi, 
                       Dimension *width, Dimension *height )
{
  Window     window;
  int        max_w, max_h, x = 0, y = 0;
  
  XtVaGetValues( w, 
                 XtNwidth,       width, 
                 XtNheight,      height, 
                 NULL );
  
  *width  -= 2; *height -= 2;
  max_w = *width; max_h = *height;
  
  window = XtWindow( w );
  
  /* insure an even number of rows, postscript demands it */
  *height -= ((*height)%2);
   
  if( *width > 1 && *height > 1 ) { 

      *xi = XGetImage( XtDisplay(w), window, 
                       x, y, *width, *height, AllPlanes, ZPixmap );
  } 
}

#define MAXCOLORMAPSIZE 65536
typedef unsigned char byte;
static byte *set_color_data( Widget w, XImage *image, 
                             int width, int height, XColor *colors, 
                             byte *Red, byte *Green, byte *Blue, int *n )
{
  Boolean mapcols[MAXCOLORMAPSIZE],
          colused[MAXCOLORMAPSIZE];
  
  Display *dsp = XtDisplay(w);
  int     x = 0, i;
  
  byte *dptr, *data, *iptr = (byte *) image->data;
  
  dptr = data = (unsigned char *) XtMalloc( height * width );
  memset( colused, False, sizeof( Boolean ) *  MAXCOLORMAPSIZE );
  
  setStatus( "Gathering color data... please wait" );
  for ( i = 0; i < MAXCOLORMAPSIZE; i++) {
    colors[i].pixel = i;
    colors[i].flags = DoRed | DoGreen | DoBlue;

    XQueryColor( dsp, DefaultColormap(dsp, 0), &colors[i] );
  }
  
  setStatus( "Determing color usage ..." );
  for (i = 0; i < image->bytes_per_line*height; i++, iptr++) {

    if (x >= image->bytes_per_line)
      x = 0;

    if (x < width) {
      colused[*iptr] = True;   /* mark this color as used */
      *dptr++ = *iptr;
    }
    x++;
  }

  setStatus( "Processing color data ..." );
  for (i = 0; i < MAXCOLORMAPSIZE; i++) {
    if( colused[i] ) {
      
      mapcols[i] = *n;
      
      Red[*n]   = colors[i].red   >> 8;
      Green[*n] = colors[i].green >> 8;
      Blue[*n]  = colors[i].blue  >> 8;

      (*n)++;
    }
  }
  
  setStatus( "Transfering image colors..." );
  dptr = data;
  for (i = 0; i < width*height; i++) {
    *dptr = mapcols[*dptr];
    dptr++;
  }
  
  return data;
}

static void write_ps_data( FILE *fp, XImage *xi, 
                           XColor *cm, Dimension w, Dimension h )
{
  int    rows, cols, cell;
  
  for ( rows = 0; rows < h; rows++ ) {
    for( cols = 0; cols < w; cols++ ) {
      
      cell = XGetPixel( xi, rows, cols );
      
      fprintf( fp, "%2.2x", cm[cell].red   / 256 );
      fprintf( fp, "%2.2x", cm[cell].green / 256 );
      fprintf( fp, "%2.2x", cm[cell].blue  / 256 );
      
    }
    fprintf( fp, "\n" );
  }
}

static void write_ps_color_conv( FILE *fp )
{
  int i = 0;
  
  static char *ColorConvStr[] =  {
    "% define colorimage if it is not defined \n",
    "/colorimage where   % do we know about colorimage? \n",
    "  { pop }           % yes: pop off the dict returned\n",
    "  {                 % no:  define one\n \n",
    "    /colortogray {  % define an RGB->I function \n",
    "      /rgbdata exch store    % call input rgbdata \n",
    "      rgbdata length 3 idiv \n",
    "      /npixls exch store \n",
    "      /rgbindx 0 store \n",
    "      0 1 npixls 1 sub { \n",
    "        grays exch \n",
    "        rgbdata rgbindx       get 20 mul    % Red \n",
    "        rgbdata rgbindx 1 add get 32 mul    % Green \n",
    "        rgbdata rgbindx 2 add get 12 mul    % Blue \n",
    "        add add 64 idiv       \n",
    "        put \n",
    "        /rgbindx rgbindx 3 add store  \n",
    "     } for \n",
    "      grays 0 npixls getinterval \n",
    "    } bind def \n",
    "                                                    \n",
    "    % Utility procedure for colorimage operator. \n",
    "    % This procedure takes two procedures off the \n",
    "    % stack and merges them into a single procedure.\n",
    "                                                    \n", 
    "    /mergeprocs { % def \n",
    "      dup length \n",
    "      3 -1 roll \n",
    "      dup \n",
    "      length \n",
    "      dup \n",
    "      5 1 roll \n",
    "      3 -1 roll \n",
    "      add \n",
    "      array cvx \n",
    "      dup \n",
    "      3 -1 roll \n",
    "      0 exch \n",
    "      putinterval \n",
    "      dup \n",
    "      4 2 roll \n",
    "      putinterval \n",
    "    } bind def \n",
    "                                            \n",
    "    /colorimage { % def \n",
    "      pop pop     % remove false 3 operands \n",
    "      {colortogray} mergeprocs \n",
    "      image \n",
    "    } bind def \n",
    "  } ifelse        % end of false case\n \n",
    NULL };
  
  while( ColorConvStr[i] ) {
    fprintf( fp, "%s", ColorConvStr[i++] );
  }
}

static void write_ps_prolog( FILE *fp, int w, int h )
{
  time_t btime;
  time( &btime );
    
  fprintf(fp, "%%!PS-Adobe-2.0 EPSF-2.0\n"         );
  fprintf(fp, "%%%%Creator: J. R. Brown\n"         );
  fprintf(fp, "%%%%Title: 2D Graphic Editor\n"     );
  fprintf(fp, "%%%%CreationDate: %s", ctime(&btime)); 
  fprintf(fp, "%%%%Pages: 1\n");
  fprintf(fp, "%%%%BoundingBox: 0 0 %d %d\n", w, h );
  fprintf(fp, "%%%%EndComments\n");
  fprintf(fp, "%%%%EndProlog\n\n");
  fprintf(fp, "%%%%Page: 1 1\n\n");
}


static void write_ps( FILE *fp, XImage *xi, 
                      XColor *cm, Dimension h, Dimension w )
{
  write_ps_prolog( fp, w, h );

  fprintf(fp, "%% remember original state\n/saveorig save def\n\n" );
  
  fprintf(fp, "%% define string to hold scanline's worth of data\n");
  fprintf(fp, "/imgstr %d string def\n\n", w );
  
  fprintf(fp, "%% build a temporary dictionary\n20 dict begin\n\n" );
  
  fprintf(fp, "%% define space for color conversion\n");
  fprintf(fp, "/grays %d string def %% space for gray scale line\n", w*3);
  fprintf(fp, "/npixls 0 def\n/rgbindx 0 def\n\n" );
  
  fprintf(fp, "%% corner of image\n%d %d translate\n\n", 25, 25);
  
  fprintf(fp, "%% size of image on paper\n%d %d scale\n\n", 
  w > 550 ? 550 : w,
  h > 740 ? 740 : h ); /* could be changed for scaling */
  
  write_ps_color_conv( fp );

  fprintf( fp, "%d %d 8\n", w, h );
  fprintf( fp, "[%d 0 0 %d 0 0]\n", w, h );
  
  fprintf( fp, "{ currentfile\n\timgstr readhexstring pop }\n");
  fprintf( fp, "false 3 colorimage\n\n");
  
  write_ps_data( fp, xi, cm, w, h );

  fprintf( fp, "showpage\n\nend\n\nsaveorig restore\n\n");
  fprintf( fp, "%%%%Trailer\n\n%% End-of-file\n");
}

static void doPrintTo( char *datafile, char *printTo )
{
  struct stat sbuf;
  char cmd[128], lpDev[128];

  FILE *fp = NULL;

  if( printTo ) {
    sprintf( lpDev, "/dev/%s", printTo );

    if( stat( lpDev, &sbuf ) < 0 ) { 
      sprintf( cmd, "mv %s %s", datafile, printTo );
    } else {
       sprintf( cmd, 
                "unalias lpr;lpr -c -d %s %s;rm %s", 
                printTo, datafile, datafile );
    }
  } else {
      sprintf( cmd, 
               "unalias lp;lp -c %s;rm %s", 
               datafile, datafile );
  }
    
  if((fp = popen(cmd, "r")) != NULL) {
    char buf[128];
    fgets( buf, sizeof( buf ), fp );
    pclose( fp );
  } 

  setStatus( "Printing complete!" );
}

/*
 * gx_print
 */
void gx_print( void )
{
  char *_filename = "/tmp/gx-print-data.ps";

  XImage *xi = NULL;
  FILE   *fp = NULL;

  Dimension    width, height;
  int          numcols = 0;
  
  byte   *data;
  byte   Red[MAXCOLORMAPSIZE], 
         Green[MAXCOLORMAPSIZE], 
         Blue[MAXCOLORMAPSIZE];
  XColor cm[MAXCOLORMAPSIZE];
  
  fp = fopen( _filename, "w+" );

  if( fp ) {
    char *filename = gxGetFileName();

    get_image( GxDrawArea, &xi, &width, &height );
    write_ps_prolog( fp, width, height );

    if( xi ) {
      data = set_color_data( GxDrawArea, xi, width, height, 
                             cm, Red, Green, Blue, &numcols );
      
      write_ps( fp, xi, cm, width, height );

      XtFree( (char *)data );
      XDestroyImage( xi );

      fclose( fp );
    }
    doPrintTo( _filename, filename );
  }
}

/*
 * setStatus
 */
void setStatus( char *message )
{
  XtVaSetValues( GxStatusBar, XtNlabel, message, NULL );
}

/*
 * draw_manager
 */
void draw_manager( Widget w, XtPointer cd, XtPointer cbs )
{
  void (*draw_func)( XEvent * ) = (void (*)(XEvent *))cd;

  /* if anything is selected, deselected with a new draw request */
  deactivate_objs();

  if( draw_func != NULL ) (*draw_func)( NULL );
  draw_mgr_func = draw_func;
}

/*
 * ctrl_manager
 */
void ctrl_manager( Widget w, XtPointer cd, XtPointer cbs )
{
  void (*ctrl_func)( void ) = (void(*)(void))cd;
  if( ctrl_func != NULL ) ctrl_func();
}

/*
 * exposureEventProc
 */
void exposureEventProc( Widget w, XtPointer cd, XEvent *event, Boolean flag )
{
  gx_refresh();
}

/*
 * gx_refresh
 */
void gx_refresh( void )
{
  GXObjPtr obj = gxObjHeader;

  while( obj ) {
    (*obj->draw)( obj );
    gx_draw_handles( obj );
    obj = obj->next;
  }
}

/*
 * parse_all_objects
 */
static void parse_all_objects( GXObjPtr obj, XEvent *event, GXObjPtr *gx_obj )
{
  if( obj && (*gx_obj == NULL)  ) {
    if( (*obj->find)( obj, event ) ) {
      *gx_obj = obj;
      setStatus( "Object found..." );
    } else {

      setStatus( "No objects found..." );
      parse_all_objects( obj->next, event, gx_obj );
    }
  }
}

/*
 * gx_erase_handles
 */
void gx_erase_handles( GXObjPtr obj )
{
  GC gc;

  if( obj && obj->handles && obj->num_handles > 0 ) {
    gc = gx_allocate_gc( obj, True ); /* we want to tile */
  
    XFillRectangles( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), gc,
                       obj->handles, obj->num_handles );

    XtReleaseGC( GxDrawArea, gc );
  }
}

/*
 * gx_draw_handles
 */
void gx_draw_handles( GXObjPtr obj )
{
  static unsigned char mask_bits[8][HNDL_SIZE]  = {
   {0x7f, 0x3f, 0x1f, 0x1f, 0x3f, 0x73, 0xe1, 0xc0},   /* up left  */
   {0x18, 0x3c, 0x7e, 0xff, 0x18, 0x18, 0x18, 0x00},   /* up       */
   {0xfe, 0xfc, 0xf8, 0xf8, 0xfc, 0xce, 0x87, 0x03},   /* up right */
   {0x10, 0x30, 0x70, 0xff, 0xff, 0x70, 0x30, 0x10},   /* right    */
   {0x03, 0x87, 0xce, 0xfc, 0xf8, 0xf8, 0xfc, 0xfe},   /* dwn right*/
   {0x00, 0x18, 0x18, 0x18, 0xff, 0x7e, 0x3c, 0x18},   /* down     */
   {0xc0, 0xe1, 0x73, 0x3f, 0x1f, 0x1f, 0x3f, 0x7f},   /* dwn left */
   {0x08, 0x0c, 0x0e, 0xff, 0xff, 0x0e, 0x0c, 0x08} }; /* left     */

  static Boolean masks_created = False;
  static Pixmap  masks[8];

  GC  gc;
  int i;

  if( masks_created == False ) {
    for( i = 0; i < 8; i++ )
      masks[i] =
        XCreatePixmapFromBitmapData( XtDisplay(GxDrawArea),
                                     XtWindow(GxDrawArea),
                                     (char *)mask_bits[i],
                                     HNDL_SIZE, HNDL_SIZE, 1, 0, 1 );

    masks_created = True;
  }

  if( obj && obj->handles && obj->num_handles > 0 ) {
    gc = gx_allocate_gc( obj, False );
    XSetFillStyle( XtDisplay(GxDrawArea), gc, FillSolid );

    if( obj->num_handles != 8 ) {
      fprintf( stderr, 
               "Don't know how to draw objects with %d handles\n", 
               obj->num_handles );
    } else {
      for( i = 0; i < 8; i++ ) {
        XSetClipOrigin( XtDisplay(GxDrawArea), gc,
                        obj->handles[i].x, obj->handles[i].y );
        XSetClipMask( XtDisplay(GxDrawArea), gc, masks[i] );

        XFillRectangle( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), gc,
                        obj->handles[i].x,
                        obj->handles[i].y,
                        obj->handles[i].width,
                        obj->handles[i].height );
      }
    }
    XtReleaseGC(GxDrawArea, gc);
  }
}

/*
 * deactivate_obj
 */
static void deactivate_objs( void )
{
  GXObjPtr obj;

  while( gxObjCurrent ) {
    obj = gxObjCurrent;

    (*obj->deselect)( obj );

    
    gxObjCurrent = obj->list;
    obj->list = NULL;
  }
}

/*
 * activate_obj
 */
static void activate_obj( GXObjPtr obj )
{
  if( gxObjCurrent ) deactivate_objs();

  /* insure we have an object */
  if( obj ) {
    gxObjCurrent = obj;
    (*obj->select)( obj );
  }
}

/*
 * find_graphic
 */
static void find_graphic( XEvent *event )
{
  GXObjPtr gx_obj = NULL;

  if( event->type == ButtonPress ) {
    parse_all_objects( gxObjHeader, event, &gx_obj );

    if( gx_obj ) {
      /* set this object as selected */
      activate_obj( gx_obj );

    } else {
      /* deselect any currently selected */
      deactivate_objs();
    }
  }
}

/*
 * find_handle
 */
static 
void find_handle( GXObjPtr gx_obj, XEvent *xe, Boolean *found, int *indx )
{
  int i;

  *found = False;
 
  if( gx_obj && gx_obj->handles && !(*found)) {
    for( i = 0; (i < gx_obj->num_handles) && !(*found); i++ ) {
      if( ( xe->xbutton.x > gx_obj->handles[i].x )          &&
          ( xe->xbutton.x < gx_obj->handles[i].x + HNDL_SIZE) &&
          ( xe->xbutton.y > gx_obj->handles[i].y )          &&
          ( xe->xbutton.y < gx_obj->handles[i].y + HNDL_SIZE)) {
        *indx  = i;
        *found = True;
      }
    }
  }
}

/*
 * buttonpress_update
 */
static void buttonpress_update( GXObjPtr obj, XEvent *event )
{
  Boolean  found = False;
  GXObjPtr e_obj = NULL;
  int      indx  = 0;

  if( obj->action == NULL ) {
    FixedX = event->xbutton.x;
    FixedY = event->xbutton.y;

    find_handle( obj, event, &found, &indx );

    if( found ) {
      obj->action = obj->scale;

      set_cursor( SCALE_MODE );
      GxActiveHandle = indx;

    } else {
      parse_all_objects( obj, event, &e_obj );
      if( e_obj == obj ) {
        obj->action = obj->move;
        set_cursor( MOVE_MODE );
      } else {
        /* first no handles was selected.. then no object..
         * we are don't updating this object
         */
        deactivate_objs();
      }
    }
  }

  if( obj->action ) {
    (*obj->deselect)( obj );
    (*obj->action)( obj, event );
  }
}

/*
 * buttonrelease_update
 */
static void buttonrelease_update( GXObjPtr obj, XEvent *event )
{
  /*
   * reset the update function
   */
  if( obj->action ) {
    (*obj->action)( obj, event );
    (*obj->action)( obj, NULL );

    obj->action = NULL;
    (*obj->select)( obj );
    gx_refresh();
  }

  set_cursor( NORMAL_MODE );
}

/*
 * motionnotify_update
 */
static void motionnotify_update( GXObjPtr obj, XEvent *event )
{
  if( obj->action ) {
    (*obj->action)( obj, event );
  }
}

/*
 * update_obj
 */
static void update_obj( GXObjPtr obj, XEvent *event )
{
  switch( event->type ) {
  case ButtonPress:
    buttonpress_update( obj, event );
    break;

  case ButtonRelease:
    buttonrelease_update( obj, event );
    break;

  case MotionNotify:
    motionnotify_update( obj, event );
    break;
  }
}


/*
 * process_event
 */
static void process_event( XEvent *xe )
{
  if( gxObjCurrent == NULL ) {
    if( xe && xe->type == ButtonPress ) {
      find_graphic( xe );
    }
  } else {
    /* update the object */
    update_obj( gxObjCurrent, xe );
  }
}

/*
 * drawAreaEventProc
 */
void drawAreaEventProc( Widget w, XtPointer cd, XEvent *event, Boolean flag )
{
  if( draw_mgr_func != NULL ) {
    (*draw_mgr_func)( event );
  } else {
    process_event( event );
  }
}

/*
 *
 */
static void null_func( void )
{
  printf( "Warning: null function called!\n" );
}

/*
 * create_obj
 */
GXObjPtr gx_create_obj( void )
{
  GXObjPtr gx_obj = XtNew( GXObj );

  gx_obj->fs = None;
  gx_obj->ls = LineSolid;
  gx_obj->lw = 1;

  gx_obj->bg = WhitePixelOfScreen(XtScreen(GxDrawArea));
  gx_obj->fg = BlackPixelOfScreen(XtScreen(GxDrawArea));

  gx_obj->handles     = NULL;
  gx_obj->num_handles = 0;

  gx_obj->data = NULL;

  gx_obj->draw     = (void    (*)())null_func;
  gx_obj->erase    = (void    (*)())null_func;
  gx_obj->find     = (Boolean (*)())null_func;
  gx_obj->select   = (void    (*)())null_func;
  gx_obj->deselect = (void    (*)())null_func;
  gx_obj->move     = (void    (*)())null_func;
  gx_obj->scale    = (void    (*)())null_func;
  gx_obj->copy     = (void    (*)())null_func;

  gx_obj->save     = (void    (*)())null_func;

  gx_obj->action   = NULL;
  gx_obj->selected = False;

  gx_obj->next = NULL;
  gx_obj->list = NULL;

  /* reset the draw_mgr_func so   */
  /* future events are applied to */
  /* existing objects             */
  draw_mgr_func = NULL;

  return gx_obj;
}

/*
 * gx_add_obj
 */
void gx_add_obj( GXObjPtr obj )
{
  GXObjPtr gx_obj;

  if( gxObjHeader == NULL ) {
    gxObjHeader = obj;
  } else {
    gx_obj = gxObjHeader;

    /* find the end of the object list */
    while( gx_obj->next != NULL ) {
      gx_obj = gx_obj->next;
    }

    /*
     * add the new object to the end of our list
     */
    gx_obj->next = obj;
  }
}

/*
 * gx_allocate_gc
 */
GC gx_allocate_gc( GXObjPtr obj, Boolean tile )
{
  GC gc;

  XGCValues values;
  XtGCMask  mask = 0L;

  values.foreground = obj->fg;
  mask |= GCForeground;
 
  values.background = obj->bg;
  mask |= GCBackground;

  values.line_width = obj->lw;
  mask |= GCLineWidth;

  values.line_style = obj->ls;
  mask |= GCLineStyle;
  
  if( tile ) {
    values.tile = GxDrawAreaBG;
    mask |= GCTile;
  
    values.fill_style = FillTiled;
    mask |= GCFillStyle;
  }

  values.function = GXcopy;
  mask |= GCFunction;

  gc = XtAllocateGC( GxDrawArea, 0, mask, &values, mask, 0 );
  return gc;
}

/**
 ** end of gxGx.c
 */
