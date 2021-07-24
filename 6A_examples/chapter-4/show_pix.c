/* show_pix.c -- A minimal example of a MainWindow. Use a Label as the 
* workWindow to display a bitmap specified on the command line. 
*/

#include <Xm/MainW.h>
#include <Xm/Label.h>

main (int argc, char *argv[])
{
	Widget       app_shell, main_w, label_w;    
	XtAppContext app_context;    
	Pixmap       pixmap;    
	Arg          al[4];
	Cardinal     ac = 0;
	
	XtSetLanguageProc (NULL, NULL, NULL);    
	
	app_shell = XtVaOpenApplication (&app_context, "Demos", NULL, 0, &argc, argv, NULL, 
					  sessionShellWidgetClass, NULL);   
					  
	if (!argv[1]) {
		printf ("usage: %s bitmap-file\n", argv[0]); 
		
		exit (1);    
	}
	
	ac = 0;
	XtSetArg(al[ac], XmNscrollBarDisplayPolicy, XmAS_NEEDED); ac++;
	XtSetArg(al[ac], XmNscrollingPolicy, XmAUTOMATIC); ac++;
	main_w = XmCreateMainWindow (app_shell, "main_window", al, ac); 
	
	/* Load bitmap given in argv[1] */    
	pixmap = XmGetPixmap (XtScreen (app_shell), argv[1], 
			      BlackPixelOfScreen (XtScreen (app_shell)),        
			      WhitePixelOfScreen (XtScreen (app_shell))); 
			      
	if (pixmap == XmUNSPECIFIED_PIXMAP) {
		printf ("can't create pixmap from %s\n", argv[1]);        
		exit (1);    
	}    
	
	/* Now create label using pixmap */    
	ac = 0;
	XtSetArg(al[ac], XmNlabelType, XmPIXMAP); ac++;
	XtSetArg(al[ac], XmNlabelPixmap, pixmap); ac++;
	label_w = XmCreateLabel (main_w, "label", al, ac); 
	
	/* set the label as the "work area" of the main window */    
	XtVaSetValues (main_w, XmNworkWindow, label_w, NULL);    

	XtManageChild (label_w);
	XtManageChild (main_w);
	XtRealizeWidget (app_shell);    
	XtAppMainLoop (app_context);
} 
