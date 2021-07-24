/*
 *  gcc -o draw draw.c -lXm -lXt -lX11
 *
 * */


#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>

#include <stdio.h>
#include <stdlib.h>

/* Prototype functions */

void quit_call(void);
void draw_cbk(Widget , XtPointer ,
         XmDrawingAreaCallbackStruct *);
void load_font(XFontStruct **);

/* XLIB Data */

Display *display;
Screen *screen_ptr;

int main(int argc, char *argv[])

{   Widget top_wid, main_w, menu_bar, draw, quit;
    XtAppContext app;
    XGCValues gcv;
    GC gc;
    

    top_wid = XtVaAppInitialize(&app, "Draw", NULL, 0, 
        &argc, argv, NULL,
        XmNwidth,  500,
        XmNheight, 500,
        NULL);
       
    main_w = XtVaCreateManagedWidget("main_window",
        xmMainWindowWidgetClass,   top_wid,
        NULL);
        
    menu_bar = XmCreateMenuBar(main_w, "main_list", 
        NULL, 0);        
    XtManageChild(menu_bar);
      
     /* create quit widget + callback */
        
   quit = XtVaCreateManagedWidget( "Quit",
        xmCascadeButtonWidgetClass, menu_bar,
        XmNmnemonic, 'Q',
        NULL);
        

    XtAddCallback(quit, XmNactivateCallback, quit_call, 
                  NULL);

    /* Create a DrawingArea widget. */
    draw = XtVaCreateWidget("draw",
        xmDrawingAreaWidgetClass, main_w,
        NULL);
        
    /* get XLib Display Screen and Window ID's  for draw */
    
    display = XtDisplay(draw);
    screen_ptr = XtScreen(draw);
       
/* set the DrawingArea as the "work area" of main window */
    XtVaSetValues(main_w,
        XmNmenuBar,    menu_bar,
        XmNworkWindow, draw,
        NULL);
        
    /* add callback for exposure event */
    XtAddCallback(draw, XmNexposeCallback, draw_cbk, NULL);

    /* Create a GC. Attach GC to the DrawingArea's 
       XmNuserData.
       NOTE : This is a useful method to pass data */
    
    gcv.foreground = BlackPixelOfScreen(screen_ptr);
    gc = XCreateGC(display,
        RootWindowOfScreen(screen_ptr), GCForeground, &gcv);
    XtVaSetValues(draw, XmNuserData, gc, NULL);

    XtManageChild(draw);
    XtRealizeWidget(top_wid);
    XtAppMainLoop(app);
	
	return 0;
}


/* CALL BACKS */

void quit_call()

{   printf("Quitting program\n");
    exit(0);
}

/*  DrawingArea Callback. NOTE: cbk->reason says type of 
    callback event */
 
void
draw_cbk(Widget w, XtPointer data,
         XmDrawingAreaCallbackStruct *cbk)

{   char  str1[25];
    int len1, width1, font_height;
    unsigned int width, height;
    int  x, y, angle1, angle2, x_end, y_end;
    unsigned int line_width = 1;
    int line_style = LineSolid;
    int cap_style = CapRound;
    int join_style = JoinRound;    
    XFontStruct *font_info;
    XEvent *event = cbk->event;
    GC gc;
    Window win = XtWindow(w);


  if (cbk->reason != XmCR_EXPOSE) 
     { /* Should NEVER HAPPEN for this program */
       printf("X is screwed up!!\n");
       exit(0);
     } 
     
   /* get font info */
       
   load_font(&font_info);
       
   font_height = font_info->ascent + font_info->descent;
       
   /* get gc from Drawing Area user data */
       
   XtVaGetValues(w, XmNuserData, &gc, NULL);

  /* DRAW A RECTANGLE */
 
  x = y = 10;
  width = 100;
  height = 50;
 
  XDrawRectangle(display, win, gc, x, y, width, height);

  strcpy(str1,"RECTANGLE"); 
  len1 = strlen(str1);
  y += height + font_height + 1; 
   if ( (x = (x + width/2) - len1/2) < 0 ) x = 10;
  
  XDrawString(display, win, gc, x, y, str1, len1); 

  /* Draw a filled rectangle */

  x = 10; y = 150;
  width = 80;
  height = 70;

  XFillRectangle(display, win, gc, x, y, width, height);

  strcpy(str1,"FILLED RECTANGLE"); 
  len1 = strlen(str1);
  y += height + font_height + 1; 
  if ( (x = (x + width/2) - len1/2) < 0 ) x = 10;
  
  XDrawString(display, win, gc, x, y, str1, len1); 

 
  /* draw an arc */

  x = 200; y = 10;
  width = 80;
  height = 70;
  angle1 = 180 * 64; /* 180 degrees */
  angle2 = 90 * 64; /* 90 degrees */

  XDrawArc(display, win, gc, x, y, width, height, 
           angle1, angle2);

  strcpy(str1,"ARC"); 
  len1 = strlen(str1);
  y += height + font_height + 1; 
  if ( (x = (x + width/2) - len1/2) < 0 ) x = 200;
  
  XDrawString(display, win, gc, x, y, str1, len1);

  /* draw a filled arc */
  
  x = 200; y = 200;
  width = 100;
  height = 50;
  angle1 = 270 * 64; /* 270 degrees */
  angle2 = 180 * 64; /* 180 degrees */

  XFillArc(display, win, gc, x, y, width, height, 
           angle1, angle2);

  strcpy(str1,"FILLED ARC"); 
  len1 = strlen(str1);
  y += height + font_height + 1; 
  if ( (x = (x + width/2) - len1/2) < 0 ) x = 200;
  
  XDrawString(display, win, gc, x, y, str1, len1);

  /* SOLID LINE */

  x = 10; y = 300;
  /* start and end points of line */
  x_end = 200; y_end = y - 30; 
  XDrawLine(display, win, gc, x, y, x_end, y_end);

  strcpy(str1,"SOLID LINE"); 
  len1 = strlen(str1);
  y += font_height + 1; 
  if ( (x = (x + x_end)/2 - len1/2) < 0 ) x = 10;

  XDrawString(display, win, gc, x, y, str1, len1);

  /* DASHED LINE */

  line_style = LineOnOffDash;
  line_width = 2;

  /* set line attributes */

   XSetLineAttributes(display, gc, line_width, line_style, 
                      cap_style, join_style);

  x = 10; y = 350;
  /* start and end points of line */
  x_end = 200; y_end = y - 30; 
  XDrawLine(display, win, gc, x, y, x_end, y_end);

  strcpy(str1,"DASHED LINE"); 
  len1 = strlen(str1);
  y += font_height + 1; 
  if ( (x = (x + x_end)/2 - len1/2) < 0 ) x = 10;

  XDrawString(display, win, gc, x, y, str1, len1);
 }


void load_font(XFontStruct **font_info)

{  char *fontname = "fixed";
    XFontStruct *XLoadQueryFont();

   /* load and get font info structure  */

  if (( *font_info = XLoadQueryFont(display, fontname)) 
        == NULL)
      { /* error - quit early */
         printf("%s: Cannot load  %s font\n", "draw.c", 
                fontname); 
         exit(-1);
      }
}
