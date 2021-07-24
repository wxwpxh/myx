      
#include  <stdio.h> 
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
  Widget mesg,win,dia_shell;
  int i;
  Arg args[10];  
  win=(Widget)client_data;
   
  i=0;
  dia_shell=XmCreateDialogShell(win,"d_shell",args,i);
  
  XmString bnt_text=XmStringCreateSimple("The fuction is not be complished");
  i=0;  
  XtSetArg(args[i],XmNmessageString,bnt_text);i++;   
  mesg=XmCreateMessageBox(dia_shell,"mesg",args,i);  
  XtManageChild(mesg);
 
} 

void exec_cmd(Widget text,XmString cmdstring)
{
 char *cmd,buf[BUFSIZ];
 XmTextPosition pos;
 FILE *fp;
 XmStringGetLtoR(cmdstring,XmSTRING_DEFAULT_CHARSET,&cmd); 
 if(!cmd||!*cmd){
   if(cmd) 
    XtFree(cmd);
   return; 
 } 
 
 if(!(fp=popen(cmd,"r"))){
  perror(cmd);
}

XtFree(cmd);
if(!fp){
 return;   
 }
pos=XmTextGetLastPosition(text);
XmTextReplace(text,0,pos,"");


for(pos=0;fgets(buf,sizeof(buf),fp);pos+=strlen(buf))
XmTextReplace(text,pos,pos,buf);

pclose(fp);
}

void exec_cmdCB(Widget w,caddr_t client_data,caddr_t call_data)
{
 XmCommandCallbackStruct *cbs=(XmCommandCallbackStruct *)call_data;
 Widget scrolltext=(Widget)client_data;
exec_cmd(scrolltext,cbs->value);

} 






void main( int argc,char **argv)

 {
 
Widget toplevel,scrolled_text,myform,mymenu,mypanel,mypanel2,cascadeb1,c1b1,c1b2;   
Widget command,cascadeb2,c2b1;  
XtAppContext  app_context; 
Arg args[10];  
int i;    //define variant  


 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 i=0;   
 //XtSetArg(args[i],XmNwidth,160);i++; 
 //XtSetArg(args[i],XmNheight,100);i++; 
 
 myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i);
 // create X component--------- myform

 i=0;   
 XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
 mymenu=XmCreateMenuBar(myform,"mymenu",args,i);
 XtManageChild(mymenu);


 i=0;    
 mypanel=XmCreatePulldownMenu(mymenu,"mypanel",args,i);
                                              
 i=0;  
 XtSetArg(args[i],XmNsubMenuId,mypanel);i++; 
 cascadeb1=XmCreateCascadeButton(mymenu,"FILE",args,i);
XtManageChild(cascadeb1);

i=0;    
 mypanel2=XmCreatePulldownMenu(mymenu,"mypanel2",args,i);
                                              
 i=0;  
 XtSetArg(args[i],XmNsubMenuId,mypanel2);i++; 
 cascadeb2=XmCreateCascadeButton(mymenu,"EDIT",args,i);
XtManageChild(cascadeb2);
  
XmString bnt_text=XmStringCreateLtoR("EXIT",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c1b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i);
  // create X component--------- mybutton    
XtAddCallback(c1b1,XmNactivateCallback,activateCB,NULL);  
XtManageChild(c1b1);


bnt_text=XmStringCreateLtoR("OPEN",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c1b2=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i);
  // create X component--------- mybutton    
XtAddCallback(c1b2,XmNactivateCallback,activateCB1,myform);  
XtManageChild(c1b2);



bnt_text=XmStringCreateLtoR("COPY",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c2b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel2,args,i);
  XtManageChild(c2b1);
  


//set the mymenu as MenuBar of the main window myform


 i=0;   
 XtSetArg(args[i],XmNrows,20);i++; 
 XtSetArg(args[i],XmNcolumns,40);i++; 
 XtSetArg(args[i],XmNeditable,False);i++;   
 XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++;  
 scrolled_text=XmCreateScrolledText(myform,"scolled_text",args,i);
 XtManageChild(scrolled_text);



 bnt_text=XmStringCreateLtoR("command",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNpromptString,bnt_text);i++; 
 command=XmCreateCommand(myform,"command",args,i);
 XtManageChild(command);
 XtAddCallback(command,XmNcommandEnteredCallback,exec_cmdCB,scrolled_text);

 //XmMainWindowSetAreas(myform,NULL,command,NULL,NULL,XtParent(scrolled_text));
 
 //XtVaSetValues(myform,XmNmenuBar,mymenu,NULL);

  XmMainWindowSep1(myform);

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  

}




