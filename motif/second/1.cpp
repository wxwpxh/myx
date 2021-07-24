
#include <stdio.h> 
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h> 
#include <Xm/PushB.h> 


void activateCB(Widget w,XtPointer client_data,XtPointer call_data)
{
	printf("PushButton selected.\n");
	exit(0);
} 



int main( int argc,char **argv)

{
	//define variant
	Widget toplevel,mylabel,myform,mybutton;
	XtAppContext  app_context; 
	Arg args[10];
	int i;
	//X  initialize
	toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);

	// create X component--------- myform
	i=0;   
	XtSetArg(args[i],XmNwidth,160);i++; 
	XtSetArg(args[i],XmNheight,100);i++; 
	myform=XtCreateManagedWidget("Form",xmFormWidgetClass,toplevel,args,i);

	// create X component--------- mybutton
	XmString bnt_text=XmStringCreateLtoR((char *)"EXIT",XmSTRING_DEFAULT_CHARSET);
	i=0;
	XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
	XtSetArg(args[i],XmNheight,30);i++;
	XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
	XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
	XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++; 
	XtSetArg(args[i],XmNwidth,160);i++;
 	mybutton=XtCreateManagedWidget("button",xmPushButtonWidgetClass,toplevel,args,i);

 	XtManageChild(mybutton);
	XtAddCallback(mybutton,XmNactivateCallback,activateCB,NULL);
	XtRealizeWidget(toplevel);
	XtAppMainLoop(app_context);

	return 0;
}




