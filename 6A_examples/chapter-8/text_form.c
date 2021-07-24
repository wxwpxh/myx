/* text_form.c -- demonstrate how attachments work in Form widgets
* by creating a text-entry form type application.
*/

#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/Form.h>

char *prompts[] = {"Name:", "Phone:", "Address:",    "City:", "State:", "Zip Code:"};

main (int argc, char *argv[])
{
	Widget       toplevel, mainform, subform, last_subform, label, text;
	XtAppContext app;
	char         buf[32];
	int          i;
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL);

	mainform = XmCreateForm (toplevel, "mainform", NULL, 0);
	last_subform = NULL;
	
	for (i = 0; i < XtNumber (prompts); i++) {
		subform = XmCreateForm (mainform, "subform", NULL, 0);
		
		XtVaSetValues (subform,    
			       /* first one should be attached for form */
			       XmNtopAttachment, last_subform ? XmATTACH_WIDGET : XmATTACH_FORM,
			       /* others are attached to the previous subform */
			       XmNtopWidget, last_subform,
			       XmNleftAttachment, XmATTACH_FORM,
			       XmNrightAttachment, XmATTACH_FORM,
			       NULL);

		/* Note that the label here contains a colon from the prompts
		** array above.  This makes it impossible for external resources
		** to be set on these widgets.  Here, that is intentional, but
		** be careful in the general case.
		*/
		
		label = XmCreateLabelGadget (subform, prompts[i], NULL, 0);
		XtVaSetValues (label, XmNtopAttachment, XmATTACH_FORM, 
				      XmNbottomAttachment, XmATTACH_FORM,
				      XmNleftAttachment, XmATTACH_FORM, 
				      XmNalignment, XmALIGNMENT_BEGINNING,
				      NULL);
		XtManageChild (label);

		sprintf (buf, "text_%d", i);
		text = XmCreateText (subform, buf, NULL, 0);
		XtVaSetValues (text, XmNtopAttachment, XmATTACH_FORM,
				     XmNbottomAttachment, XmATTACH_FORM,
				     XmNrightAttachment, XmATTACH_FORM,
				     XmNleftAttachment, XmATTACH_WIDGET,
				     XmNleftWidget, label,
				     NULL);
		XtManageChild (text);
		
		XtManageChild (subform);
		last_subform = subform;
	}
	
	/* Now that all the forms are added, manage the main form */
	XtManageChild (mainform);

	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
