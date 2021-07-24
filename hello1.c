/*
   gcc -o hello1 hello1.c `pkg-config --libs --cflags xaw7 xt xext`
   gcc -o hello1 hello1.c -lX11 -lXt -lXaw
   */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define DUMP_ERR printf

static void draw(Display *display,Window win,GC gc,int width,int height);

int main()
{
	Display *display;
	int screennum;
	int width;
	int height;
	Atom protocols;

	const char *strdraw = "hello world";

	Window win;
	GC gc;

	display = XOpenDisplay(NULL);
	if( !display ){
		DUMP_ERR("call XOpenDisplay(%s) fail/n",XDisplayName(NULL));
		return 1;
	}

	// get default screen
	screennum = DefaultScreen(display);
	width = DisplayWidth(display,screennum)/2;
	height = DisplayHeight(display,screennum)/2;

	// create window
	win = XCreateSimpleWindow(display,
			RootWindow(display,screennum),
			0,0,width,height,3,
			BlackPixel(display,screennum),
			WhitePixel(display,screennum));

	//增加这个代码是捕获程序退出事件
	protocols = XInternAtom(display,"WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, win, &protocols, 1);

	// select event
	XSelectInput(display,win,
			ExposureMask|KeyPressMask
			|ButtonPressMask|StructureNotifyMask);

	// create gc
	{
		unsigned long valuemask = 0;
		XGCValues values;
		gc = XCreateGC(display,win,valuemask,&values);
	}

	// show window
	XMapWindow(display,win);

	// event loop
	while(1){
		XEvent event;
		XNextEvent(display,&event);

		switch(event.type){
			case Expose: // expose window
				if( event.xexpose.count != 0) break;
				draw(display,win,gc,width,height);
				//XDrawString(display,win,gc,10,10,
				//strdraw,strlen(strdraw));
				break;

			case ConfigureNotify: // when the window’s size change
				width = event.xconfigure.width;
				height = event.xconfigure.height;
				break;
			case ClientMessage :
				if(event.xclient.data.l[0] == protocols){
					DUMP_ERR("recv destroy notify/n");
					XFreeGC(display,gc);
					XDestroyWindow(display, win);
					XCloseDisplay(display);
				}
				return 0;

			default:
				break;
		}
	}
}
// 使用changeGC修改属性的代码 只有valuemask被设置的属性才会根据XCGValues里面的值进行修改
static void set_gc_values(Display *display,GC gc)
{
	XGCValues values;
	values.line_width = 2;
	values.line_style = LineSolid;
	values.cap_style = CapRound;
	values.join_style = JoinRound;

	XChangeGC(display,gc,GCLineWidth|GCLineStyle,//|GCCapStyle|GCFillStyle,
			&values);
}

// 画画的代码
static void draw(Display *display,Window win,GC gc,int width,int height)
{
	int i,x,y;
	unsigned int ling_width = 0;
	int line_style = LineSolid;// 是实线
	int cap_style = CapRound; // 端点类型是圆的
	int join_style = JoinRound;// 接点类型是圆的

	XPoint points[5] = {
		{400,200},
		{600,200},
		{450,300},
		{500,150},
		{550,300},
	};

	// draw point
	for(i = 0;i< width;i++)
		XDrawPoint(display,win,gc,i,10);

	// draw line
	y = 30;
	for(i = 0;i<10;i+=2){
		// int XSetLineAttributes(Display *display, GC gc, unsigned int
		// line_width, int line_style, int cap_style, int join_style);
		XSetLineAttributes(display,gc,i,line_style,cap_style,join_style);
		// int XDrawLine(Display *display, Drawable d, GC gc, int x1, int y1, int
		//x2, int y2);
		XDrawLine(display,win,gc,10,y,width-10,y);
		y += 20;
	}

	set_gc_values(display,gc);
	XDrawRectangle(display,win,gc,10,y,50,50);
	XDrawRectangle(display,win,gc,100,y,200,200);

	XDrawArc(display,win,gc,100,y,200,200,0,90*64);
	XFillArc(display,win,gc,100,y,200,200,90*64,120*64);

	XFillPolygon(display,win,gc,points,5,Complex,EvenOddRule);
}

