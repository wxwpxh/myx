/* popups.c -- demonstrate the use of a popup menus in an arbitrary
** widget. Display two PushButtons. The second one has a popup
** menu attached to it that is activated with the third
** mouse button.
*/

#include <Xm/LabelG.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleBG.h>
#include <Xm/ToggleB.h>
#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>
#include <Xm/FileSB.h>
#include <Xm/CascadeBG.h>

extern void exit(int);
Widget      toplevel;
void        open_dialog_box(Widget, XtPointer, XtPointer);

/* callback for pushbutton activation */
void put_string (Widget w, XtPointer client_data, XtPointer call_data)
{
	String str = (String) client_data;
	puts (str);
}

typedef struct _menu_item {
	char        *label;
	WidgetClass *class;
	char         mnemonic;
	char        *accelerator;
	char        *accel_text;
	void       (*callback)();
	XtPointer    callback_data;
	struct _menu_item *subitems;
} MenuItem;

MenuItem file_items[] = {
	{ "File Items", &xmLabelGadgetClass,      NULL, NULL, NULL, NULL, NULL, NULL },
	{ "_sep1",      &xmSeparatorGadgetClass,  NULL, NULL, NULL, NULL, NULL, NULL },
	{ "New",        &xmPushButtonGadgetClass, 'N',  NULL, NULL, put_string, "New", NULL },
	{ "Open...",    &xmPushButtonGadgetClass, 'O',  NULL, NULL, 
			open_dialog_box, (XtPointer) XmCreateFileSelectionDialog, NULL },
	{ "Save",       &xmPushButtonGadgetClass, 'S',  NULL, NULL, put_string, "Save", NULL },
	{ "Save As...", &xmPushButtonGadgetClass, 'A',  NULL, NULL, 
			open_dialog_box, (XtPointer) XmCreateFileSelectionDialog, NULL },
	{ "Exit",       &xmPushButtonGadgetClass, 'x', "Ctrl<Key>C", "Ctrl+C", exit, NULL, NULL }, 
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

/* build_menu.c -- Demonstrate the BuildMenu() routine and
** how it can be used to build pulldown -and- pullright menus.
** Menus are defined by declaring an array of MenuItem structures.
*/

/* Pulldown menus are built from cascade buttons, so this function
** also includes pullright menus. Create the menu, the cascade button
** that owns the menu, and then the submenu items.
*/
Widget BuildMenu (Widget parent, int menu_type, char *menu_title, char menu_mnemonic, 
                          Boolean tear_off, MenuItem *items)
{
	Widget   menu, cascade, widget;
	int      i;
	XmString str;
	Arg      args[8];
	int      n;

	if (menu_type == XmMENU_PULLDOWN)	
		menu = XmCreatePulldownMenu (parent, "_pulldown", NULL, 0);
	else {
		n = 0;
		XtSetArg (args[n], XmNpopupEnabled, XmPOPUP_AUTOMATIC_RECURSIVE); n++;
		menu = XmCreatePopupMenu (parent, "_popup", args, n);
	}
	
	if (tear_off) 
		XtVaSetValues (menu, XmNtearOffModel, XmTEAR_OFF_ENABLED, NULL);
		
	if (menu_type == XmMENU_PULLDOWN) {
		str = XmStringCreateLocalized (menu_title);
		n = 0;
		XtSetArg (args[n], XmNsubMenuId, menu); n++;
		XtSetArg (args[n], XmNlabelString, str); n++;
		XtSetArg (args[n], XmNmnemonic, menu_mnemonic); n++;
		cascade = XmCreateCascadeButtonGadget (parent, menu_title, args, n);
		XtManageChild (cascade);
		XmStringFree (str);
	}

	/* Now add the menu items */
	for (i = 0; items[i].label != NULL; i++) {
		/* If subitems exist, create the pull-right menu by calling this
		** function recursively. Since the function returns a cascade
		** button, the widget returned is used.. 
		*/
		
		if (items[i].subitems)
			widget = BuildMenu (menu, XmMENU_PULLDOWN, items[i].label, 
                                                items[i].mnemonic, tear_off, items[i].subitems);
		else
			widget = XtVaCreateManagedWidget (items[i].label, *items[i].class, menu, NULL);

		/* Whether the item is a real item or a cascade button with a
		** menu, it can still have a mnemonic.
		*/
		if (items[i].mnemonic)
			XtVaSetValues (widget, XmNmnemonic, items[i].mnemonic, NULL);
			
		/* any item can have an accelerator, except cascade menus. But,
		** we don't worry about that; we know better in our declarations.
		*/
		if (items[i].accelerator) {
			str = XmStringCreateLocalized (items[i].accel_text);
			
			XtVaSetValues (widget, XmNaccelerator, items[i].accelerator, XmNacceleratorText, 
				       str, NULL);
			XmStringFree (str);
		}
		
		if (items[i].callback) {
			String resource;
			
			if (XmIsToggleButton(widget) || XmIsToggleButtonGadget(widget))
				resource = XmNvalueChangedCallback;
			else
				resource = XmNactivateCallback;
				
			XtAddCallback(widget, resource, items[i].callback,(XtPointer) items[i].callback_data);
		}
	}

	return (menu_type == XmMENU_PULLDOWN) ? cascade : menu ;
}

main (int argc, char *argv[])
{
	Widget       button, rowcol, popup;
	XtAppContext app; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
	sessionShellWidgetClass, NULL);
	
	/* Build a RowColumn to contain two PushButtons */
	rowcol = XtVaCreateManagedWidget ("rowcol", xmRowColumnWidgetClass, toplevel, NULL);
	
	/* The first PushButton is a gadget, just to show that the Motif 2.x popup
	** routines work for Gadgets as well as widgets.
	*/
	button = XmCreatePushButtonGadget (rowcol, "Button 1", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, put_string, "Button 1");
	XtManageChild (button);
	
	/* This PushButton is a widget.
	*/
	button = XmCreatePushButton (rowcol, "Button 2", NULL, 0);
	XtAddCallback (button, XmNactivateCallback, put_string, "Button 2");
	XtManageChild (button);
	
	/* build the menu... */
	popup = BuildMenu(rowcol, XmMENU_POPUP, "Stuff", NULL, True, file_items);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
/* open_dialog_box() -- callback for some of the menu items declared
** in the MenuItem struct. The client data is the creation function
** for the dialog. Associate the dialog with the menu
** item via XmNuserData so we don't have to keep a global and
** don't have to repeatedly create one.
*/
void open_dialog_box (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget (*func)() = (Widget (*)()) client_data;
	Widget   dialog = NULL;
	
	/* first see if this menu item's dialog has been created yet */
	XtVaGetValues(w, XmNuserData, &dialog, NULL);
	
	if (!dialog) {
		/* menu item hasn't been chosen yet -- create the dialog.
		** Use the toplevel as the parent because we don't want the
		** parent of a dialog to be a menu item.
		*/
		dialog = (*func)(toplevel, "dialog", NULL, 0);
		XtVaSetValues (XtParent (dialog), XmNtitle, XtName (w), NULL);
		XtVaSetValues (dialog, XmNautoUnmanage, True, NULL);

		/* store the newly created dialog in the XmNuserData for the menuu
		** item for easy retrieval next time. (see get-values above.)
		*/
		XtVaSetValues (w, XmNuserData, dialog, NULL);
	}
	XtManageChild (dialog);
	
	/* If the dialog was already open, XtPopup does nothing. In
	** this case, at least make sure the window is raised to the top
	** of the window tree (or as high as it can get).
	*/
	XRaiseWindow (XtDisplay (dialog), XtWindow (XtParent (dialog)));
}
	
