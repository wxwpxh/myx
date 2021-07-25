/**
 ** 2D Graphical Editor (2d-gx)
 **
 ** gxArc.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>

#include <math.h>

#include "gxGraphics.h"
#include "gxProtos.h"

#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif

static void create_arc( GXObjPtr, XArc * );
static double sqr(float f) { return ( f * f ); }

/*
 */
static void apply_delta_top_side( XArc *data, int dx, int dy )
{
  int y1, y2;

  y1 = data->y;
  y2 = data->y + data->height;

  data->y = min( y1 - dy, y2 + dy );
  data->height = max( y1 - dy, y2 + dy ) - data->y;
}
static void apply_delta_right_side( XArc *data, int dx, int dy )
{
  int x1, x2;

  x1 = data->x;
  x2 = data->x + data->width;

  data->x = min( x1 + dx, x2 - dx );
  data->width = max(x1 + dx, x2 - dx) - data->x;
}
static void apply_delta_top_right( XArc *data, int dx, int dy )
{
  apply_delta_right_side( data, dx, dy );
  apply_delta_top_side( data, dx, dy );
}
static void apply_delta_bottom_side( XArc *data, int dx, int dy )
{
  int y1, y2;

  y1 = data->y;
  y2 = data->y + data->height;

  data->y = min( y1 + dy, y2 - dy );
  data->height = max(y1 + dy, y2 - dy) - data->y;
}
static void apply_delta_bottom_right( XArc *data, int dx, int dy )
{
  apply_delta_bottom_side( data, dx, dy );
  apply_delta_right_side( data, dx, dy );
}
static void apply_delta_left_side( XArc *data, int dx, int dy )
{
  int x1, x2;

  x1 = data->x;
  x2 = data->x + data->width;

  data->x = min( x1 - dx, x2 + dx );
  data->width = max( x1 - dx, x2 + dx ) - data->x;
}
static void apply_delta_bottom_left( XArc *data, int dx, int dy )
{
  apply_delta_bottom_side( data, dx, dy );
  apply_delta_left_side( data, dx, dy );
}
static void apply_delta_top_left( XArc *data, int dx, int dy )
{
  apply_delta_top_side( data, dx, dy );
  apply_delta_left_side( data, dx, dy );
}

/*
 * apply_delta
 */
static void apply_delta( XArc *data, int dx, int dy )
{
  switch( GxActiveHandle ) {
  case 0:
    apply_delta_top_left( data, dx, dy );
    break;
  case 1:
    apply_delta_top_side( data, dx, dy );
    break;
  case 2:
    apply_delta_top_right( data, dx, dy );
    break;
  case 3:
    apply_delta_right_side( data, dx, dy );
    break;
  case 4:
    apply_delta_bottom_right( data, dx, dy );
    break;
  case 5:
    apply_delta_bottom_side( data, dx, dy );
    break;
  case 6:
    apply_delta_bottom_left( data, dx, dy );
    break;
  case 7:
    apply_delta_left_side( data, dx, dy );
    break;
  default:
    /* -shouldn't- happen in my time */
    setStatus( "ARC: The end is nigh!" );
  }
}

/*
 * update_arc
 */
static XArc *update_arc( XEvent *event, XArc *upd )
{
  static int fix_x = 0, fix_y = 0;
  static XArc arc;

  XArc *arcPtr = &arc;
  int x1, y1, x2, y2, rx, ry;

  if( upd == NULL ) {
    arc.angle1 = 0*64;
    arc.angle2 = 360*64;

    fix_x = event->xbutton.x;
    fix_y = event->xbutton.y;
  }

  rx = event->xbutton.x - fix_x;
  ry = event->xbutton.y - fix_y;

  x1 = fix_x + rx;
  x2 = fix_x - rx;
  y1 = fix_y + ry;
  y2 = fix_y - ry;
  

  arc.x = min(x1, x2);
  arc.y = min(y1, y2);
  
  arc.width  = max(x1, x2) - arc.x;
  arc.height = max(y1, y2) - arc.y;

  return arcPtr;
}

/*
 * draw_erase
 */
static void draw_erase( GXObjPtr arc, Boolean tile )
{
  GC gc;
  XArc *arc_data;

  gc = gx_allocate_gc( arc, tile );
  arc_data = (XArc *)arc->data;

  XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), gc,
            arc_data->x,      arc_data->y,
            arc_data->width,  arc_data->height,
            arc_data->angle1, arc_data->angle2 );

  XtReleaseGC( GxDrawArea, gc );
}

