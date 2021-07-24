#include <Xm/Xm.h>
#include <X11/bitmaps/mailfull>

main (int argc, char *argv[])
{
	Widget       toplevel;
 	XtAppContext app;
	Pixmap       bitmap;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	/* size is irrelevant -- toplevel is iconified */
	/* it just can't be 0, or Xt complains */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass,
					XmNwidth, 100,
					XmNheight, 100,
					XmNiconic, True,
					NULL);
					
	bitmap = XCreatePixmapFromBitmapData (XtDisplay (toplevel), 
					      RootWindowOfScreen (XtScreen (toplevel)), 
					      (char *) mailfull_bits, 
					      mailfull_width, 
					      mailfull_height, 
					      1, 0, 1);

	XtVaSetValues (toplevel, XmNiconPixmap, bitmap, NULL);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
