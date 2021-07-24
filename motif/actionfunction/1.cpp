#include <stdio.h> 
#include <stdlib.h>
 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
  
  void Quit(Widget w,XEvent *env,String *params,Cardinal *num_params){ 
    exit(0);
   } 


  void Confirm(Widget w,XEvent *env,String *params,Cardinal *num_params){
   printf("are you sure?\n");
  } 

  int main( int argc,char **argv){
   Widget toplevel,mybutton,myform;
   XtAppContext  app_context; 
   Arg args[10];
   int i=0;
   XtActionsRec two_quits[]={
       {(char *)"confirm",Confirm},
       {(char *)"quit",Quit}
    };
  
   toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
   //X  initialize
   XtSetArg(args[i],XmNwidth,160);i++; 
   XtSetArg(args[i],XmNheight,100);i++; 
   myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i);
   // create X component--------- myform
 
   XmString bnt_text=XmStringCreateLtoR((char *)"QUIT",XmSTRING_DEFAULT_CHARSET);
   i=0;
   XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
   XtSetArg(args[i],XmNheight,30);i++;
   XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
   XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
   XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
   XtSetArg(args[i],XmNwidth,160);i++; 

   mybutton=XtCreateManagedWidget("button",xmPushButtonWidgetClass,myform,args,i);   // create X component--------- mybutton
   XtOverrideTranslations(mybutton,XtParseTranslationTable("#oerrride\
                        <Btn1Down>,<Btn1Up>:confirm()\n\
                        <Btn3Down>,<Btn3Up>:quit()"));
 
   XtAppAddActions(app_context,two_quits,XtNumber(two_quits));
   XtRealizeWidget(toplevel);
   XtAppMainLoop(app_context);
   return 0;
 }




