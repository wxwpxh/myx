/* help_text.c:
** Create a simple main window that contains a sample (dummy) work
** area and a menubar. The menubar contains two items: File and Help.
** The items in the Help pulldown call help_cb(), which pops up a
** home-made dialog that displays predefined help texts. The purpose
** of this program is to demonstrate how one might approach the
** problem displaying a large amount of text in a dialog box.
*/

#include <stdio.h>
#include <ctype.h>
#include <Xm/DialogS.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <Xm/PushBG.h>
#include <Xm/LabelG.h>
#include <Xm/PanedW.h>

/* The following help text information is a continuous stream of characters
** that will all be placed in a single ScrolledText object. If a specific
** newline is desired, you must do that yourself. See "index_help" below.
*/
String context_help[] = { "This is context-sensitive help. Well, not really, but such", 
			  "help text could easily be generated by a real help system.",
			  "All you really need to do is obtain information from the user",
			  "about the widget from which he needs help, or perhaps prompt",
			  "for other application-specific contexts.",
			  NULL };
String window_help[] = { "Each of the windows in your application should have an",
			 "XmNhelpCallback associated with it so you can monitor when",
			 "the user presses the Help key over any particular widget.",
			 "This is another way to provide context-sensitive help.",
			 "The MenuBar should always have a Help entry at the far right",
			 "that provides help for most aspects of the program, including",
			 "the user interface. By providing different levels of help",
			 "indexing, you can provide multiple stages of help, making the",
			 "entire help system easier to use.",
			 NULL};
String index_help[] = { "This is a small demonstration program, so there is very little",
			"material to provide an index. However, an index should contain",
			"a summary of the type of help available. For example, we have:\n",
			"    Help On Context\n",
			"    Help On Windows\n",
			"    This Index\n", 
			"\n", 
			"Higher-end applications might also provide a tutorial.",
			NULL};
String *help_texts[] = {context_help, window_help, index_help};

main (int argc, char *argv[])
{
	XtAppContext app;
	Widget       toplevel, rc, main_w, menubar, w, label_g;
	void         help_cb(Widget, XtPointer, XtPointer);
	void         file_cb(Widget, XtPointer, XtPointer);
	XmString     str1, str2, str3;
	Widget      *cascade_btns;
	int          num_btns;
	Arg          args[2];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);   

	/* the main window contains the work area and the menubar */
	main_w = XmCreateMainWindow (toplevel, "main_w", NULL, 0);
	
	/* Create a simple MenuBar that contains two cascade buttons */
	str1 = XmStringCreateLocalized ("File");
	str2 = XmStringCreateLocalized ("Help");
	menubar = XmVaCreateSimpleMenuBar (main_w, "main_w", XmVaCASCADEBUTTON, str1, 'F',
							     XmVaCASCADEBUTTON, str2, 'H',
							     NULL);
	XmStringFree (str1);
	XmStringFree (str2); 
	
	/* create the "File" pulldown menu -- callback is file_cb() */
	str1 = XmStringCreateLocalized ("New");
	str2 = XmStringCreateLocalized ("Open");
	str3 = XmStringCreateLocalized ("Quit");
	XmVaCreateSimplePulldownMenu (menubar, "file_menu", 0, file_cb, 
					       XmVaPUSHBUTTON, str1, 'N', NULL, NULL, 
					       XmVaPUSHBUTTON, str2, 'O', NULL, NULL,
					       XmVaSEPARATOR,
					       XmVaPUSHBUTTON, str3, 'Q', NULL, NULL,
					       NULL);
					       
	XmStringFree (str1);
	XmStringFree (str2);
	XmStringFree (str3);
	
	/* create the "Help" menu -- callback is help_cb() */
	str1 = XmStringCreateLocalized ("On Context");
	str2 = XmStringCreateLocalized ("On Window");
	str3 = XmStringCreateLocalized ("Index");
	w = XmVaCreateSimplePulldownMenu (menubar, "help_menu", 1, help_cb,
						   XmVaPUSHBUTTON, str1, 'C', NULL, NULL,
						   XmVaPUSHBUTTON, str2, 'W', NULL, NULL,
						   XmVaPUSHBUTTON, str3, 'I', NULL, NULL,
						   NULL);
	XmStringFree (str1);
	XmStringFree (str2);
	XmStringFree (str3);
	
	/* Identify the Help Menu for the MenuBar */
	XtVaGetValues (menubar, XmNchildren, &cascade_btns, XmNnumChildren, &num_btns, NULL);
	XtVaSetValues (menubar, XmNmenuHelpWidget, cascade_btns[num_btns-1], NULL);
	XtManageChild (menubar);
	
	/* the work area for the main window -- just create dummy stuff */
	rc = XmCreateRowColumn (main_w, "rc", NULL, 0);
	str1 = XmStringCreateLocalized ("\n This is an Empty\nSample Control Area\n ");
	XtSetArg (args[0], XmNlabelString, str1); 
	label_g = XmCreateLabelGadget (rc, "label", args, 1);
	XtManageChild (label_g);
	XmStringFree (str1);
	
	XtManageChild (rc);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback for all the entries in the File pulldown menu. */
