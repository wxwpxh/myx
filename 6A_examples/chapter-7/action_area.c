/* action_area.c -- demonstrate how CreateActionArea() can be used 
** in a real application. Create what would otherwise be identified 
** as a PromptDialog, only this is of our own creation. As such, 
** we provide a TextField widget for input. When the user presses 
** Return, the OK button is activated. 
*/
#include <Xm/DialogS.h>
#include <Xm/PushBG.h>
#include <Xm/PushB.h>
#include <Xm/LabelG.h>
#include <Xm/PanedW.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>

typedef struct 
{
	char     *label;
	void    (*callback)();
	XtPointer data;
} ActionAreaItem;

static void do_dialog(Widget, XtPointer, XtPointer);
static void close_dialog(Widget, XtPointer, XtPointer);
static void activate_cb(Widget, XtPointer, XtPointer);
static void ok_pushed(Widget, XtPointer, XtPointer);
static void clear_pushed(Widget, XtPointer, XtPointer);
static void help(Widget, XtPointer, XtPointer);

main (int argc, char *argv[])
{
	Widget       toplevel, button;
	XtAppContext app; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	button = XmCreatePushButton (toplevel, "Push Me", NULL, 0);
	XtManageChild (button);
	XtAddCallback (button, XmNactivateCallback, do_dialog, NULL);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}

/* callback routine for "Push Me" button. Actually, this represents 
** a function that could be invoked by any arbitrary callback. Here, 
** we demonstrate how one can build a standard customized dialog box. 
** The control area is created here and the action area is created in 
** a separate, generic routine: CreateActionArea(). 
*/
static void do_dialog (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget   dialog, pane, rc, text_w, action_a, label_g;
	XmString string;
	Arg      args[6];
	int      n;
	Widget   CreateActionArea (Widget, ActionAreaItem *, int);
	static   ActionAreaItem action_items[] = { 
		{ "OK", ok_pushed, NULL },
		{ "Clear", clear_pushed, NULL },
		{ "Close", close_dialog, NULL },
		{ "Help", help, "Help Button" }, 
	};

	/* The DialogShell is the Shell for this dialog. Set it up so 
	** that the "Close" button in the window manager's system menu
	** destroys the shell (it only unmaps it by default). 
	*/
	
	n = 0; 
	XtSetArg (args[n], XmNtitle, "Dialog Shell"); n++;
	
	/* give arbitrary title in wm */
	XtSetArg (args[n], XmNdeleteResponse, XmDESTROY); n++;
	
	/* system menu "Close" action */ 
	dialog = XmCreateDialogShell (XtParent (w), "dialog", args, n);    
	
	/* now that the dialog is created, set the Cancel button's 
	** client data, so close_dialog() will know what to destroy. 
	*/
	
	action_items[2].data = (XtPointer) dialog;
	
	/* Create the paned window as a child of the dialog. This will
	** contain the control area and the action area 
	** (created by CreateActionArea() using the action_items above). 
	*/
	
	n = 0;
	XtSetArg (args[n], XmNsashWidth, 1); n++;
	XtSetArg (args[n], XmNsashHeight, 1); n++;
	pane = XmCreatePanedWindow (dialog, "pane", args, n);
	
	/* create the control area which contains a 
	** Label gadget and a TextField widget. 
	*/
	
	rc = XmCreateRowColumn (pane, "control_area", NULL, 0);

	string = XmStringCreateLocalized ("Type Something:");
	n = 0;
	XtSetArg (args[n], XmNlabelString, string); n++;
	label_g = XmCreateLabelGadget (rc, "label", args, n);
	XmStringFree (string);
	XtManageChild (label_g);
	
	text_w = XmCreateTextField (rc, "text-field", NULL, 0);
	XtManageChild (text_w);
	
	/* RowColumn is full -- now manage */
	XtManageChild (rc);
	
	/* Set the client data for the "OK" and "Cancel" buttons */
	action_items[0].data = (XtPointer) text_w;
	action_items[1].data = (XtPointer) text_w;
	
	/* Create the action area. */
	action_a = CreateActionArea (pane, action_items, XtNumber (action_items));
	
	/* callback for Return in TextField. Use action_a as client data */
	XtAddCallback (text_w, XmNactivateCallback, activate_cb, (XtPointer) action_a);
	
	XtManageChild (pane);
}

