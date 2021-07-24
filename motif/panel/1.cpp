#include<stdio.h>
#include<stdlib.h>
#include<X11/Intrinsic.h>
#include <Xm/XmAll.h>

#include<iostream>
using namespace std;
 
int main( int argc,char **argv)

 {
 
  Widget toplevel,pane,label,text;  
  XtAppContext  app_context;
 
   Arg args[10]; 
   int i,n,num;  
   XmStringTable s_table;

  toplevel=XtVaAppInitialize(&app_context,"Demos",NULL,0,&argc,argv,NULL,NULL);
   //X  initialize    


 i=0;  
 pane=XmCreatePanedWindow(toplevel,(char *)"pane",args,i);   
 
 i=0; 
 XtSetArg(args[i],XmNpaneMinimum,25);i++;   
 XtSetArg(args[i],XmNpaneMaximum,45);i++;     
 label=XmCreateLabel( pane,(char *)"label" ,args,i);
 XtManageChild(label);

 i=0;   
 XtSetArg(args[i],XmNrows,5);i++; 
 XtSetArg(args[i],XmNcolumns,80);i++; 
 XtSetArg(args[i],XmNpaneMinimum,35);i++;   
 XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++; 
 XtSetArg(args[i],XmNvalue,"this is a test for panedwindow");i++; 
 text=XmCreateText( pane,(char *)"text" ,args,i);
 XtManageChild(text);
 


 
  XtManageChild(pane);
  XtRealizeWidget(toplevel); 
  XtAppMainLoop(app_context);


  return 0;
  

}




