/*
   gcc -o hello2 hello2.c `pkg-config --libs --cflags xaw7 xt xext`
   gcc -o hello2 hello2.c -lX11 -lXt -lXaw
   */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>


int main() {
        Display *display;
        Window window;
        XSetWindowAttributes attr;
        Colormap colormap;
        XColor color1, color2;
        XGCValues gcvalue;
        GC gc;
        XSizeHints *sz;
        
        display = XOpenDisplay("0:0");
        /* 取得预设之 colormap */
        colormap = DefaultColormap(display, 
                DefaultScreen(display));
        /* 取得 colorcell */
        color1.red = color1.blue = 0xffff;
        color1.green = 0;
        color2.red = color2.green = color2.blue = 0xff;
        color1.flags = color2.flags = DoRed | DoGreen | DoBlue; 
        XAllocColor(display, colormap, &color1);
        XAllocColor(display, colormap, &color2);
        /* 设定视窗的 attribute 和建设 */
        attr.background_pixel = color2.pixel; /* 背景颜色 */
        window = XCreateWindow(display,
                XDefaultRootWindow(display), 100, 100, 300, 300,
                2, XDefaultDepth(display, 0), InputOutput, 
                CopyFromParent, CWBackPixel, &attr);
        /* 设定和 window manager 进行沟通 */
        XStoreName(display, window, "hello!! world!!");
        sz = XAllocSizeHints();
        sz->x = 100;
        sz->y = 100;
        sz->width = 300;
        sz->height = 300;
        sz->flags = USPosition | USSize;
        XSetNormalHints(display, window, sz);
        /* 显示视窗 */
        printf("Map window＼n");
        XMapWindow(display, window);
        XFlush(display);
        getchar();
        
        /* 建立并设定 GC */
        gc = XCreateGC(display, window, 0, &gcvalue);
        XSetForeground(display, gc, color1.pixel);
        XSetBackground(display, gc, color2.pixel);
        
        /* 画一个矩形 */
        printf("Draw rectangle＼n");
        XDrawRectangle(display, window, gc, 10, 10, 100, 100);
        XFlush(display);
        getchar();
        
        /* 清除视窗 */
        XClearWindow(display, window);
        
        /* 设定 GC 内，线的形式 */
        XSetLineAttributes(display, gc, 5, LineOnOffDash,
                CapButt, JoinRound);
        /* 画线 (200, 10) - (200, 2Array0) */
        printf("Draw line＼n");
        XDrawLine(display, window, gc, 200, 10, 200, 400);
        XFlush(display);
        getchar();
        
        /* 关闭视窗 */
        printf("Destory Window＼n");
        XDestroyWindow(display, window);
        XFlush(display);
        getchar();
        
        printf("close display＼n");
        XCloseDisplay(display);
		getchar();
}