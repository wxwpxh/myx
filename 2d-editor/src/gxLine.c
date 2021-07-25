/**
 ** 2D Graphical Editor (2d-gx)
 **
 ** gxLine.c
 */
#include <stdio.h>
#include <X11/Intrinsic.h>

#include "gxGraphics.h"
#include "gxProtos.h"

static void create_line( GXObjPtr, GXLine * );

/*
 *
 */
static void
apply_delta_bottom_side( XPoint *pts, int num_pts, int dx, int dy )
{
  int i;
 
  if( ExntY == 0 ) return;
 
  for( i = 0; i < num_pts; i++ ) {

    pts[i].y -= (int)( dy *
                       ((float)(pts[i].y - OrigY) / (float)ExntY));
  }
}
static void
apply_delta_right_side( XPoint *pts, int num_pts, int dx, int dy )
{
  int i;

  if( ExntX == 0 ) return;

  for( i = 0; i < num_pts; i++ ) {

    pts[i].x -= (int)( dx *
                       ((float)(pts[i].x - OrigX) / (float)ExntX));
  }
}
static void
apply_delta_top_side( XPoint *pts, int num_pts, int dx, int dy )
{
  int i;

  if( OrigY == 0 ) return;

  for( i = 0; i < num_pts; i++ ) {
    pts[i].y +=
      (int)( dy * ((float)(pts[i].y - ExntY) / (float)OrigY));
  }
}
static void
apply_delta_left_side( XPoint *pts, int num_pts, int dx, int dy )
{
  int i;

  if( OrigX == 0 ) return;

  for( i = 0; i < num_pts; i++ ) {
    pts[i].x +=
      (int)( dx * ((float)(pts[i].x - ExntX) / (float)OrigX));
  }
}
static void
apply_delta_bottom_right( XPoint *pts, int num_pts, int dx, int dy )
{
  apply_delta_right_side( pts, num_pts, dx, dy );
  apply_delta_bottom_side( pts, num_pts, dx, dy );
}
static void
apply_delta_bottom_left( XPoint *pts, int num_pts, int dx, int dy )
{
  apply_delta_bottom_side( pts, num_pts, dx, dy );
  apply_delta_left_side( pts, num_pts, dx, dy );
}
static void
apply_delta_top_right( XPoint *pts, int num_pts, int dx, int dy )
{
  apply_delta_top_side( pts, num_pts, dx, dy );
  apply_delta_right_side( pts, num_pts, dx, dy );
}
static void
apply_delta_top_left( XPoint *pts, int num_pts, int dx, int dy )
{
  apply_delta_top_side( pts, num_pts, dx, dy );
  apply_delta_left_side( pts, num_pts, dx, dy );

}

/*
 * Function:
 *
 * Description:  scale routine that invokes the correct apply_delta
 *               based on the current active handle
 */
void apply_delta( XPoint *data, int num, int dx, int dy )
{

  switch( GxActiveHandle ) {
  case 0:
    apply_delta_top_left( data, num, dx, dy );
    break;
  case 1:
    apply_delta_top_side( data, num, dx, dy );
    break;
  case 2:
    apply_delta_top_right( data, num, dx, dy );
    break;
  case 3:
    apply_delta_right_side( data, num, dx, dy );
    break;
  case 4:
    apply_delta_bottom_right( data, num, dx, dy );
    break;
  case 5:
    apply_delta_bottom_side( data, num, dx, dy );
    break;
  case 6:
    apply_delta_bottom_left( data, num, dx, dy );
    break;
  case 7:
    apply_delta_left_side( data, num, dx, dy );
    break;
  default:
    setStatus( "LINE: The end is nigh!" );
  }
}

/*
 * point_equal_event
 */
static Boolean point_equal_event( GXLine *line, XEvent *event )
{
  Boolean pts_equal = False;
  int xe_x, xe_y;

  xe_x = event->xbutton.x;
  xe_y = event->xbutton.y;

  /*
   * the last point will always be the current motion event
   * so check the one before for redundancy (equates to a
   * double click to end the action )
   */
  if( line && (line->num_pts > 2) ) {
    int num = line->num_pts - 2;

    if( (abs(line->pts[num].x - xe_x) <= TOLERANCE ) &&
        (abs(line->pts[num].y - xe_y) <= TOLERANCE )) {

      pts_equal = True;
    }
  }    
  return pts_equal;
}