/*
 * arc_draw
 */
static void arc_draw( GXObjPtr obj )
{
  draw_erase( obj, False );
}

/*
 * arc_erase
 */
static void arc_erase( GXObjPtr obj )
{
  gx_erase_handles( obj );
  draw_erase( obj, True );
}

/*
 * arc_move
 */
static void arc_move( GXObjPtr arc, XEvent *event )
{
  static int x = 0, y = 0;
  XArc *arc_data = (XArc *) arc->data;

  if( x && y ) {
    /*
     * erase the rubberband arc
     */
    XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
              arc_data->x, arc_data->y,
              arc_data->width, arc_data->height,
              arc_data->angle1, arc_data->angle2 );

  } else {
    /*
     * our first time through - erase the actual arc...
     */
    (*arc->erase)( arc );

    /*
     * ...store the current event location
     */
    x = event ? event->xbutton.x : 0;
    y = event ? event->xbutton.y : 0;
  }

  if( event ) {
    /*
     *  get the x,y delta
     */
    arc_data->x += (event->xbutton.x - x);
    arc_data->y += (event->xbutton.y - y);

    /*
     * draw a rubberband arc
     */
    XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
              arc_data->x, arc_data->y,
              arc_data->width, arc_data->height,
              arc_data->angle1, arc_data->angle2 );

    x = event->xbutton.x;
    y = event->xbutton.y;
  } else {
    x = 0;
    y = 0;
  }
}

/*
 * arc_scale
 */
static void arc_scale( GXObjPtr arc, XEvent *event )
{
  static XArc *tmp_data = NULL;

  if( tmp_data ) {
    /*
     * erase the rubberband arc
     */
    XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
              tmp_data->x, tmp_data->y,
              tmp_data->width, tmp_data->height,
              tmp_data->angle1, tmp_data->angle2 );

  } else {
    /*
     * our first time through - erase the actual arc
     */
    (*arc->erase)( arc );

    tmp_data = (XArc *)XtNew( XArc );
  }
 
  if( event ) {
    memcpy( (char *)tmp_data, (char *)arc->data, sizeof(XArc));
    apply_delta( tmp_data,
                 FixedX - event->xbutton.x,
                 FixedY - event->xbutton.y );

    /*
     * draw a rubberband arc
     */
    XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
              tmp_data->x, tmp_data->y,
              tmp_data->width, tmp_data->height,
              tmp_data->angle1, tmp_data->angle2 );

  } else {
    if( tmp_data ) {
       memcpy( (char *)arc->data, (char *)tmp_data,
              sizeof(XArc));

      XtFree( (char *)tmp_data );
      tmp_data = NULL;
    }
  }
}

/*
 * arc_copy
 */
static void arc_copy( GXObjPtr arc )
{
  XArc *temp_data;
  (*arc->deselect)(arc);
  
  temp_data = (XArc *)XtNew(XArc);
  memcpy( (char *)temp_data, (char *)arc->data, sizeof(XArc));

  temp_data->x += OFFSET;
  temp_data->y += OFFSET;
  create_arc( NULL, temp_data );

  XtFree( (char *)temp_data );
}

/*
 * arc_bounding handles
 */
static void arc_bounding_handles( GXObjPtr gx_arc ) 
{
  int i;
  XArc *arc = gx_arc->data;
 
  gx_arc->handles = (XRectangle *)XtMalloc( sizeof(XRectangle) * 8 );
  gx_arc->num_handles = 8;

  if( gx_arc->handles == NULL ) {
    fprintf(stderr, "Alloc failed for arc handles" );
    gx_arc->num_handles = 0;
    return;
  }

  for( i = 0; i < 8; i++ ) {
    gx_arc->handles[i].x = 
    gx_arc->handles[i].y = 0;
 
    gx_arc->handles[i].width  = HNDL_SIZE;
    gx_arc->handles[i].height = HNDL_SIZE;
  }

  gx_arc->handles[0].x = arc->x - HNDL_SIZE - HNDL_OFFSET;
  gx_arc->handles[0].y = arc->y - HNDL_SIZE - HNDL_OFFSET;

  gx_arc->handles[1].x = arc->x + (arc->width/2) - HNDL_OFFSET;
  gx_arc->handles[1].y = arc->y - HNDL_SIZE - HNDL_OFFSET;

  gx_arc->handles[2].x = arc->x + arc->width + HNDL_OFFSET;
  gx_arc->handles[2].y = arc->y - HNDL_SIZE - HNDL_OFFSET;

  gx_arc->handles[3].x = arc->x + arc->width  + HNDL_OFFSET;
  gx_arc->handles[3].y = arc->y +(arc->height/2) - HNDL_OFFSET;

  gx_arc->handles[4].x = arc->x + arc->width  + HNDL_OFFSET;
  gx_arc->handles[4].y = arc->y + arc->height + HNDL_OFFSET;

  gx_arc->handles[5].x = arc->x + (arc->width/2) - HNDL_OFFSET;
  gx_arc->handles[5].y = arc->y + arc->height + HNDL_OFFSET;

  gx_arc->handles[6].x = arc->x - HNDL_SIZE - HNDL_OFFSET;
  gx_arc->handles[6].y = arc->y + arc->height + HNDL_OFFSET;

  gx_arc->handles[7].x = arc->x - HNDL_SIZE - HNDL_OFFSET;
  gx_arc->handles[7].y = arc->y +(arc->height/2) - HNDL_OFFSET;
}

