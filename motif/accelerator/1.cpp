      
#include  <stdio.h> 
#include <stdlib.h>
 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
  
 Widget d_text,s_text;
 char     display_str[1000];


 
void  Display_text(Widget  w,XEvent *ev,String *params,Cardinal *num_params)
 {  
  XmTextFieldSetString(s_text,display_str);
 }
 


void collect(Widget w,caddr_t client_data,XEvent * env ,Boolean *b ) 
{ 
 char buf[1];
  

  XLookupString((XKeyEvent*)env,buf,1,NULL,NULL); 
 sprintf(display_str,"%s%s",display_str);
} 
 

void main( int argc,char **argv)

 {
  XtAccelerators accel;
  Widget toplevel,mybutton,myform,d_text,s_text;
  XtAppContext  app_context; 
  Arg args[10];
  int i; 

  static XtActionsRec one_action[]={
       {"display",Display_text}
       };

 strcpy(display_str,"");
 toplevel=XtAppInitialize(&app_context,"Xmdem
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
  
 Widget d_text,s_text;
 char     display_str[1000];


 
void  Display_text(Widget  w,XEvent ev,char * params,Cardinal num_params)
 {  
  XmTextFieldSetString(s_text,display_str);os",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 i=0;   
  
 XtSetArg(args[i],XmNwidth,150);i++; 
 XtSetArg(args[i],XmNheight,100);i++; 
 myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i);
 
 i=0;  
 s_text =XmCreateTextField(myform,"s_text",args,i);  
 XtManageChild(s_text);

 i=0;   
 XtSetArg(args[i],XmNtopWidget,s_text);i++;
 XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++; 
 d_text =XmCreateTextField(myform,"d_text",args,i);  
 XtManageChild(d_text);
 XtAddEventHandler(d_text,KeyPressMask,False,collect,NULL);
 accel=XtParseAcceleratorTable("#override\
                        <Key>:display()");
 XtVaSetValues(s_text,XmNaccelerators,accel,NULL);  
 XtAppAddActions(app_context,one_action,XtNumber(one_action));
 XtInstallAccelerators(d_text,s_text);
 
 XtRealizeWidget(toplevel); 
 XtAppMainLoop(app_context);
  

}