/*
 * GXRubberLine
 */
static void GXRubberLine( GXLine *line )
{
  int indx; 
  if( line && line->pts && (line->num_pts > 1)) {
    indx = line->num_pts - 1;
    XDrawLine( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
               line->pts[indx  ].x,
               line->pts[indx  ].y,
               line->pts[indx-1].x,
               line->pts[indx-1].y );
  }
}

/*
 * update_line
 */
static GXLinePtr update_line( XEvent *event, GXLine *line )
{
  static GXLine xline;
  GXLinePtr     xlinePtr = &xline;

  if( line == NULL ) {
    xline.pts = (XPoint *)XtMalloc( sizeof(XPoint) );
    xline.num_pts = 0;
  } else {
    xline.pts = (XPoint *)XtRealloc( (char *)xline.pts,
                                     sizeof(XPoint) * (xline.num_pts + 1));
  }
  
  xline.pts[xline.num_pts].x = event->xbutton.x;
  xline.pts[xline.num_pts].y = event->xbutton.y;

  xline.num_pts++;

  return xlinePtr;
}

/*
 * draw_erase
 */
static void draw_erase( GXObjPtr line, Boolean tile )
{
  GC gc;
  GXLinePtr line_data = (GXLinePtr)line->data;

  gc = gx_allocate_gc( line, tile );

  XDrawLines(XtDisplay(GxDrawArea), XtWindow(GxDrawArea), gc,
             line_data->pts, line_data->num_pts, CoordModeOrigin );
  
  XtReleaseGC( GxDrawArea, gc );
}

/*
 * line_draw
 */
static void line_draw( GXObjPtr line )
{
  draw_erase( line, False );
}

/*
 * line_erase
 */
static void line_erase( GXObjPtr line )
{
  draw_erase( line, True );
}

/*
 * near_segment
 */
Boolean near_segment( int x1, int y1, int x2, int y2, int xp, int yp )
{
  int xmin, ymin, xmax, ymax;
  float slope, x, y, dx, dy;

  if( abs(xp - x1) <= TOLERANCE && abs(yp - y1) <= TOLERANCE ) {
    return True;
  }

  if( abs(xp - x2) <= TOLERANCE && abs(yp - y2) <= TOLERANCE ) {
    return True;
  }

  if( x1 < x2 ) {
    xmin = x1 - TOLERANCE;
    xmax = x2 + TOLERANCE;
  } else {
    xmin = x2 - TOLERANCE;
    xmax = x1 + TOLERANCE;
  }

  if( xp < xmin || xmax < xp ) {
    return False;
  }

  if( y1 < y2 ) {
    ymin = y1 - TOLERANCE;
    ymax = y2 + TOLERANCE;
  } else {
    ymin = y2 - TOLERANCE;
    ymax = y1 + TOLERANCE;
  }

  if (yp < ymin || ymax < yp)
    return False;


  if( x2 == x1 ) {
    x = x1;
    y = yp;
  } else if( y1 == y2 ) {
    x = xp;
    y = y1;
  } else {
    slope = ((float) (x2 - x1)) / ((float) (y2 - y1));
    y = (slope * (xp - x1 + slope * y1) + yp) / (1 + slope * slope);
    x = ((float) x1) + slope * (y - y1);
  }

  dx = ((float) xp) - x;
  dy = ((float) yp) - y;

  if ( (float)(dx * dx + dy * dy) < (float)(TOLERANCE*TOLERANCE) ) {
    return True;
  }

  return False;
}

/*
 * segment_selected
 */
static Boolean segment_selected( GXLinePtr data, XPoint *pt )
{
  Boolean found = False;
  int i;

  for( i = 0; i < data->num_pts - 1 && found == False; i++ ) {
    found = near_segment( data->pts[i  ].x, data->pts[i  ].y,
                          data->pts[i+1].x, data->pts[i+1].y,
                          pt->x, pt->y );
  }
  return found;
}

/*
 * point_selected
 */
static Boolean point_selected( GXLinePtr line, XPoint *pt )
{
  int i, x, y, found = False;
 
  for( i = 0; (i < line->num_pts) && !found; i++ ) {

    x = line->pts[i].x;
    y = line->pts[i].y;

    if( abs(pt->x - x) <= TOLERANCE  &&
        abs(pt->y - y) <= TOLERANCE ) {

      found = True;
    }
  }
  return found;
}

/*
 * line_find
 */
