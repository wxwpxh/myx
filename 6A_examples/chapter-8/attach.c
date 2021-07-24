/* attach.c -- demonstrate how attachments work in Form widgets. */
#include <Xm/PushB.h>
#include <Xm/Form.h>

main (int argc, char *argv[])
{
	Widget       toplevel, parent, one, two, three;
	XtAppContext app;
	
	XtSetLanguageProc (NULL, NULL, NULL); 
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	parent = XmCreateForm (toplevel, "form",  NULL, 0);
	one = XmCreatePushButton (parent, "One", NULL, 0);
	two = XmCreatePushButton (parent, "Two", NULL, 0);
	three = XmCreatePushButton (parent, "Three", NULL, 0);

	XtVaSetValues (one, XmNtopAttachment,  XmATTACH_FORM, XmNleftAttachment, XmATTACH_FORM, NULL);
	XtVaSetValues (two, 
		       XmNleftAttachment, XmATTACH_WIDGET, 
		       XmNleftWidget, one, /* attach top of widget to same y coordinate as top of "one" */ 
		       XmNtopAttachment,  XmATTACH_OPPOSITE_WIDGET,
		       XmNtopWidget, one,
		       NULL);
			    
	XtVaSetValues (three, 
		       XmNtopAttachment,   XmATTACH_WIDGET,
		       XmNtopWidget, one, /* attach left of widget to same x coordinate as left side of "one" */
		       XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
		       XmNleftWidget, one,
		       NULL);
		       
	XtManageChild (one);
	XtManageChild (two);
	XtManageChild (three);
	XtManageChild (parent);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
