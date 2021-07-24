      
#include  <stdio.h> 
#include <stdlib.h>
 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
 
 



void newvalue_callbk(Widget w,caddr_t client_data,caddr_t call_data)
{
 XmScrollBarCallbackStruct *cbs;
 
 cbs=(XmScrollBarCallbackStruct *)call_data;
 printf("%s:%d\n",XtName(w),cbs->value);
 
} 

void main( int argc,char **argv)

 {
 
  Widget toplevel,rowcol,scale;
  XtAppContext  app_context; 
  Arg args[10];
  int i;
   //define variant
 


 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 rowcol=XtVaCreateWidget("rowcol",xmRowColumnWidgetClass,toplevel,
               XmNorientation,XmHORIZONTAL,NULL);





// create X component--------- scale  
 XmString bnt_text=XmStringCreateLtoR("Days",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,7);i++;
 XtSetArg(args[i],XmNminimum,1);i++; 
 XtSetArg(args[i],XmNvalue,1);i++; 
 XtSetArg(args[i],XmNshowValue,True);i++; 
 scale=XtCreateManagedWidget("Days",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 
 bnt_text=XmStringCreateLtoR("Weeks",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,52);i++;
 XtSetArg(args[i],XmNminimum,1);i++; 
 XtSetArg(args[i],XmNvalue,1);i++; 
 XtSetArg(args[i],XmNshowValue,True);i++; 
 scale=XtCreateManagedWidget("Weeks",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 
 bnt_text=XmStringCreateLtoR("Months",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,12);i++;
 XtSetArg(args[i],XmNminimum,1);i++; 
 XtSetArg(args[i],XmNvalue,1);i++; 
 XtSetArg(args[i],XmNshowValue,True);i++; 
 scale=XtCreateManagedWidget("Months",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 

 bnt_text=XmStringCreateLtoR("Years",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,100);i++;
 XtSetArg(args[i],XmNminimum,1);i++; 
 XtSetArg(args[i],XmNvalue,1);i++; 
 XtSetArg(args[i],XmNshowValue,True);i++; 
 scale=XtCreateManagedWidget("Years",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 




  XtManageChild(rowcol);
  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  

}