static Boolean line_find( GXObjPtr line, XEvent *event )
{
  Boolean found = False;
  XPoint p;

  p.x = event->xbutton.x;
  p.y = event->xbutton.y;


  found = point_selected( (GXLinePtr)line->data, &p );
  if( found == False )
    found = segment_selected( (GXLinePtr)line->data, &p );

  return found;
}

/*
 * line_move
 */
static void line_move( GXObjPtr line, XEvent *event )
{
  static int x = 0, y = 0;

  GXLinePtr line_data = (GXLinePtr)line->data;
  int i;


  if( x && y ) {
    XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                line_data->pts, line_data->num_pts, CoordModeOrigin );               
  } else {
    /* our first time through */
    (*line->erase)( line );

    x = event ? event->xbutton.x : 0;
    y = event ? event->xbutton.y : 0;
  }

  if( event ) {
    for( i = 0; i < line_data->num_pts; i++ ) {
      line_data->pts[i].x += (event->xbutton.x - x);
      line_data->pts[i].y += (event->xbutton.y - y);
    }  

    /*
     * draw rubberband line
     */
    XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                line_data->pts, line_data->num_pts, CoordModeOrigin );

    x = event->xbutton.x;
    y = event->xbutton.y;
  } else {
    x = 0;
    y = 0;
  }
}

/*
 * line_copy
 */
static void line_copy( GXObjPtr line )
{
  int i;
  GXLinePtr temp_data;
  GXLinePtr line_data = (GXLinePtr)line->data;

  (*line->deselect)( line );

  temp_data = (GXLine *)XtNew(GXLine);
  temp_data->num_pts = line_data->num_pts;

  temp_data->pts = (XPoint *)XtMalloc(sizeof(XPoint) * temp_data->num_pts );
  for( i = 0; i < temp_data->num_pts; i++ ) {
    temp_data->pts[i].x = line_data->pts[i].x + OFFSET;
    temp_data->pts[i].y = line_data->pts[i].y + OFFSET;
  }

  create_line( NULL, temp_data );
  XtFree((char *)temp_data );
}

/*
 * line_scale
 */
static void line_scale( GXObjPtr line, XEvent *event )
{
  static GXLinePtr tmp_data = NULL;
  GXLinePtr line_data = (GXLinePtr)line->data;

  if( tmp_data ) {
    XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                tmp_data->pts, tmp_data->num_pts, CoordModeOrigin );
  } else {
    /* our first time... */
    (*line->erase)( line );

    tmp_data = (GXLinePtr)XtNew(GXLine);
    tmp_data->num_pts = line_data->num_pts;
    tmp_data->pts = (XPoint *)XtMalloc( sizeof(XPoint) * tmp_data->num_pts );
  
    get_bounds( line_data->pts, line_data->num_pts, 
                &OrigX, &OrigY, &ExntX, &ExntY );
  }

  if( event ) {
    memcpy( (char *)tmp_data->pts, (char *)line_data->pts, 
            sizeof(XPoint) * tmp_data->num_pts );

    apply_delta( tmp_data->pts, tmp_data->num_pts, 
                 FixedX - event->xbutton.x, 
                 FixedY - event->xbutton.y );
    
     XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                 tmp_data->pts, tmp_data->num_pts, CoordModeOrigin );
  } else {
    if( tmp_data ) {
      memcpy( (char *)line_data->pts, (char *)tmp_data->pts,
              sizeof(XPoint) * line_data->num_pts );

      XtFree((char *)tmp_data->pts);
      XtFree((char *)tmp_data);

      tmp_data = NULL;
    }
  }
}

/*
 * line_bounding_handles
 */