/*
 * arc_select
 */
static void arc_select( GXObjPtr arc )
{
  arc_bounding_handles( arc );
  gx_draw_handles( arc );
}

/*
 * arc_deselect
 */
static void arc_deselect( GXObjPtr arc )
{
  if( arc->handles && arc->num_handles > 0 ) {
    gx_erase_handles( arc );

    XtFree((char *)arc->handles );

    arc->handles = NULL;
    arc->num_handles = 0;
  }
}

/*
 * arc_find
 */
static Boolean arc_find( GXObjPtr arc, XEvent *event )
{
  double rx,     ry,
         cx,     cy,
         ex,     ey,
         f1x,    f1y,
         f2x,    f2y,
         hmaj,   hmin,
         d,      angle,
         angle1, angle2;

  XArc *arc_data = (XArc *)arc->data;

  /*
  ** Calculate half of the major and minor axes (radius x and radius y)
  */
  rx = (float)arc_data->width  / 2;
  ry = (float)arc_data->height / 2;

  /*
  ** Find the center of the ellipse
  */
  cx = arc_data->x + rx;
  cy = arc_data->y + ry;

  /*
  ** Calculate the distance between the center of the ellipse
  ** and the foci.  Subsequently find the coordinates of the foci.
  */
  d  = sqrt( fabs( sqr(rx) - sqr(ry) ) );
  if( rx >= ry ) {
    f1x  = cx - d;
    f1y  = cy;
    f2x  = cx + d;
    f2y  = cy;
    hmaj = rx;
    hmin = ry;
  } else {
    f1x  = cx;
    f1y  = cy - d;
    f2x  = cx;
    f2y  = cy + d;
    hmaj = ry;
    hmin = rx;
  }

  ex = event->xbutton.x;
  ey = event->xbutton.y;

  /*
  ** Determine the angle the measured from the positive x-axis
  ** (3 O'Clock) of the vector ( (ex, ey), (cx, cy) ) measured in degrees.
  ** atan2 returns a value between -M_PI and M_PI.
  */
  angle  = atan2( cy - ey, ex - cx ) * 180/M_PI;
  if( angle < 0 )
    angle += 360;

  /*
  ** Convert the X measurement of the angles (degrees * 64 as an integer)
  ** to degrees.
  */
  angle1 = (double)arc_data->angle1 / 64.0;
  angle2 = (double)arc_data->angle2 / 64.0;

  /*
  ** If the sum of the distances between the selected point and the
  ** foci is greater than the major-axis by more than the tolerance
  ** then don't select the arc.
  */
  if( sqrt( sqr(f1x - ex) + sqr(f1y - ey) ) +
      sqrt( sqr(f2x - ex) + sqr(f2y - ey) ) > 2 * (hmaj + TOLERANCE))
    return( False );
#ifdef HOLD
  /*
  ** Now check if the point falls within the exclude area of the ellipse,
  ** which is either a pie-slice (arc_mode == ArcPieSlice) or the points
  ** above the chord determined by the two extremes of the ellipse (arcmode
  ** == ArcChord).
  ** If the selected point falls within the angles specified (modulo
  ** tolerance) then don't select the arc.
  ** Note: don't perform comparison if slice hasn't been cut from the pie.
  */
  if( fabs( angle1 - angle2 ) < 359.5 &&
      ( angle < angle1 - .3 || angle > angle1 + angle2 + .3)
      /* && arc->arc_mode == PieSlice  ||  (point above the chord) */ )
    return( False );
  /*
  ** At this point, the point lies on or within the ellipse.  So if the
  ** fill_style is not NULL, i.e. the ellipse is filled, the arc should be
  ** selected.  On the other hand if the ellipse is not filled, then we
  ** need to check that the selected point is on the boundary within the
  ** set tolerance. This will be the case iff the sum of the squares of the
  ** distances is less than the major axis modulo TOLERANCE.
  */
  if( ! arc->fill_style ) {
    /*
    ** If the sum of the distances between the selected point and the
    ** foci is greater than the major-axis by more than the tolerance
    ** then don't select the arc.
    */
#endif /* HOLD */
    if( sqrt( sqr(f1x - ex) + sqr(f1y - ey) ) +
        sqrt( sqr(f2x - ex) + sqr(f2y - ey) ) < 2 * hmaj - TOLERANCE )
      return( False );
#ifdef HOLD
  }
#endif

  return( True );
}

