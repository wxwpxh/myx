        
#include <stdio.h> 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
#include "editdialog.h"
#include "filetotext.h"
#include "scrolllist.h"

#include <iostream>
using namespace std;

Widget editdialog;
Widget filedialog;
Widget scrolled_list;
 


void activateCB(Widget w,XtPointer client_data,XtPointer call_data)
{
 printf("PushButton selected.\n");
 exit(0);
} 

void copy_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{  

 XtManageChild(editdialog);   
} 

void c2b2_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{  

 XtManageChild(scrolled_list);   
} 


//call back fuction for file dialog session be compeleted
void ok_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{
 char *filename;
 Widget text; 
 XmFileSelectionBoxCallbackStruct *cbs;
   
    
 cbs=(XmFileSelectionBoxCallbackStruct *)call_data;
 text=(Widget)client_data;
 if(!XmStringGetLtoR(cbs->value,XmSTRING_DEFAULT_CHARSET,&filename))
     return;
 if(!filename){
     puts("no file selected. ");
     XtFree(filename);
      
}
 XtUnmanageChild(w);
 ReadFiletoXmtext(filename,text);
 XtFree(filename);  

} 


void open_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{  
 XtManageChild(filedialog);
 
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
XmTextReplace(text,0,pos,(char *)"");


for(pos=0;fgets(buf,sizeof(buf),fp);pos+=strlen(buf))
XmTextReplace(text,pos,pos,buf);

pclose(fp);
}

void exec_cmdCB(Widget w,XtPointer client_data,XtPointer call_data)
{
 XmCommandCallbackStruct *cbs=(XmCommandCallbackStruct *)call_data;
 Widget scrolltext=(Widget)client_data;
exec_cmd(scrolltext,cbs->value);

} 






int main( int argc,char **argv)

 {
 
 Widget toplevel,scrolled_text,myform;
 Widget mymenu,mypanel,mypanel2,cascadeb1,c1b1,c1b2;   
 Widget command,cascadeb2,c2b1,c2b2;
 XtAppContext  app_context; 
 Arg args[10];  
 int i;    

 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
 
 

 i=0;   
 XtSetArg(args[i],XmNheight,300);i++;  
 myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i); 
 XtVaSetValues(myform,XmNwidth,650,XtVaTypedArg,
               XtNbackground,XtRString,"red",strlen("red")+1,NULL);

 i=0; 
 XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
 XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
 mymenu=XmCreateMenuBar(myform,(char *)"mymenu",args,i);
 XtManageChild(mymenu);


 i=0;    
 mypanel=XmCreatePulldownMenu(mymenu,(char *)"mypanel",args,i);
                                              
 i=0;  
 XtSetArg(args[i],XmNsubMenuId,mypanel);i++; 
 cascadeb1=XmCreateCascadeButton(mymenu,(char *)"FILE",args,i);
 XtManageChild(cascadeb1);

 i=0;    
 mypanel2=XmCreatePulldownMenu(mymenu,(char *)"mypanel2",args,i);
                                              
 i=0;  
 XtSetArg(args[i],XmNsubMenuId,mypanel2);i++; 
 cascadeb2=XmCreateCascadeButton(mymenu,(char *)"EDIT",args,i);
 XtManageChild(cascadeb2);
  
 XmString bnt_text=XmStringCreateLtoR((char *)"EXIT",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c1b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i); 
 XtAddCallback(c1b1,XmNactivateCallback,activateCB,NULL);  
 XtManageChild(c1b1);
 XmStringFree(bnt_text);


 bnt_text=XmStringCreateLtoR((char *)"COPY",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c2b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel2,args,i); 
 XtAddCallback(c2b1,XmNactivateCallback,copy_callbk,myform);   
 XmStringFree(bnt_text);
 editdialog=create_edit_dialog(c2b1);
 XtManageChild(c2b1);

 bnt_text=XmStringCreateLtoR((char *)"select",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c2b2=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel2,args,i); 
 XtAddCallback(c2b2,XmNactivateCallback,c2b2_callbk,myform);   
 XmStringFree(bnt_text);
 scrolled_list=create_scrolled_list(toplevel);
 XtManageChild(c2b2);

//create a scrolledtext for myform
 i=0;   
 XtSetArg(args[i],XmNrows,20);i++; 
 XtSetArg(args[i],XmNcolumns,40);i++; 
 XtSetArg(args[i],XmNeditable,False);i++;   
 XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++;  
 scrolled_text=XmCreateScrolledText(myform,(char *)"scrolled_text",args,i);
 XtManageChild(scrolled_text);
 

 bnt_text=XmStringCreateLtoR((char *)"OPEN",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c1b2=XtCreateManagedWidget("c1b2",xmPushButtonWidgetClass,mypanel,args,i);   
 XtAddCallback(c1b2,XmNactivateCallback,open_callbk,NULL);  
 XtManageChild(c1b2);
 XmStringFree(bnt_text);

 
 Widget dia_shell=XmCreateDialogShell(c1b2,(char *)"OPEN FILE",args,i); 
 XmString file_title=XmStringCreateLtoR((char *)"open file to view",XmSTRING_DEFAULT_CHARSET);
 i=0; 
 XtSetArg(args[i],XmNdialogTitle,file_title);i++; 
 filedialog=XmCreateFileSelectionBox(dia_shell,(char *)"filedialog",args,i);
 XtAddCallback(filedialog,XmNokCallback,ok_callbk,scrolled_text); 
 XmStringFree(file_title);





 bnt_text=XmStringCreateLtoR((char *)"command",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNpromptString,bnt_text);i++; 
 command=XmCreateCommand(myform,(char *)"command",args,i);
 XtManageChild(command);
 XtAddCallback(command,XmNcommandEnteredCallback,exec_cmdCB,scrolled_text);
 XmStringFree(bnt_text);

 XtRealizeWidget(toplevel);
 XtAppMainLoop(app_context);
  
 return 0;
}