void file_cb (Widget w, XtPointer client_data, XtPointer call_data)
{
	int item_no = (int) client_data;
	
	if (item_no == 2)
		/* the Quit menu button */
		exit (0);
		
	printf ("Item %d (%s) selected\n", item_no + 1, XtName (w));
}

/* climb widget tree until we get to the top. Return the Shell */
Widget GetTopShell (Widget w)
{
	while (w && !XtIsWMShell (w))
		w = XtParent (w);
	return w;
}

#include "info.xbm" 

/* bitmap data used by our dialog */
/* callback for all the entries in the Help pulldown menu.
** Create a dialog box that contains control and action areas.
*/
void help_cb (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget    help_dialog, pane, text_w, form, sep, widget, label;
	void      DestroyShell(Widget, XtPointer, XtPointer);
	Pixmap    pixmap;
	Pixel     fg, bg; 
	Arg       args[10];
	int       n = 0;
	int       i;
	char     *p, buf[BUFSIZ];
	int       item_no = (int) client_data; 
	Dimension h;
	
	/* Set up a DialogShell as a popup window. Set the delete
	** window protocol response to XmDESTROY to make sure that
	** the window goes away appropriately. Otherwise, it's XmUNMAP
	** which means it'd be lost forever, since we're not storing
	** the widget globally or statically to this function. 
	*/

	i = 0;
	XtSetArg (args[i], XmNdeleteResponse, XmDESTROY); i++;	
	help_dialog = XmCreateDialogShell (GetTopShell (w), "Help", args, i);

	/* Create a PanedWindow to manage the stuff in this dialog. */
	XtSetArg (args[0], XmNsashWidth, 1);
	
	/* PanedWindow won't let us set these to 0! */
	XtSetArg (args[1], XmNsashHeight, 1);
	
	/* Make small so user doesn't try to resize */ 
	pane = XmCreatePanedWindow (help_dialog, "pane", args, 2);
	
	/* Create a RowColumn in the form for Label and Text widgets.
	** This is the control area. 
	*/
	
	form = XmCreateForm (pane, "form1", NULL, 0);
	XtVaGetValues (form, /* once created, we can get its colors */
		       XmNforeground, &fg,
		       XmNbackground, &bg,
		       NULL);
		       
	/* create the pixmap of the appropriate depth using the colors 
	** that will be used by the parent (form). 
	*/
	
	pixmap = XCreatePixmapFromBitmapData (XtDisplay (form),
					      RootWindowOfScreen (XtScreen (form)),
					      (char *) info_bits, info_width, info_height, fg, bg,
					      DefaultDepthOfScreen (XtScreen (form)));
					      
	/* Create a label gadget using this pixmap */
	n = 0;
	XtSetArg (args[n], XmNlabelType, XmPIXMAP); n++;
	XtSetArg (args[n], XmNlabelPixmap, pixmap); n++;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++; 
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	label = XmCreateLabelGadget (form, "label", args, n);
	XtManageChild (label); 
	
	/* prepare the text for display in the ScrolledText object 
	** we are about to create. 
	*/
	
	for (p = buf, i = 0; help_texts[item_no][i]; i++) {
		p += strlen (strcpy (p, help_texts[item_no][i]));
		
		if (!isspace (p[-1]))
			/* spaces, tabs and newlines are spaces. */
			
			*p++ = ' '; /* lines are concatenated together, insert a space */
	}
		
	*--p = 0; /* get rid of trailing space... */

	n = 0;
	XtSetArg (args[n], XmNscrollVertical, True); n++; 
	XtSetArg (args[n], XmNscrollHorizontal, False); n++;
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
	XtSetArg (args[n], XmNeditable, False); n++;
	XtSetArg (args[n], XmNcursorPositionVisible, False); n++;
	XtSetArg (args[n], XmNwordWrap, True); n++;
	XtSetArg (args[n], XmNvalue, buf); n++;
	XtSetArg (args[n], XmNrows, 5); n++;
	text_w = XmCreateScrolledText (form, "help_text", args, n);
		
	/* Attachment values must be set on the Text widget's PARENT, 
	** the ScrolledWindow. This is the object that is positioned. 
	*/ 
		
	XtVaSetValues (XtParent (text_w), XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, label,
					  XmNtopAttachment, XmATTACH_FORM,
					  XmNrightAttachment, XmATTACH_FORM,
					  XmNbottomAttachment, XmATTACH_FORM,
					  NULL);
	XtManageChild (text_w);
	XtManageChild (form);
	
	/* Create another form to act as the action area for the dialog */
	
	XtSetArg (args[0], XmNfractionBase, 5);
	form = XmCreateForm (pane, "form2", args, 1);
	
	/* The OK button is under the pane's separator and is 
	** attached to the left edge of the form. It spreads from
	** position 0 to 1 along the bottom (the form is split into 
	** 5 separate grids via XmNfractionBase upon creation). 
	*/
		
	widget = XmCreatePushButtonGadget (form, "OK", NULL, 0);
		
	XtVaSetValues (widget, XmNtopAttachment, XmATTACH_FORM, 
			       XmNbottomAttachment, XmATTACH_FORM,        
			       XmNleftAttachment, XmATTACH_POSITION,        
			       XmNleftPosition, 1, 
			       XmNrightAttachment, XmATTACH_POSITION, 
			       XmNrightPosition, 2, 
			       XmNshowAsDefault, True,
			       XmNdefaultButtonShadowThickness, 1,
			       NULL);
	XtManageChild (widget);
	XtAddCallback (widget, XmNactivateCallback, DestroyShell, help_dialog);
		
	/* This is created with its XmNsensitive resource set to False 
	** because we don't support "more" help. However, this is the 
	** place to attach it to if there were any more. 
	*/
		
	widget = XmCreatePushButtonGadget (form, "More", NULL, 0);
	XtVaSetValues (widget, XmNsensitive, False, 
			       XmNtopAttachment, XmATTACH_FORM,
			       XmNbottomAttachment, XmATTACH_FORM,
			       XmNleftAttachment, XmATTACH_POSITION,
			       XmNleftPosition, 3,
			       XmNrightAttachment, XmATTACH_POSITION,
			       XmNrightPosition, 4,
			       XmNshowAsDefault, False,
			       XmNdefaultButtonShadowThickness, 1, 
			       NULL);
	XtManageChild (widget);
		
	/* Fix the action area pane to its current height -- never let it resize */
	XtManageChild (form);
		
	XtVaGetValues (widget, XmNheight, &h, NULL);
	XtVaSetValues (form, XmNpaneMaximum, h, XmNpaneMinimum, h, NULL);
		
	/* This also pops up the dialog, as it is the child of a DialogShell */
		
	XtManageChild (pane);
}

/* The callback function for the "OK" button. Since this is not a 
** predefined Motif dialog, the "widget" parameter is not the dialog
** itself. That is only done by Motif dialog callbacks. Here in the
** real world, the callback routine is called directly by the widget
** that was invoked. Thus, we must pass the dialog as the client
** data to get its handle. (We could get it using GetTopShell(),
** but this way is quicker, since it's immediately available.)
*/
void DestroyShell (Widget widget, XtPointer client_data, XtPointer call_data)
{
	Widget shell = (Widget) client_data;
	
	XtDestroyWidget (shell);
}
