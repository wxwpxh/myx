/* file_menu.c -- demonstrate how to create a menu bar and pulldown
** menu using the Motif creation routines.
*/

#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>

main (int argc, char *argv[])
{
	Widget       toplevel, main_w, menu_w, file_w, cascade_w, push_b, sep_w;
	XmString     label_str;
	XtAppContext app;
	Arg          args[4];
	XrmValue     from_value;
	XrmValue     to_value;
	int          n;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);
	
	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	main_w = XmCreateMainWindow (toplevel, "main_w", args, n);
	menu_w = XmCreateMenuBar (main_w, "MenuBar", NULL, 0);
	
	/* create the "File" Menu */
	file_w = XmCreatePulldownMenu (menu_w, "FilePullDown", NULL, 0);
	
	/* create the "File" button (attach Menu via XmNsubMenuId) */
	label_str = XmStringCreateLocalized ("File");
	
	n = 0;
	XtSetArg (args[n], XmNmnemonic, 'F'); n++;
	XtSetArg (args[n], XmNlabelString, label_str); n++;
	XtSetArg (args[n], XmNsubMenuId, file_w); n++;
	cascade_w = XmCreateCascadeButton (menu_w, "File", args, n);
	XtManageChild (cascade_w);
	XmStringFree (label_str);
	
	/* Now add the menu items */
	push_b = XmCreatePushButtonGadget (file_w, "Open", NULL, 0);
	XtManageChild (push_b);
	push_b = XmCreatePushButtonGadget (file_w, "Save", NULL, 0);
	XtManageChild (push_b);
	sep_w = XmCreateSeparatorGadget (file_w, "separator", NULL, 0);
	XtManageChild (sep_w);
	push_b = XmCreatePushButtonGadget (file_w, "Exit", NULL, 0);
	XtManageChild (push_b);
	
	XtManageChild (menu_w);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
	
