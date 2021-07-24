      
#include <stdio.h> 
#include <stdlib.h>
 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
 
 



void activateCB(Widget w,caddr_t client_data,caddr_t call_data)
{
 printf("PushButton selected.\n");
 exit(0);
} 

void activateCB1(Widget w,caddr_t client_data,caddr_t call_data)
{
 printf("PushButton selected.\n");
 exit(0);
} 



int main( int argc,char **argv)

 {
 
  Widget
  toplevel,scrolled_text,myform,mymenu,mypanel,mypanel2,cascadeb1,c1b1,c1b2;  
  Widget command,cascadeb2,c2b1;   XtAppContext  app_context; 
  Arg args[10];
  int i;
   //define variant
 


 
  toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

  i=0;   
  XtSetArg(args[i],XmNwidth,160);i++; 
  XtSetArg(args[i],XmNheight,100);i++; 
 
  myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i);
  // create X component--------- myform

  i=0;   
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
  mymenu=XmCreateMenuBar(myform,(char*)"mymenu",args,i);
  XtManageChild(mymenu);


  i=0;    
  mypanel=XmCreatePulldownMenu(mymenu,(char*)"mypanel",args,i);
                                              
  i=0;  
  XtSetArg(args[i],XmNsubMenuId,mypanel);i++; 
  cascadeb1=XmCreateCascadeButton(mymenu,(char*)"FILE",args,i);
  XtManageChild(cascadeb1);

  i=0;    
  mypanel2=XmCreatePulldownMenu(mymenu,(char*)"mypanel2",args,i);
                                              
  i=0;  
  XtSetArg(args[i],XmNsubMenuId,mypanel2);i++; 
  cascadeb2=XmCreateCascadeButton(mymenu,(char*)"EDIT",args,i);
  XtManageChild(cascadeb2);
  
  XmString bnt_text=XmStringCreateLtoR((char*)"EXIT",XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  c1b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i);
  // create X component--------- mybutton    
  XtAddCallback(c1b1,XmNactivateCallback,(XtCallbackProc)activateCB,(XtPointer)NULL);  
  XtManageChild(c1b1);


  bnt_text=XmStringCreateLtoR((char*)"OPEN",XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  c1b2=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i);
  // create X component--------- mybutton    
  XtAddCallback(c1b2,XmNactivateCallback,(XtCallbackProc)activateCB,NULL);  
  XtManageChild(c1b2);



  bnt_text=XmStringCreateLtoR((char*)"COPY",XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  c2b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel2,args,i);
  XtManageChild(c2b1);
  


  XtVaSetValues(myform,XmNmenuBar,mymenu,NULL);
  //set the mymenu as MenuBar of the main window myform


  i=0;   
  XtSetArg(args[i],XmNrows,20);i++; 
  XtSetArg(args[i],XmNcolumns,80);i++; 
  XtSetArg(args[i],XmNeditable,False);i++;   
  XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++;  
  scrolled_text=XmCreateScrolledText(myform,(char*)"scolled_text",args,i);
  XtManageChild(scrolled_text);



  bnt_text=XmStringCreateLtoR((char*)"command",XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNpromptString,bnt_text);i++; 
  command=XmCreateCommand(myform,(char*)"command",args,i);
  XtManageChild(command);

  XmMainWindowSetAreas(myform,mymenu,command,NULL,NULL,XtParent(scrolled_text));
 

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  return 0; 

}




