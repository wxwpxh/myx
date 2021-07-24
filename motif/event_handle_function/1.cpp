
#include  <stdio.h> 
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <Xm/XmAll.h>





void Quit(Widget w,XtPointer client_data,XtPointer call_data)
{ 
	exit(0);
} 

void set_display(Widget label_w,char * string)
{ 
	char str[50];

	XmString xmstr=XmStringCreateLtoR(string,XmSTRING_DEFAULT_CHARSET);
	XtVaSetValues(label_w,XmNlabelString,xmstr,NULL);
	XmStringFree(xmstr);  
}

void clear_display(Widget w,XtPointer client_data,XEvent * env ,Boolean *b)
{
	Widget tracker=(Widget)client_data;
	set_display(tracker,(char *)"leaving");
}






void trace_mouse_position(Widget w,XtPointer client_data,XEvent *env,Boolean *b) 
{
	char string[20];
	Widget tracker=(Widget)client_data;

	sprintf(string,"X:%04d,Y:%04d",env->xmotion.x,env->xmotion.y);
	set_display(tracker,string); 
} 


int main( int argc,char **argv)

{

	Widget toplevel,mybutton,command,mypanel,mylabel,target;
	XtAppContext  app_context; 
	Arg args[10];
	int i; 


	toplevel=XtAppInitialize(&app_context,"Xmdemos",NULL,0,&argc,argv,NULL,NULL,0);
	//X  initialize


	i=0;  
	mypanel=XmCreatePanedWindow(toplevel,(char *)"mypanel",args,i);
	XtManageChild(mypanel); 


	i=0; 
	XtSetArg(args[i],XmNpacking,XmPACK_COLUMN);i++;   
	XtSetArg(args[i],XmNnumColumns,2);i++;     
	command=XmCreateRowColumn( mypanel,(char *)"command" ,args,i);
	XtManageChild(command);




	XmString bnt_text=XmStringCreateLtoR((char *)"QUIT",XmSTRING_DEFAULT_CHARSET);
	i=0;
	XtSetArg(args[i],XmNlabelString,bnt_text);i++;  
	mybutton=XtCreateManagedWidget("button",xmPushButtonWidgetClass,command,args,i);
	XtAddCallback(mybutton,XmNactivateCallback,Quit,NULL);  
	XmStringFree(bnt_text);


	bnt_text=XmStringCreateLtoR((char *)"mousetracker",XmSTRING_DEFAULT_CHARSET);
	i=0;
	XtSetArg(args[i],XmNlabelString,bnt_text);i++;  
	mylabel=XtCreateManagedWidget("button",xmPushButtonWidgetClass,command,args,i);
	XmStringFree(bnt_text); 

	i=0;

	XtSetArg(args[i],XmNheight,100);i++;
	XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++; 
	XtSetArg(args[i],XmNtopWidget,mybutton);i++;  
	XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
	XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;
	target=XmCreateDrawingArea(mypanel,(char *)"target",args,i);
	XtManageChild(target);

	XtAddEventHandler(target,LeaveWindowMask,False,clear_display,mylabel);

	XtAddEventHandler(target,Button1MotionMask,False,trace_mouse_position,mylabel);

	XtRealizeWidget(toplevel);

	XtAppMainLoop(app_context);
	
	return 0;
}




