/* app_box.c -- make an array of DrawnButtons that, when activated,
** executes a program. When the program is running, the drawn button
** associated with the program is insensitive. When the program dies,
** reactivate the button so the user can select it again.
*/

#include <Xm/DrawnB.h>
#include <Xm/RowColumn.h>
#include <signal.h>

#ifndef   SYSV
#include <sys/wait.h>
#else  /* SYSV */
#ifndef   SIGCHLD
#define   SIGCHLD SIGCLD
#endif /* SIGCHLD */
#endif /* SYSV */

#define MAIL_PROG "/bin/mail"

typedef struct 
{
	Widget  drawn_w;
	char   *pixmap_file;
	char   *exec_argv[6]; /* 6 is arbitrary, but big enough */
	int     pid;
} ExecItem;

ExecItem prog_list[] = {
	{ NULL, "terminal", { "xterm", NULL }, 0 },
	{ NULL, "flagup", { "xterm", "-e", MAIL_PROG, NULL }, 0 },
	{ NULL, "calculator", { "xcalc", NULL }, 0 },
	{ NULL, "xlogo64", { "foo", NULL }, 0 }
};

XtAppContext app; /* application context for the whole program */
GC           gc;  /* used to render pixmaps in the widgets */

void reset (int);
void reset_btn (XtPointer, XtIntervalId *);
void redraw_button (Widget, XtPointer, XtPointer);
void exec_prog (Widget, XtPointer, XtPointer);

main (int argc, char *argv[])
{
	Widget toplevel, rowcol;
	Pixmap pixmap;
	Pixel  fg, bg;
	Arg    args[8];
	int    i, n;
	
	/* we want to be notified when child programs die */
	(void) signal (SIGCHLD, reset);
	
	XtSetLanguageProc (NULL, NULL, NULL);

	/* For X11R6, replace with XtVaOpenApplication() */
	toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0, &argc, argv, NULL, NULL);
					
	n = 0;
	XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
	rowcol = XmCreateRowColumn (toplevel, "rowcol", args, n);
	
	/* get the foreground and background colors of the rowcol
	** so the gc (DrawnButtons) will use them to render pixmaps.
	*/
	
	XtVaGetValues (rowcol, XmNforeground, &fg, XmNbackground, &bg, NULL);
	
	gc = XCreateGC (XtDisplay (rowcol), RootWindowOfScreen (XtScreen (rowcol)), NULL, 0);
	
	XSetForeground (XtDisplay (rowcol), gc, fg);
	XSetBackground (XtDisplay (rowcol), gc, bg);
	
	for (i = 0; i < XtNumber (prog_list); i++) {
		/* the pixmap is taken from the name given in the structure */
		pixmap = XmGetPixmap (XtScreen (rowcol), prog_list[i].pixmap_file, fg, bg);
		
		/* Create a drawn button 64x64 (arbitrary, but sufficient)
		** shadowType has no effect till pushButtonEnabled is false.
		*/
		
		n = 0;
		XtSetArg (args[n], XmNwidth, 64); n++;
		XtSetArg (args[n], XmNheight, 64); n++;
		XtSetArg (args[n], XmNpushButtonEnabled, True); n++;
		XtSetArg (args[n], XmNshadowType, XmSHADOW_ETCHED_OUT); n++;
		prog_list[i].drawn_w = XmCreateDrawnButton (rowcol, "dbutton", args, n);
		
		XtManageChild (prog_list[i].drawn_w);
		
		/* if this button is selected, execute the program */
		XtAddCallback (prog_list[i].drawn_w, XmNactivateCallback, exec_prog, (XtPointer) &prog_list[i]);
		/* when the resize and expose events come, redraw pixmap */
		XtAddCallback (prog_list[i].drawn_w, XmNexposeCallback, redraw_button, (XtPointer) pixmap);
		XtAddCallback (prog_list[i].drawn_w, XmNresizeCallback, redraw_button, (XtPointer) pixmap);
	}

	XtManageChild (rowcol);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* redraw_button() -- draws the pixmap into its DrawnButton
