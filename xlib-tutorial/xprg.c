/* 
* Xlib 学习 
* 
* 
* XEvent  
* XMotion 
* 
* 
* 
* 
* 编译基于Xlib的程序需要与Xlib库连接。可以使用下面的命令行： 
  
   gcc xprg.c -o xprg -lX11
  
  如果编译器报告找不到X11库，可以试着加上"-L"标志，像这样： 
  
   cc prog.c -o prog -L/usr/X11/lib -lX11 
  
   或者这样（针对使用X11的版本6） 
  
   cc prog.c -o prog -L/usr/X11R6/lib -lX11 
  
   在SunOs 4 系统上，X的库被放到了 /usr/openwin/lib 
  
   cc prog.c -o prog -L/usr/openwin/lib -lX11 
* 
* GC-图形上下文.[图形,文本等, 绘制,前景,背景,使用什么颜色,使用什么字体等等.] 
* 对象句柄:例如: 窗口,绘图区和光标-相应的函数就会返回一个句柄. 
* 释放内存: XFree() 
* 事件: XEvent,(联合体), XMotion, XButon. 
* */  
   
/* 
*      
Display* display;    
display = XOpenDisplay("simey:0"); 
if (NULL == display){ 
fprintf(stderr, "连接不上X Server %s\n", "simey:0"); 
exit(-1); 
} 
关闭X服务器连接 
XCloseDisplay(display) 
* */  
   
   
   
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <X11/keysymdef.h>  
#include <X11/Xlib.h>  
   
int main(int argc, char *argv[])  
{  
       
     Display* display;  
     display = XOpenDisplay(0);  
       
     if (NULL == display){  
          fprintf(stderr, "连接不上X Server %s\n", "0");  
          exit(-1);  
     }  
     int screen_num;  
     int screen_width;  
     int screen_height;  
     Window root_window;  
       
     unsigned long white_pixel;  
     unsigned long black_pixel;  
       
     screen_num = DefaultScreen(display);  
       
     screen_width = DisplayWidth(display, screen_num);  
     screen_height = DisplayHeight(display, screen_num);  
           
     puts("测试输出:");  
     printf("句柄:%x宽度:%d高度:%d\n",  
            screen_num, screen_width, screen_height);  
           
     root_window = RootWindow(display, screen_num);  
     white_pixel = WhitePixel(display, screen_num);  
     black_pixel = BlackPixel(display, screen_num);  
       
     Window win;  
     int win_width;  
     int win_height;  
     int win_x;  
     int win_y;       
     int win_border_width;  
     int win_border_height;  
           
     int width;  
     int height;  
       
     win_x = win_y = win_border_width= win_border_height = 0;  
     win_width = DisplayWidth(display, screen_num);  
     win_height = DisplayHeight(display, screen_num);  
       
     width = (win_width / 3);  
     height = (win_height / 3);  
     win_border_width = 2;  
       
     /*创建一个窗口*/       
     win = XCreateSimpleWindow(  
          display,  
          RootWindow(display, screen_num),  
          win_x, win_y,  
          width, height,  
          win_border_width, win_border_height,  
          WhitePixel(display, screen_num)  
          );  
     /*注册事件*/  
     /* XSelectInput(display, win, ExposureMask); */  
     /*ExposureMask在头文件"X.h"中被定义，如果我们想注册更多的事件类型，我们可以使用逻辑"or"*/  
       
     XSelectInput(display, win,  
                  ExposureMask | ButtonPressMask |  
                  ButtonReleaseMask | ButtonPress |  
                  ButtonRelease | EnterWindowMask |  
                  LeaveWindowMask | EnterNotify |  
                  LeaveNotify  
          );  
     /*鼠标的进入和离开 Enter Leave和GTK很像.*/  
       
     XMapWindow(display, win);  
       
     /*画笔*/  
     GC gc;  
     XGCValues values;  
     unsigned long valuemask = 0;  
     /* XGCValues values = CapButt | JoinBevel; */  
     /* unsigned long valuemask = GCCapStyle | GCJoinStyle; */  
     gc = XCreateGC(display, win, valuemask, &values);  
     XSync(display, False);  
     if (gc < 0)  
     {  
          fprintf(stderr, "XCreateGC:\n");  
     }  
     /*画画.*/  
       
     XSetBackground(display, gc, WhitePixel(display, screen_num));/*设置背景颜色*/  
     XSetForeground(display, gc, BlackPixel(display, screen_num));/*设置前景色*/  
     unsigned int line_width = 2;  
     int line_style = LineSolid;  
     int cap_style = CapButt;  
     int join_style = JoinBevel;  
   
     XSetLineAttributes(display, gc,  
                        line_width, line_style, cap_style, join_style);  
     XSetFillStyle(display, gc, FillSolid);  
       
   
     /* sleep(14); */  
     XEvent an_event;  
     /*事件循环*/  
     while (1)  
     {  
          XNextEvent(display, &an_event);  
          /*这里就是判断所有事件*/  
   
           
          switch(an_event.type)  
          {  
                 
          case KeyPress:  
               printf("键盘被按下");  
               break;  
          case Expose: /*重绘*/  
               if(an_event.xexpose.count > 0)       
               {  
                    break;  
               }  
               XDrawLine(display, win, gc, 0, 100, 400, 100);  
               XDrawPoint(display, win, gc, 5, 5);  
               XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
               XFillRectangle(display, win, gc, 60, 150, 50, 60);  
               /*刷新*/  
               XFlush(display);  
               printf("正在重绘事件\n");  
               break;  
          case ButtonPress: /*按下事件*/  
               /* int x; */  
               /* int y; */  
               /* x = an_event.xbutton.x; */  
               /* y = an_event.xbutton.window; */                 
               switch(an_event.xbutton.button){  
               case Button1:  
                    XDrawRectangle(display, win, gc, 120, 150, 50, 60);       
                    puts("鼠标左键按下");  
                    break;  
               case Button2:       
                    puts("button2...");  
                    break;  
               default:  
                    break;  
               }  
          }                 
           
     }  
     /*关闭X服务器连接*/  
     XCloseDisplay(display);  
     return 0;  
}  
   
   
/* 
  Display* display 
  指向显示结构的指针 
  Window parent 
  新窗口的父窗口的ID。 
  int x 
  窗口的左上X坐标（单位为屏幕像素） 
  int y 
  窗口的左上Y坐标（单位为屏幕像素） 
  unsigned int width 
  窗口的宽度（单位为屏幕像素） 
  unsigned int height 
  窗口的高度（单位为屏幕像素） 
  unsigned int border_width 
  窗口的边框宽度（单位为屏幕像素） 
  unsigned long border 
  用来绘制窗口边框的颜色 
  unsigned long background 
  用来绘制窗口背景的颜色 
* */  
   
