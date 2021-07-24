/* wprint.c -- varargs Text messaging widget */

#include <stdio.h>
#include <stdarg.h>
#include <Xm/Text.h>

/* global variable */
Widget text_output;

void wprint (const char *fmt, ...) ;

main (int argc, char *argv[])
{
	Widget       toplevel;
	XtAppContext app;
	Arg          args[10];
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos",  NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);


	/* Create output_text as a ScrolledText window */
	n = 0;
	XtSetArg(args[n], XmNrows, 6); n++;
	XtSetArg(args[n], XmNcolumns,  80); n++;
	XtSetArg(args[n], XmNeditable, False); n++;
	XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	XtSetArg(args[n], XmNwordWrap, True); n++;
	XtSetArg(args[n], XmNscrollHorizontal, False); n++;
	XtSetArg(args[n], XmNcursorPositionVisible, False); n++;
	text_output = XmCreateScrolledText(toplevel, "text_output", args, n);
	XtManageChild (text_output);

	wprint("%s %s %ld", "Hello", "World", time((long *) 0)) ;
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/*PRINTFLIKE1*/
void wprint (const char *fmt, ...)
{
	char    msgbuf[BUFSIZ];
	static  XmTextPosition wpr_position;
	va_list data ;
	
	va_start (data, fmt);
	
#ifndef   NO_VPRINTF
	(void) vsprintf (msgbuf, fmt, data);
#else /* NO_VPRINTF */
	{
		FILE foo;
		foo._cnt = 256;
		foo._base = foo._ptr = msgbuf; /* (unsigned char *) ?? */
		foo._flag = _IOWRT + _IOSTRG;
		(void) _doprnt (fmt, args, &foo);
		*foo.ptr = `\0'; /* Null termination to the string */
	}
#endif /* NO_VPRINTF */

	va_end (args);

	XmTextInsert (text_output, wpr_position, msgbuf);
	wpr_position = wpr_position + strlen (msgbuf);
	XtVaSetValues (text_output, XmNcursorPosition, wpr_position, NULL);
	XmTextShowPosition (text_output, wpr_position);
}
