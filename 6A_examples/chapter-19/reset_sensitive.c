#include <Xm/Xm.h>
#include <Xm/FileSB.h>

/* reset_sensitive() -- generalized routine that resets the
** sensitivity on the widget passed as the client_data parameter
** in a call to XtAddCallback().
*/
void reset_sensitive (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget reset_widget = (Widget) client_data;

	XtSetSensitive (reset_widget, True);
}

/* open_callback() -- the callback routine for when the "Open"
** menu item is selected from the "File" title in the MenuBar.
*/
void open_callback (Widget menu_item, XtPointer client_data, XtPointer call_data)
{
	Widget dialog, parent = menu_item;
	
	/* Get the window manager shell widget associated with item */
	while (!XtIsWMShell (parent))
		parent = XtParent (parent);
	
	/* turn off the sensitivity for the Open button ... */
	XtSetSensitive (menu_item, False);
	
	dialog = XmCreateFileSelectionDialog (parent, "files", NULL, 0);
	
	/* Add callback routines to respond to OK button selection here. */
	/* Make sure that if the dialog is popped down or destroyed, the
	** menu_item's sensitivity is reset.
	*/
	XtAddCallback (XtParent(dialog), /* dialog's _parent_ */
		       XmNpopdownCallback, reset_sensitive, (XtPointer) menu_item);
	XtAddCallback (dialog, XmNdestroyCallback, reset_sensitive, menu_item);
	XtManageChild (dialog);
}