static void line_bounding_handles( GXObjPtr gx_line )
{
  int  i, x1, y1, x2, y2, width, height;
 
  GXLine *line_data = gx_line->data;
 
  gx_line->handles     = (XRectangle *)XtMalloc( sizeof(XRectangle) * 8 );
  gx_line->num_handles = 8;
 
  if( gx_line->handles == NULL ) {
    perror( "Alloc failed for line handles" );
    gx_line->num_handles = 0;
    return;
  }

  for( i = 0; i < 8; i++ ) {
    gx_line->handles[i].width  = HNDL_SIZE;
    gx_line->handles[i].height = HNDL_SIZE;
  }

  get_bounds( line_data->pts, line_data->num_pts, &x1, &y1, &x2, &y2 );
  width  = x2 - x1;
  height = y2 - y1;

  gx_line->handles[0].x = x1 - HNDL_OFFSET - HNDL_SIZE;
  gx_line->handles[0].y = y1 - HNDL_OFFSET - HNDL_SIZE;
 
  gx_line->handles[1].x = x1 + (width/2) - HNDL_OFFSET;
  gx_line->handles[1].y = y1 - HNDL_SIZE - HNDL_OFFSET;
 
  gx_line->handles[2].x = x2 + HNDL_OFFSET;
  gx_line->handles[2].y = y1 - HNDL_SIZE - HNDL_OFFSET;
 
  gx_line->handles[3].x = x2 + HNDL_OFFSET;
  gx_line->handles[3].y = y1 + (height/2) - HNDL_OFFSET;
 
  gx_line->handles[4].x = x2 + HNDL_OFFSET;
  gx_line->handles[4].y = y2 + HNDL_OFFSET;
 
  gx_line->handles[5].x = x1 + (width/2) - HNDL_OFFSET;
  gx_line->handles[5].y = y2 + HNDL_OFFSET;
 
  gx_line->handles[6].x = x1 - HNDL_OFFSET - HNDL_SIZE;
  gx_line->handles[6].y = y2 + HNDL_OFFSET;
 
  gx_line->handles[7].x = x1 - HNDL_OFFSET - HNDL_SIZE;
  gx_line->handles[7].y = y1 + (height/2) - HNDL_OFFSET;
}

/*
 * line select
 */
static void line_select( GXObjPtr line )
{
  line_bounding_handles( line );
  gx_draw_handles( line );
}

/*
 * line_deselect
 */
static void line_deselect( GXObjPtr line )
{
  if( line->handles && line->num_handles > 0 ) {
    gx_erase_handles( line );

    XtFree((char *)line->handles );

    line->handles = NULL;
    line->num_handles = 0;
  }
}

void gxLineLoad( FILE *fp, GXObjPtr obj )
{
    int i;
    GXLine line;

    fscanf( fp, "%d\n", &line.num_pts );

    line.pts = (XPoint *)XtMalloc(sizeof(XPoint) * line.num_pts);
     
    for( i = 0; i < line.num_pts; i++ ) {
        fscanf( fp, "%hd %hd\n", &line.pts[i].x, &line.pts[i].y );
    }

    create_line( obj, &line );
}

/*
 * line_save
 */
static void line_save( FILE *fp, GXObjPtr obj )
{
    int i;
    GXLinePtr line = (GXLinePtr)obj->data;

    fprintf( fp, "LINE [numpts x y x y ...]\n" );
    fprintf( fp, "%d\n", line->num_pts );

    for( i = 0; i < line->num_pts; i++ ) {
        fprintf( fp, "%d %d\n", line->pts[i].x, line->pts[i].y );
    }
}

/*
 * create_line
 */
static void create_line( GXObjPtr _obj, GXLine *line )
{
  GXLinePtr line_data;
  GXObjPtr  obj = _obj;

  if( obj == NULL ) {
      obj = gx_create_obj();
  }

  line_data = (GXLinePtr)XtNew(GXLine);
  memcpy( (char *)line_data, (char *)line, sizeof(GXLine));

  obj->data = line_data;

  obj->draw     = line_draw;
  obj->erase    = line_erase;
  obj->find     = line_find;
  obj->move     = line_move;
  obj->scale    = line_scale;
  obj->copy     = line_copy;
  obj->select   = line_select;
  obj->deselect = line_deselect;
  
  obj->save = line_save;

  gx_add_obj( obj );
}

/*
 * gx_line
 */
