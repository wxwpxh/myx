/** 
 ** 2D Graphical Editor (2d-gx)
 **
 ** gxProtos.h
 */
#include <X11/Intrinsic.h>
#include "gxIcons.h"

#ifndef EXTERN
#define EXTERN
#else
#undef  EXTERN
#define EXTERN extern
#endif

/* 
 * Creations routines in gxGraphics.c 
 */
EXTERN Widget create_canvas ( Widget );
EXTERN void   create_status ( Widget, Widget );
EXTERN void   create_buttons( Widget );

EXTERN void   drawAreaEventProc( Widget, XtPointer, XEvent *, Boolean );
EXTERN void   exposureEventProc( Widget, XtPointer, XEvent *, Boolean );

/*
 * obj funcs
 */
EXTERN GXObjPtr gx_create_obj( void );
EXTERN void     gx_add_obj   ( GXObjPtr );
EXTERN void     gx_refresh   ( void );

EXTERN GC gx_allocate_gc( GXObjPtr, Boolean );

/* 
 * Drawing functions used in GxIcons.h 
 */
EXTERN void gx_line( XEvent * );
EXTERN void gx_pencil( XEvent * );
EXTERN void gx_arc( XEvent * );
EXTERN void gx_box( XEvent * );
EXTERN void gx_arrow( XEvent * );
EXTERN void gx_text( XEvent * );

/* 
 * Control functions used in GxIcons.h
 */
EXTERN void gx_copy( void );
EXTERN void gx_delete( void );
EXTERN void gx_save( void );
EXTERN void gx_load( void );
EXTERN void gx_export( void );
EXTERN void gx_print( void );
EXTERN void gx_exit( Widget, XtPointer, XtPointer );

/*
 * Utilities in gxGx.c
 */
EXTERN void setStatus( char * );
EXTERN void draw_manager( Widget, XtPointer, XtPointer );
EXTERN void ctrl_manager( Widget, XtPointer, XtPointer );
EXTERN void initializeGX( void );
EXTERN void set_cursor( CursorMode );

EXTERN Boolean near_segment( int, int, int, int, int, int );
EXTERN void    toggle_handles( GXObjPtr, Boolean, void (*)(GXObjPtr));
EXTERN void    gx_draw_handles( GXObjPtr );
EXTERN void    gx_erase_handles( GXObjPtr );

EXTERN void get_bounds( XPoint *, int, int *, int *, int *, int * );

/*
 * Object files
 */
void gxArcLoad ( FILE *, GXObjPtr );
void gxLineLoad( FILE *, GXObjPtr );
void gxTextLoad( FILE *, GXObjPtr );
/**
 ** end of gxProtos.h
 */
