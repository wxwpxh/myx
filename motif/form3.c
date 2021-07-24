/*
gcc -o form3 form3.c -lXm -lXt -lX11

*/

#include <Xm/Xm.h>  
#include <Xm/PushB.h>  
#include <Xm/Form.h>

int main(int argc, char **argv) 
    
{

  XtAppContext app;
  Widget   top_wid, form,
	       button1, button2, 
           button3, button4;
  int n=0;
 
	
  top_wid = XtVaAppInitialize(&app, "Form3",
        NULL, 0, &argc, argv, NULL, NULL);
        
  /* create form and child buttons */
  
  form = XtVaCreateManagedWidget("form", xmFormWidgetClass, 
      top_wid, NULL);
  
  button1 = XtVaCreateManagedWidget("Button 1",
      xmPushButtonWidgetClass, form,
      /* attach to top, left of form */
      XmNtopAttachment, XmATTACH_FORM, 
      XmNleftAttachment, XmATTACH_FORM,
      NULL); 
  
 	
  button2 = XtVaCreateManagedWidget("Button 2",
      xmPushButtonWidgetClass, form,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, button1,
      XmNleftAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
      XmNrightWidget, button1,
      NULL);
  
  button3 = XtVaCreateManagedWidget("Button 3",
      xmPushButtonWidgetClass, form,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, button1,
      NULL);

  
  button4 = XtVaCreateManagedWidget("Button 4",
      xmPushButtonWidgetClass, form,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, button3,
      XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
      XmNleftWidget, button3,
      NULL);

 
  XtRealizeWidget (top_wid); 
  XtAppMainLoop (app); 
  
  return 0;

}
