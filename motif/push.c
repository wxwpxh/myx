/*
 *  gcc -o push push.c -lXm -lXt -lX11
 *
 * */


#include <stdio.h>
#include <Xm/Xm.h> 
#include <Xm/PushB.h>

/* Prototype Callback function */

void pushed_fn(Widget , XtPointer , 
			XtPointer call_data);


int main(int argc, char **argv) 

{   Widget top_wid, button;
	XtAppContext  app;

	top_wid = XtVaAppInitialize(&app, "ush", NULL, 0,
				&argc, argv, NULL, NULL);

	button = XmCreatePushButton(top_wid, "ush_me", NULL, 0);
	/* tell Xt to manage button */
	XtManageChild(button);

	/* attach fn to widget */
	XtAddCallback(button, XmNactivateCallback, pushed_fn, NULL);

	XtRealizeWidget(top_wid); /* display widget hierarchy */
	XtAppMainLoop(app); /* enter processing loop */ 

	return 0;
}

void pushed_fn(Widget w, XtPointer client_data, 
			XtPointer call_data) 
{   
	printf("Don't Push Me!!\n");
}
