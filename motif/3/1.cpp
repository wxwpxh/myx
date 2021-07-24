#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h> 
#include <Xm/PushB.h> 
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
 



void activateCB(Widget w,XtPointer client_data,XtPointer call_data)
{
  cout<<"PushButton selected."<<endl;
 exit(0);
} 

int main( int argc,char **argv)

 {
 
  Widget toplevel,mybutton,myform;
  XtAppContext  app_context; 
  Arg args[10];
  int i;
   //define variant
 


 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 i=0;   
 XtSetArg(args[i],XmNwidth,160);i++; 
 XtSetArg(args[i],XmNheight,100);i++; 
 myform=XtCreateManagedWidget("Form",xmFormWidgetClass,toplevel,args,i);
 // create X component--------- myform
 
XmString bnt_text=XmStringCreateLtoR((char*)"EXIT",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 XtSetArg(args[i],XmNheight,30);i++;
 XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
 XtSetArg(args[i],XmNwidth,160);i++; 

mybutton=XtCreateManagedWidget("button",xmPushButtonWidgetClass,myform,args,i);   // create X component--------- mybutton
  XtAddCallback(mybutton,XmNactivateCallback,activateCB,NULL); 

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  

}