** using the global GC.  Get the width and height of the pixmap
** being used so we can either center it in the button or clip it.
*/
void redraw_button (Widget button, XtPointer client_data, XtPointer call_data)
{
	Pixmap       pixmap = (Pixmap) client_data;
	int          destx, desty;
	int          unused;
	unsigned int srcx, srcy, pix_w, pix_h;
	int          drawsize, border;
	Dimension    bdr_w, w_width, w_height;
	short        hlthick, shthick;
	Window       root;
	XmDrawnButtonCallbackStruct *cbs = (XmDrawnButtonCallbackStruct *) call_data;

	/* get width and height of the pixmap */
	XGetGeometry (XtDisplay (button), pixmap, &root, &unused, &unused, &pix_w, &pix_h, &srcx, &srcx);
	
	/* get the values of all the resources that affect the entire
	** geometry of the button.
	*/
	XtVaGetValues (button, XmNwidth, &w_width,
			       XmNheight, &w_height,
			       XmNborderWidth, &bdr_w,
			       XmNhighlightThickness, &hlthick,
			       XmNshadowThickness, &shthick,
			       NULL);
			       
	/* calculate available drawing area, width first */
	border = bdr_w + hlthick + shthick;
	
	/* if window is bigger than pixmap, center it; else clip pixmap */
	drawsize = w_width - 2 * border;
	
	if (drawsize > pix_w) {
		srcx = 0;
		destx = (drawsize - pix_w) / 2 + border;
	}
	else {
		srcx = (pix_w - drawsize) / 2;
		pix_w = drawsize;
		destx = border;
	}

	drawsize = w_height - 2 * border;
	
	if (drawsize > pix_h) {
		srcy = 0;
		desty = (drawsize - pix_h) / 2 + border;
	}
	else {
		srcy = (pix_h - drawsize) / 2;
		pix_h = drawsize;
		desty = border;
	}
	
	XCopyArea (XtDisplay (button), pixmap, cbs->window, gc, srcx, srcy, pix_w, pix_h, destx, desty);
}

/* exec_proc() -- the button has been pressed; fork() and call
** execvp() to start up the program.  If the fork or the execvp
** fails (program not found?), the sigchld catcher will get it
** and clean up.  If the program is successful, set the button's
** sensitivity to False (to prevent the user from execing again)
** and set pushButtonEnabled to False to allow shadowType to work.
*/
void exec_prog (Widget drawn_w, XtPointer client_data, XtPointer call_data)
{
	ExecItem *program = (ExecItem *) client_data;
	XmDrawnButtonCallbackStruct *cbs = (XmDrawnButtonCallbackStruct *) call_data;

	switch (program->pid = fork ()) {
		case 0:  /* child */
			execvp (program->exec_argv[0], program->exec_argv);
			perror (program->exec_argv[0]); /* command not found? */
			_exit (255);
		case -1:
			printf ("fork() failed.\n");
	}
	
	/* The child is off executing program... parent continues */
	if (program->pid > 0) {
		XtVaSetValues (drawn_w, XmNpushButtonEnabled, False, NULL);
		XtSetSensitive (drawn_w, False);
	}
}

/* reset() -- a program died, so find out which one it was and
** reset its corresponding DrawnButton widget so it can be reselected
*/
void reset (int unused)
{
	int pid, i;
#ifdef    SYSV
	int status;
#else  /* SYSV */
	union wait status;
#endif /* SYSV */
	
	if ((pid = wait (&status)) != -1) {
		for (i = 0; i < XtNumber (prog_list); i++)
			if (prog_list[i].pid == pid) {
				/* program died -- now reset item. But not here! */
				XtAppAddTimeOut (app, (unsigned long) 0, reset_btn, 
				                 (XtPointer) prog_list[i].drawn_w);
				break;
			}
	}

	(void) signal (SIGCHLD, reset);
}

/* reset_btn() -- reset the sensitivity and pushButtonEnabled resources
** on the drawn button.  This cannot be done within the signal
** handler or we might step on an X protocol packet since signals are
** asynchronous.  This function is safe because it's called from a timer
*/
void reset_btn (XtPointer closure, XtIntervalId *id) 
{
	Widget drawn_w = (Widget) closure;

	XtVaSetValues (drawn_w, XmNpushButtonEnabled, True, NULL);
	XtSetSensitive (drawn_w, True);
	XmUpdateDisplay (drawn_w);
}  
