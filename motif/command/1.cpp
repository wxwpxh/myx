        
#include <stdio.h> 
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
#include <iostream>
using namespace std; 
// Widget filedialog;
 
 
void ReadFiletoXmtext(char *filename,Widget text){
 XmTextPosition pos; 
 char value[81];
 FILE *fp;

 value[80]=0; 
XmTextSetString(text,"");
if((fp=fopen(filename,"r"))==NULL){
 cerr<<"can not open the file:"<<filename<<endl;
 exit(0); 
 };

while(!feof(fp)){
fread(value,sizeof(char),80,fp);
pos=XmTextGetLastPosition(text);
XmTextInsert(text,pos,value);
}

fclose(fp);

}



void activateCB(Widget w,XtPointer client_data,XtPointer call_data)
{
 printf("PushButton selected.\n");
 exit(0);
} 

void copy_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{ 
 Widget mydraw,parent_win;
 int i;
 Arg args[10];  
 parent_win=(Widget)client_data; 
  
 i=0;
 mydraw=XmCreateDrawingArea(parent_win,"mydraw",args,i);
 XtManageChild(mydraw);  
 printf("PushButton copy selected.\n");

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
     return; 
}
 XtUnmanageChild(w);
 ReadFiletoXmtext(filename,text);
 XtFree(filename);  
   
 
} 


void open_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{ 
 Widget win,dia_shell;Widget filedialog;
 int i;
 Arg args[10];  
 win=(Widget)client_data;
  
 i=0;
 dia_shell=XmCreateDialogShell(win,"OPEN FILE",args,i);
 
 i=0; 
 filedialog=XmCreateFileSelectionBox(dia_shell,"filedialog",args,i);
 XtAddCallback(filedialog,XmNokCallback,ok_callbk,win);
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
XmTextReplace(text,0,pos,"");


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
 
Widget toplevel,scrolled_text,myform,mymenu,mypanel,mypanel2,cascadeb1,c1b1,c1b2;   
Widget command,cascadeb2,c2b1;  
XtAppContext  app_context; 
Arg args[10];  
int i;    //define variant  


 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 i=0;   
 //XtSetArg(args[i],XmNwidth,800);i++; 
 XtSetArg(args[i],XmNheight,300);i++; 
 
 myform=XtCreateManagedWidget("Form",xmMainWindowWidgetClass,toplevel,args,i);
 // create X component--------- myform

XtVaSetValues(myform,XmNwidth,650,XtVaTypedArg,XtNbackground,XtRString,"red",strlen("red")+1,NULL);

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





bnt_text=XmStringCreateLtoR("COPY",XmSTRING_DEFAULT_CHARSET);
i=0;
XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
c2b1=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel2,args,i);

XtAddCallback(c2b1,XmNactivateCallback,copy_callbk,myform);   
XtManageChild(c2b1);



//create a scrolledtext for myform
 i=0;   
 XtSetArg(args[i],XmNrows,20);i++; 
 XtSetArg(args[i],XmNcolumns,40);i++; 
 XtSetArg(args[i],XmNeditable,False);i++;   
 XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++;  
 scrolled_text=XmCreateScrolledText(myform,"scrolled_text",args,i);
 XtManageChild(scrolled_text);
 
 bnt_text=XmStringCreateLtoR("OPEN",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
 c1b2=XtCreateManagedWidget("button",xmPushButtonWidgetClass,mypanel,args,i);
  // create X component--------- mybutton    
 XtAddCallback(c1b2,XmNactivateCallback,open_callbk,scrolled_text);  
 XtManageChild(c1b2);






 bnt_text=XmStringCreateLtoR("command",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNpromptString,bnt_text);i++; 
 command=XmCreateCommand(myform,"command",args,i);
 XtManageChild(command);
 XtAddCallback(command,XmNcommandEnteredCallback,exec_cmdCB,scrolled_text);

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  
  return 0;
}