/* 
  在事件结构里，通过"an_event.xbutton"来获得事件的类型，另外它还包括下面这些有趣的内容： 
  
  Window window 
  事件发送的目标窗口的ID（如果我们为多个窗口注册了事件） 
  
  int x, y 
  从窗口的左上坐标算起，鼠标键按下时光标在窗口中的坐标 
  
  int button 
  鼠标上那个标号的按钮被按下了，值可能是Button1，Button2，Button3 
  
  Time time 
  事件被放进队列的时间。可以被用来实现双击的处理 
* */  
   
   
/* 
  
  鼠标光标的进入和离开事件 
  
  另一个程序通常会感兴趣的事件是，有关鼠标光标进入一个窗口的领域以及离开那个窗口的领域的事件。有些程序利用该事件来告诉用户程序现在在焦点里面。为了注册这种事件，我们将会在函数XSelectInput()里注册几个面具。 
  
  EnterWindowMask 
  通知我们鼠标光标进入了我们的窗口中的任意一个 
  
  LeaveWindowMask 
  通知我们鼠标光标离开了我们的窗口中的任意一个 
  
  我们的事件循环中的分支检查将检查以下的事件类型 
  
  EnterNotify 
  鼠标光标进入了我们的窗口 
  
  LeaveNotify 
  鼠标光标离开了我们的窗口 
  
  这些事件类型的数据结构通过例如"an_event.xcrossing"来访问，它还包含以下有趣的成员变量： 
  
  Window window 
  事件发送的目标窗口的ID（如果我们为多个窗口注册了事件） 
  
  Window subwindow 
  在一个进入事件中，它的意思是从那个子窗口进入我们的窗口，在一个离开事件中，它的意思是进入了那个子窗口，如果是"none"，它的意思是从外面进入了我们的窗口。 
  
  int x, y 
  从窗口的左上坐标算起，事件产生时鼠标光标在窗口中的坐标 
  
  int mode 
  鼠标上那个标号的按钮被按下了，值可能是Button1，Button2，Button3 
  
  Time time 
  事件被放进队列的时间。可以被用来实现双击的处理 
  
  unsigned int state 
  这个事件发生时鼠标按钮（或是键盘键）被按下的情况 - 如果有的话。这个成员使用按位或的方式来表示 
  Button1Mask 
  Button2Mask 
  Button3Mask 
  Button4Mask 
  ShiftMask 
  LockMask 
  ControlMask 
  Mod1Mask 
  Mod2Mask 
  Mod3Mask 
  Mod4Mask 
  
  它们的名字是可以扩展的，当第五个鼠标钮被按下时，剩下的属性就指明其它特殊键（例如Mod1一般是"ALT"或者是"META"键） 
  
  Bool focus 
  当值是True的时候说明窗口获得了键盘焦点，False反之 
* */  
   
   
/* 
* 键盘键按下和松开事件 
如果我们程序控制的窗口获得了键盘焦点，它就可以接受按键的按下和松开事件。为了注册这些事件的类型，我们就需要通过函数XSelectInput()来注册下面的面具。 
  
  
KeyPressMask 
通知我们的程序什么时候按键被按下了 
  
KeyPressMask 
通知我们的程序什么时候按键被松开了 
  
我们的事件循环中的分支检查将检查以下的事件类型 
  
Window window 
事件发送的目标窗口的ID（如果我们为多个窗口注册了事件） 
  
unsigned int keycode 
被按下（或松开）的键的编码。这是一些X内部编码，应该被翻译成一个键盘键符号才能方便使用，将会在下面介绍。 
  
int x, y 
从窗口的左上坐标算起，事件产生时鼠标光标在窗口中的坐标 
  
Time time 
事件被放进队列的时间。可以被用来实现双击的处理 
  
unsigned int state 
这个事件发生时鼠标按钮（或是键盘键）被按下的情况 - 如果有的话。这个成员使用按位或的方式来表示 
Button1Mask 
Button2Mask 
Button3Mask 
Button4Mask 
ShiftMask 
LockMask 
ControlMask 
Mod1Mask 
Mod2Mask 
Mod3Mask 
Mod4Mask 
  
它们的名字是可以扩展的，当第五个鼠标钮被按下时，剩下的属性就指明其它特殊键（例如Mod1一般是"ALT"或者是"META"键） */  
   
   