void gx_line( XEvent *event )
{
  static GXLine *rubber_line = NULL;

  if( event == NULL ) {
    rubber_line = NULL;
  } else {
    /* remove the current rubber line (if there is one) */
    GXRubberLine( rubber_line );

    switch( event->type ) {
      case ButtonPress:
        if( rubber_line == NULL ) set_cursor( LINE_MODE );

        /*
         * First: See if we 'double clicked' and are done selecting points
         */
        if( point_equal_event( rubber_line, event ) == True ) {

          /* erase our temp line */
          XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                      rubber_line->pts, rubber_line->num_pts,
                      CoordModeOrigin );

          create_line( NULL, rubber_line );
          gx_refresh();

          set_cursor( NORMAL_MODE );
          rubber_line = NULL;
        } else {
          /*
           * Initialize a GXLine struture to manage our creation
           */
          GXRubberLine( rubber_line );
          rubber_line = update_line( event, rubber_line );
        }  
      break;

      case ButtonRelease:
      case MotionNotify:
        /*
         * update the GXLine structure based on the
         * new point and current location of the mouse
         */
        if( rubber_line ) {
          /*
           * replace the last point with the current event location
           * IF we have more than one point
           */
          if( rubber_line->num_pts > 1 ) {

            rubber_line->pts[rubber_line->num_pts-1].x = event->xbutton.x;
            rubber_line->pts[rubber_line->num_pts-1].y = event->xbutton.y;

          } else {
            (void)update_line( event, rubber_line);
          }
          /*
           * redraw the rubberbanding line
           */
          GXRubberLine( rubber_line );
        }
      break;
    }
  }
}

/*
 * gx_new_vertex
 */
static void gx_new_vertex( XEvent *xe, GXLinePtr upd, int *x, int *y )
{
  float a = -1.0, b = -1.0;

  if( upd && (upd->num_pts > 0) && (xe->xbutton.x > 0) ) {
   
    /*
     *  get the last point and see if the slope has changed
     */
    a = (float)upd->pts[upd->num_pts - 1].y /
        (float)upd->pts[upd->num_pts - 1].x;

    b = (float)xe->xbutton.y / (float)xe->xbutton.x;

    if( a != b ) {
      *x = xe->xbutton.x;
      *y = xe->xbutton.y;
    }
  } else {
    *x = xe->xbutton.x;
    *y = xe->xbutton.y;
  }
}

/*
 * update_pencil
 */
static GXLinePtr update_pencil( XEvent *event, GXLinePtr pencil )
{
  static GXLine    pen = {NULL, 0};
  GXLinePtr penPtr = &pen;

  int x = -1, y = -1;

  gx_new_vertex( event, pencil, &x, &y );

  /* start over if the user is being silly... */
  if( pen.num_pts > 1024 ) {
    pen.num_pts = 0;
  }

  if( (x > 0) && (y > 0) ) {
    if( !pencil ) {
      pen.pts = (XPoint *)XtMalloc( sizeof(XPoint) );
      pen.num_pts = 0;
    } else {
      pen.pts = (XPoint *)
        XtRealloc( (char *)pen.pts,
                   sizeof(XPoint) * (pen.num_pts + 1));
    }

    pen.pts[pen.num_pts].x = x;
    pen.pts[pen.num_pts].y = y;

    pen.num_pts++;
  }

  return penPtr;
}

/*
 * gx_pencil
 */
void gx_pencil( XEvent *event )
{
  static GXLinePtr rubber_pencil = NULL;
  static int       ptCnt         = 0;

  if( event == NULL ) {
    rubber_pencil = NULL;
  } else {
    /* 
     * remove any current rubber banding...
     */
    if( rubber_pencil ) {
      XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                  rubber_pencil->pts, rubber_pencil->num_pts, 
                  CoordModeOrigin );
    }

    switch( event->type ) {
      case ButtonPress:
        if( rubber_pencil == NULL ) {
          rubber_pencil = update_pencil( event, rubber_pencil );
          set_cursor( PENCIL_MODE );
        } else {
          create_line( NULL, rubber_pencil );
          gx_refresh();

          set_cursor( NORMAL_MODE );
          rubber_pencil = 0;
          ptCnt = 0;
        }
      break;
      
      case ButtonRelease:
      case MotionNotify:
        /*
         * update the GXLine structure based on the
         * new point and current location of the mouse
         */
        if( rubber_pencil ) {
          (void)update_pencil( event, rubber_pencil );
          XDrawLines( XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                      rubber_pencil->pts, rubber_pencil->num_pts, 
                      CoordModeOrigin );
        }
      break;
    }
  }
}

/*
 *
 */
static XRectangle *update_box( XEvent *event, XRectangle *upd )
{
  static int fix_x = 0, fix_y = 0;
  static XRectangle box;

  XRectangle *boxPtr = &box;

  if( upd == NULL ) {
    fix_x = event->xbutton.x;
    fix_y = event->xbutton.y;
  }

  box.x = min( fix_x, event->xbutton.x );
  box.y = min( fix_y, event->xbutton.y );

  box.width  = max( fix_x, event->xbutton.x ) - box.x;
  box.height = max( fix_y, event->xbutton.y ) - box.y;
  
  return boxPtr;
}