void gxArcLoad( FILE *fp, GXObjPtr obj )
{
    XArc arc;
   
    fscanf( fp, "%hd %hd %hd %hd %hd %hd\n",
                 &arc.x, &arc.y, &arc.width, 
                 &arc.height, &arc.angle1, &arc.angle2 );

    create_arc( obj, &arc );  
}

/*
 * arc_save
 */
static void arc_save( FILE *fp, GXObjPtr arc )
{
    XArc *arc_data = (XArc *)arc->data;

    fprintf( fp, "ARC [x y width height angle1 angle2]\n");

    fprintf( fp, "%d %d %d %d %d %d\n",
                 arc_data->x, arc_data->y, 
                 arc_data->width, arc_data->height,
                 arc_data->angle1, arc_data->angle2 );
}

/*
 * create_arc
 */
static void create_arc( GXObjPtr _obj, XArc *arc )
{
  XArc *arc_data;
  GXObjPtr obj = _obj;

  if( obj == NULL ) {
      obj = gx_create_obj();
  }

  arc_data  = (XArc *)XtNew( XArc );
  obj->data = (void *)arc_data;

  arc_data->width  = arc->width;
  arc_data->height = arc->height;

  arc_data->x = arc->x;
  arc_data->y = arc->y;

  arc_data->angle1 = arc->angle1;
  arc_data->angle2 = arc->angle2;

  obj->draw  = arc_draw;
  obj->erase = arc_erase;
  obj->find  = arc_find;
  obj->move  = arc_move;
  obj->scale = arc_scale;
  obj->copy  = arc_copy;

  obj->save  = arc_save;

  obj->select   = arc_select;
  obj->deselect = arc_deselect;

  gx_add_obj( obj );
} 

/*
 * gx_arc
 */
void gx_arc( XEvent *event )
{
  static XArc *rubber_arc = NULL;

  if( event == NULL ) {
    rubber_arc = NULL;
  } else {
    if( rubber_arc && rubber_arc->width > 0 && rubber_arc->height > 0 ) {
      XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                rubber_arc->x, rubber_arc->y,
                rubber_arc->width, rubber_arc->height, 
                rubber_arc->angle1, rubber_arc->angle2 );
    }

    switch( event->type ) {
      case ButtonPress:
        /*
         * initailize an XArc structure to retain
         * the arc info during rubber banding
         */
        rubber_arc = update_arc( event, NULL );
        set_cursor( EDIT_MODE );
      break;

    case ButtonRelease:
      /*
       * create the arc, and mark it selected as
       * the current object
       */
      if( rubber_arc && rubber_arc->width && rubber_arc->height ) {
        create_arc( NULL, rubber_arc );
        gx_refresh();
      }
      set_cursor( NORMAL_MODE );
      rubber_arc = NULL;

      break;
    case MotionNotify:
      /*
       * update the XArc structure based on the
       * new location of the mouse pointer
       */
      if( rubber_arc ) {
        (void)update_arc( event, rubber_arc );

        /*
         * redraw the rubberbanding arc
         */
        if( rubber_arc->width && rubber_arc->height ) {
          XDrawArc( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                    rubber_arc->x, rubber_arc->y,
                    rubber_arc->width, rubber_arc->height,
                    rubber_arc->angle1, rubber_arc->angle2 );
        }
      }
      break;
    }
  }
}

/**
 ** end of gxArc.c
 */
