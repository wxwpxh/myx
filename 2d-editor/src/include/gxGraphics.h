/**
 ** 2D Graphical Editor (2d-gx)
 **
 ** gxGraphics.h
 */
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>

#ifndef _GX_GRAPHICS_H_INC_
#define _GX_GRAPHICS_H_INC_

/* defines & macros */
#define TOLERANCE   3
#define OFFSET      5
#define HNDL_SIZE   8
#define HNDL_OFFSET (HNDL_SIZE/2)


#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/**
 **
 **
 */
typedef struct _gx_obj {
  /*
   * attributes
   */
  Pixel fg; /* foreground */
  Pixel bg; /* background */

  Pixmap fs; /* fill style */ 
  int    ls; /* line style */ 
  int    lw; /* line width */ 

  /*
   * status
   */
  Boolean selected;

  XRectangle *handles;
  int        num_handles;

  void *data; /* this varies in type for each object */

  void (*draw)  ( struct _gx_obj * );
  void (*erase) ( struct _gx_obj * );

  XRectangle *(*bounds)( struct _gx_obj * );
  Boolean    (*find)  ( struct _gx_obj *, XEvent * );

  void (*select)    ( struct _gx_obj * );
  void (*deselect)  ( struct _gx_obj * );
  void (*copy)      ( struct _gx_obj * );

  void (*move)  ( struct _gx_obj *, XEvent * );
  void (*scale) ( struct _gx_obj *, XEvent * );

  void (*action)  ( struct _gx_obj *, XEvent * );

  void (*save) ( FILE *, struct _gx_obj * );

  /*
   * link to other objects
   */
  struct _gx_obj *next; /* entire list of all objects */
  struct _gx_obj *list; /* active list of objecs */

} GXObj, *GXObjPtr;

/** 
 ** struture to hold 
 ** all that's pertinent 
 ** for drawing lines
 **/
typedef struct _gxline {

  XPoint *pts;
  int    num_pts;

} GXLine, *GXLinePtr;

typedef XArc GXArc;
typedef XArc *GXArcPtr;
 
typedef XPoint ***GXFont;
typedef int    ** GXFontP;

/**
 ** structure for holding all 
 ** thats pertient to text objects
 */
typedef struct _gxtext {
  int x, y;    /* top-left */

  int dx, dy; /* delta from original character points */
  char *text;
  int  len;

  GXFont  vpts;
  GXFont  font;   /* segment definitions */
  GXFontP fontp;  /* num segs per char   */

} GXText, *GXTextPtr;

/*
 * cursor modes
 */
typedef enum _cursor_mode {
  NORMAL_MODE = 0,
  PENCIL_MODE,
  EDIT_MODE,
  TEXT_MODE,
  MOVE_MODE,
  SCALE_MODE,
  LINE_MODE
} CursorMode;

#endif /* _GX_GRAPHICS_H_INC_ */

#ifndef GLOBAL
#define GLOBAL
#else
#undef  GLOBAL
#define GLOBAL extern
#endif

GLOBAL Widget GxStatusBar;
GLOBAL Widget GxDrawArea;
GLOBAL Pixmap GxDrawAreaBG;
GLOBAL GC     rubberGC;
GLOBAL int    GxActiveHandle;

GLOBAL GXObjPtr gxObjHeader;
GLOBAL GXObjPtr gxObjCurrent;

GLOBAL int    FixedX, FixedY;
GLOBAL int    OrigX,  OrigY;
GLOBAL int    ExntX,  ExntY;
/**
 ** end of gxGraphics.h
 */
