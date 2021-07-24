/* form_corners.c -- demonstrate form layout management. Just as
** in corners.c, there are four widgets each labelled top-left,
** top-right, bottom-left and bottom-right. Their positions in the
** form correspond to their names. As opposed to the BulletinBoard
** widget, the Form manages this layout management automatically by
** specifying attachment types for each of the widgets.
*/

#include <Xm/PushB.h>
#include <Xm/Form.h>

char *corners[] = {"Top Left", "Top Right", "Bottom Left", "Bottom Right"};

main (int argc, char *argv[])
{
	Widget       toplevel, form, button;
	XtAppContext app; 
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0,  &argc, argv, NULL,
					 sessionShellWidgetClass, NULL);

	form = XmCreateForm (toplevel, "form", NULL, 0);
	
	/* Attach the edges of the widgets to the Form. Which edge of
	** the widget that's attached is relative to where the widget is
	** positioned in the Form. Edges not attached default to having
	** an attachment type of XmATTACH_NONE.
	*/
	
	button = XmCreatePushButton (form, corners[0], NULL, 0);
	XtVaSetValues (button, XmNtopAttachment, XmATTACH_FORM, XmNleftAttachment, XmATTACH_FORM, NULL);
	XtManageChild (button);
	
	button = XmCreatePushButton (form, corners[1], NULL, 0);
	XtVaSetValues (button, XmNtopAttachment, XmATTACH_FORM, XmNrightAttachment, XmATTACH_FORM, NULL);
	XtManageChild (button);
	
	button = XmCreatePushButton (form, corners[2], NULL, 0);
	XtVaSetValues (button, XmNbottomAttachment, XmATTACH_FORM, XmNleftAttachment, XmATTACH_FORM, NULL);
	XtManageChild (button);
	
	button = XmCreatePushButton (form, corners[3], NULL, 0);
	XtVaSetValues (button, XmNbottomAttachment, XmATTACH_FORM, XmNrightAttachment, XmATTACH_FORM, NULL);
	XtManageChild (button);
	
	XtManageChild (form);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
