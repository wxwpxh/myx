/* copy_retrieve.c -- simple copy and retrieve program. Two
** pushbuttons: the first places text in the clipboard, the other
** receives text from the clipboard. This just demonstrates the
** API involved.
*/

#include <Xm/CutPaste.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

static void to_clipbd(Widget, XtPointer, XtPointer);
static void from_clipbd(Widget, XtPointer, XtPointer);

main (int argc, char *argv[])
{
	Widget       toplevel, rowcol, button;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	/* Initialize toolkit, application context and session shell */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	/* manage two buttons in a RowColumn widget */
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	/* button1 copies to the clipboard */
	button = XmCreatePushButton (rowcol, "Copy To Clipboard", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, to_clipbd, "text");
	XtManageChild (button);
	/* button2 retrieves text stored in the clipboard */
	button = XmCreatePushButton (rowcol, "Retrieve From Clipboard", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, from_clipbd, NULL);
	XtManageChild (button);

	/* manage RowColumn, realize toplevel shell and start main loop */
	XtManageChild (rowcol);
	XtRealizeWidget (toplevel); 
	XtAppMainLoop (app);
}

/* copy data to clipboard. */
static void to_clipbd (Widget widget, XtPointer client_data, XtPointer call_data)
{
	long       item_id = 0; /* clipboard item id */
	int        status;
	XmString   clip_label;
	char       buf[32];
	static int cnt;
	Display   *dpy = XtDisplayOfObject (widget);
	Window     window = XtWindowOfObject (widget);
	char      *data = (char *) client_data;
	
	(void) sprintf (buf, "%s-%d", data, ++cnt); /* make each copy unique */
	
	clip_label = XmStringCreateLocalized ("to_clipbd");
	
	/* start a copy -- retry till unlocked */
	do
		status = XmClipboardStartCopy (dpy, window, clip_label, 
					       CurrentTime, NULL, NULL, &item_id);
	while (status == ClipboardLocked);
	
	XmStringFree (clip_label);
	
	/* copy the data (buf) -- pass "cnt" as private id for kicks */
	do
		status = XmClipboardCopy (dpy, window, item_id, "STRING", buf, 
					 (long) strlen (buf)+1, cnt, NULL);
	while (status == ClipboardLocked);
	
	/* end the copy */
	do
		status = XmClipboardEndCopy (dpy, window, item_id);
	while (status == ClipboardLocked);
	
	printf ("Copied \"%s\" to clipboard.\n", buf);
}

static void from_clipbd (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int      status;
	long     private_id;
	char     buf[32];
	Display *dpy = XtDisplayOfObject (widget);
	Window   window = XtWindowOfObject (widget);
	
	do
		status = XmClipboardRetrieve (dpy, window, "STRING", buf, 
					      sizeof (buf), NULL, &private_id);
	while (status == ClipboardLocked);
	
	if (status == ClipboardSuccess)
		printf ("Retrieved \"%s\" (private id = %d).\n", buf, private_id);
}
