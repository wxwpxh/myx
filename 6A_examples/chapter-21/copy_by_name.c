/* copy_by_name.c -- demonstrate clipboard copies "by-name".
** Copying by name requires that the copy *to* clipboard
** functions use the same window as the copy *from* clipboard
** functions. This is a restriction placed on the API by the
** toolkit, not by the ICCCM.
*/

#include <Xm/CutPaste.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

static void copy_by_name(Widget, int *, int *, int *);
static void to_clipbd(Widget, XtPointer, XtPointer);
static void from_clipbd(Widget, XtPointer, XtPointer);
Widget      toplevel;

main (int argc, char *argv[])
{
	Widget       rowcol, button;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	/* Initialize toolkit, application context and toplevel shell */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
	
	/* manage two buttons in a RowColumn widget */
	rowcol = XmCreateRowColumn (toplevel, "rowcol", NULL, 0);
	/* button1 copies to the clipboard */
	button = XmCreatePushButton (rowcol, "Copy To Clipboard", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, to_clipbd, NULL);
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

static void copy_by_name (Widget widget, int *data_id, int *private_id, int *reason)
{
	Display   *dpy = XtDisplay (toplevel);
	Window     window = XtWindow (toplevel);
	static int cnt;
	int        status;
	char       buf[32];
	
	printf ("Copy by name:\n\treason: %s, id: %d, data_id: %d\n",
		*reason == XmCR_CLIPBOARD_DATA_REQUEST? "request" : "delete",
		*private_id, *data_id);
		
	if (*reason == XmCR_CLIPBOARD_DATA_REQUEST) {
		sprintf (buf, "stuff-%d", ++cnt); /* make each copy unique */
		
		do
			status = XmClipboardCopyByName (dpy, window, *data_id, buf, 
						      strlen (buf)+1, *private_id = cnt);
		while (status != ClipboardSuccess);
	}
}

/* copy data to clipboard */
static void to_clipbd (Widget widget, XtPointer client_data, XtPointer call_data)
{
	long     item_id = 0; /* clipboard item id */
	int      status;
	XmString clip_label;
	Display *dpy = XtDisplay (toplevel);
	Window   window = XtWindow (toplevel);
	
	clip_label = XmStringCreateLocalized ("to_clipbd");
	
	/* start a copy. retry till unlocked */
	do
		status = XmClipboardBeginCopy (dpy, window, clip_label, widget, 
					       copy_by_name, &item_id);
	while (status == ClipboardLocked);
	
	/* copy by name by passing NULL as the "data", copy_by_name() as
	** the callback and "widget" as the widget.
	*/
	do
		status = XmClipboardCopy (dpy, window, item_id, "STRING", NULL, 8L, 0, NULL);
	while (status == ClipboardLocked);
	
	/* end the copy */
	do
		status = XmClipboardEndCopy (dpy, window, item_id);
	while (status == ClipboardLocked);
}

static void from_clipbd (Widget widget, XtPointer client_data, XtPointer call_data)
{
	int      status;
	unsigned total_bytes;
	unsigned long received;
	char    *data = NULL, buf[32];
	Display *dpy = XtDisplay (toplevel);
	Window   window = XtWindow (toplevel);
	
	do
		status = XmClipboardStartRetrieve (dpy, window, CurrentTime);
	while (status == ClipboardLocked);
	
	/* initialize data to contain at least one byte. */
	data = XtMalloc (1);
	total_bytes = 1;
	
	do {
		buf[0] = 0;
		
		/* retrieve data from clipboard -- if locked, try again */
		status = XmClipboardRetrieve (dpy, window, "STRING", buf, 
					      sizeof (buf), &received, NULL);
		if (status == ClipboardNoData) {
			puts ("No data on the clipboard");
			break;
		}
		
		/* reallocate data to contain enough space for everything */
		if (!(data = XtRealloc (data, total_bytes + received))) {
			XtError ("Can't allocate space for data");
			break; /* XtError may or may not return */
		}

		/* copy buf into data. strncpy() does not NULL terminate */
		strncpy (&data[total_bytes-1], buf, received);
		total_bytes += received;
	} while (status == ClipboardTruncate);

	data[total_bytes-1] = 0; /* NULL terminate */

	if (status == ClipboardSuccess)
		printf ("Retrieved \"%s\" from clipboard (%d bytes)\n", data, total_bytes);

	status = XmClipboardEndRetrieve (dpy, window);
}