/*
 * arrow_from_box
    4
    /\
   /  \
 3/_  _\5
   2||6
   1||7
  0/  \8

 */
static GXLinePtr arrow_from_box( XRectangle *box )
{
  static GXLine arrow;
  static XPoint pts[10];

  pts[0].x = box->x + (box->width/10);
  pts[0].y = box->y + box->height;

  pts[1].x = box->x + (box->width  / 4);
  pts[1].y = box->y + box->height - (box->height / 8);

  pts[2].x = box->x + (box->width  / 4);
  pts[2].y = box->y + (box->height / 2);

  pts[3].x = box->x;
  pts[3].y = box->y + (box->height / 2);

  pts[4].x = box->x + (box->width  / 2);
  pts[4].y = box->y;

  pts[5].x = box->x + box->width;
  pts[5].y = box->y + (box->height / 2);

  pts[6].x = box->x + ((box->width * 3)  / 4);
  pts[6].y = box->y + (box->height / 2);

  pts[7].x = box->x + ((box->width * 3)  / 4);
  pts[7].y = box->y + box->height - (box->height / 8);

  pts[8].x = box->x + box->width - (box->width / 10);
  pts[8].y = box->y + box->height;

  arrow.pts     = pts;
  arrow.num_pts = 9;

  return &arrow;
}

/*
 * gx_arrow
 */
void gx_arrow( XEvent *event )
{
  static XRectangle *rubber_box = NULL;
  GXLinePtr arrow;

  if( event == NULL ) {
    rubber_box = NULL;
  } else {
    if( rubber_box ) {
      arrow = arrow_from_box( rubber_box );
      XDrawLines(XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                 arrow->pts, arrow->num_pts, CoordModeOrigin );
    }

    switch( event->type ) {
      case ButtonPress:
        rubber_box = update_box( event, NULL );
        set_cursor( EDIT_MODE );

        break;

      case ButtonRelease:
        if( rubber_box ) {
          create_line( NULL, arrow_from_box(rubber_box) );
          gx_refresh();
        }
        set_cursor( NORMAL_MODE );
        rubber_box = NULL;

      break;

      case MotionNotify:
        if( rubber_box ) {
          (void)update_box( event, rubber_box );

          arrow = arrow_from_box( rubber_box );
          XDrawLines(XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                     arrow->pts, arrow->num_pts, CoordModeOrigin );

        }
      break;
    }
  }
}

/*
 *
 */
static GXLinePtr line_from_box( XRectangle *box )
{
  static GXLine line;

  line.pts = (XPoint *)XtMalloc( sizeof(XPoint) * 5);
  line.num_pts = 5;

  line.pts[0].x = box->x;
  line.pts[0].y = box->y;

  line.pts[1].x = box->x + box->width;
  line.pts[1].y = box->y;

  line.pts[2].x = box->x + box->width;
  line.pts[2].y = box->y + box->height;

  line.pts[3].x = box->x;
  line.pts[3].y = box->y + box->height;

  line.pts[4].x = box->x;
  line.pts[4].y = box->y;

  return &line;
}

/*
 * gx_box
 */
void gx_box( XEvent *event )
{
  static XRectangle *rubber_box = NULL;

  if( event == NULL ) {
    rubber_box = NULL;
  } else {
    /*
     * remove the current rubberband if there is one
     */
    if( rubber_box ) {
      XDrawRectangle(XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                     rubber_box->x, rubber_box->y,
                     rubber_box->width, rubber_box->height );
    }

    switch( event->type ) {
      case ButtonPress:
        rubber_box = update_box( event, NULL );
        set_cursor( EDIT_MODE );
      break;

      case ButtonRelease:
        if( rubber_box ) {
          create_line( NULL, line_from_box(rubber_box) );
          gx_refresh();
        }
        set_cursor( NORMAL_MODE );
        rubber_box = NULL;
      break;

      case MotionNotify:
        if( rubber_box ) {
          (void)update_box( event, rubber_box );
          XDrawRectangle(XtDisplay(GxDrawArea), XtWindow(GxDrawArea), rubberGC,
                         rubber_box->x, rubber_box->y,
                         rubber_box->width, rubber_box->height );
        }
      break;
    }
  }
}

/**
 ** end of gxLine.c
 */