/* The next four functions are the callback routines for the buttons 
** in the action area for the dialog created above. Again, they are 
** simple examples, yet they demonstrate the fundamental design approach. 
*/
static void close_dialog (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget shell = (Widget) client_data;
	XtDestroyWidget (shell);
}

/* The "ok" button was pushed or the user pressed Return */
static void ok_pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget               text_w = (Widget) client_data;
	XmAnyCallbackStruct *cbs    = (XmAnyCallbackStruct *) call_data;
	char                *text   = XmTextFieldGetString (text_w);
	
	printf ("String = %s\n", text);
	XtFree (text);
}

static void clear_pushed (Widget w, XtPointer client_data, XtPointer call_data)
{
	Widget               text_w = (Widget) client_data; 
	XmAnyCallbackStruct *cbs    = (XmAnyCallbackStruct *) call_data;
	
	/* cancel the whole operation; reset to NULL. */
	XmTextFieldSetString (text_w, "");
}

static void help (Widget w, XtPointer client_data, XtPointer call_data)
{
	String string = (String) client_data;
	puts (string);
}

/* When Return is pressed in TextField widget, respond by getting 
** the designated "default button" in the action area and activate 
** it as if the user had selected it. 
*/
static void activate_cb (Widget text_w, /* user pressed Return in this widget */
			 XtPointer client_data, /* action_area passed as client data */
			 XtPointer call_data)
{
	XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *) call_data;
	Widget dflt, action_area = (Widget) client_data;
	
	/* get the "default button" from the action area... */
	XtVaGetValues (action_area, XmNdefaultButton, &dflt, NULL);
	
	if (dflt) /* sanity check -- this better work */
		/* make the default button think it got pushed using
		** XtCallActionProc(). This function causes the button
		** to appear to be activated as if the user pressed it.
		*/
	XtCallActionProc (dflt, "ArmAndActivate", cbs->event, NULL, 0);
}

#define TIGHTNESS 20

Widget CreateActionArea (Widget parent, ActionAreaItem *actions, int num_actions)
{
	Widget action_area, widget;
	int    i;
	
	action_area = XmCreateForm (parent, "action_area", NULL, 0);
	
	XtVaSetValues (action_area, XmNfractionBase, TIGHTNESS*num_actions - 1,
				    XmNleftOffset, 10,
				    XmNrightOffset, 10,
				    NULL);

	for (i = 0; i < num_actions; i++) {
		widget = XmCreatePushButton (action_area, actions[i].label, NULL, 0);
		
		XtVaSetValues (widget, XmNleftAttachment, i? XmATTACH_POSITION: XmATTACH_FORM, 
				       XmNleftPosition, TIGHTNESS*i,
				       XmNtopAttachment, XmATTACH_FORM,
				       XmNbottomAttachment, XmATTACH_FORM,
				       XmNrightAttachment, 
					   i != num_actions - 1 ? XmATTACH_POSITION : XmATTACH_FORM,
				       XmNrightPosition, TIGHTNESS * i + (TIGHTNESS - 1),
				       XmNshowAsDefault, i == 0, 
				       XmNdefaultButtonShadowThickness, 1, 
				       NULL); 

		if (actions[i].callback)
			XtAddCallback (widget, XmNactivateCallback, 
				       actions[i].callback, (XtPointer) actions[i].data);
			
		XtManageChild (widget);
		
		if (i == 0) {
			/* Set the action_area's default button to the first widget 
			** created (or, make the index a parameter to the function
			** or have it be part of the data structure). Also, set the
			** pane window constraint for max and min heights so this
			** particular pane in the PanedWindow is not resizable.
			*/
			
			Dimension height, h;
			
			XtVaGetValues (action_area, XmNmarginHeight, &h, NULL);
			XtVaGetValues (widget, XmNheight, &height, NULL);
			
			height += 2 * h; 
			
			XtVaSetValues (action_area, XmNdefaultButton, widget, 
						    XmNpaneMaximum, height, 
						    XmNpaneMinimum, height,
						    NULL);
		}
	}
	
	XtManageChild (action_area);
	
	return action_area;
}
