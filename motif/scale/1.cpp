      
#include  <stdio.h> 
#include <stdlib.h>
 
#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>
 Widget colorwin;
 XColor color;
 



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
  
color.flags=DoRed|DoGreen|DoBlue;

 
 toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
    //X  initialize
 

 rowcol=XtVaCreateManagedWidget("rowcol",xmRowColumnWidgetClass,toplevel,
                              XmNheight,400 ,NULL);

 
 colorwin=XtVaCreateManagedWidget("colorwin",xmRowColumnWidgetClass,rowcol,
              XmNheight,200 
              ,XmNbackground,color.pixel
              ,NULL);
 

 rowcol=XtVaCreateWidget("rowcol",xmRowColumnWidgetClass,rowcol,
                XmNheight,400 ,XmNorientation,XmHORIZONTAL,NULL);


// create X component--------- scale  
 XmString bnt_text=XmStringCreateLtoR("Red",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNheight,100);i++; 
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,255);i++;
 XtSetArg(args[i],XmNscaleMultiple,5);i++; 
 XtSetArg(args[i],XmNforeground,bnt_text);i++; ; 
 scale=XtCreateManagedWidget("Red",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 
 
 bnt_text=XmStringCreateLtoR("Green",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNheight,100);i++; 
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,255);i++;
 XtSetArg(args[i],XmNscaleMultiple,5);i++; 
 XtSetArg(args[i],XmNforeground,bnt_text);i++; ; 
 scale=XtCreateManagedWidget("Green",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 
bnt_text=XmStringCreateLtoR("Blue",XmSTRING_DEFAULT_CHARSET);
 i=0;
 XtSetArg(args[i],XmNheight,100);i++; 
 XtSetArg(args[i],XmNtitleString,bnt_text);i++; 
 XtSetArg(args[i],XmNmaximum,255);i++;
 XtSetArg(args[i],XmNscaleMultiple,5);i++; 
 XtSetArg(args[i],XmNforeground,bnt_text);i++; ; 
 scale=XtCreateManagedWidget("Blue",xmScaleWidgetClass,rowcol,args,i);  
 XtAddCallback(scale,XmNvalueChangedCallback,newvalue_callbk,NULL); 
 
 



  XtManageChild(rowcol);
  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_context);
  

}




