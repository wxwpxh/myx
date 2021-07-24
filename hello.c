/*
   gcc -o hello hello.c `pkg-config --libs --cflags xaw7 xt xext`
   gcc -o hello hello.c -lX11 -lXt -lXaw
 */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Label.h>

int main(int argc,char **argv)
{
	XtAppContext app_context;
	Widget toplevel,hello;

	toplevel = XtVaAppInitialize(&app_context,"XHello",NULL,0,
				&argc,argv,NULL,NULL);
	hello = XtVaCreateManagedWidget("Hello World!",labelWidgetClass,
				toplevel,(void*)0);

	XtRealizeWidget(toplevel);

	XtAppMainLoop(app_context);
	return 0;
}
