/* frame.c --
 mount pushbutton of push.c in a frame widget
 
gcc -o frame frame.c -lXm -lXt -lX11

 */

#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>  /* header file for frame stuff */

#include <stdio.h>

/* Prototype callback */

void pushed_fn(Widget , XtPointer , 
               XmPushButtonCallbackStruct *);

int main(int argc, char **argv)
{
    Widget        top_wid, button, frame;
    XtAppContext app;
    

    top_wid = XtVaAppInitialize(&app, "Push", NULL, 0,
        &argc, argv, NULL, NULL);


    frame = XtVaCreateManagedWidget("frame",
         xmFrameWidgetClass, top_wid,
         XmNshadowType, XmSHADOW_IN,
         NULL);


    button = XmCreatePushButton(frame, "Push_me",
        NULL, 0);

    XtManageChild(button);

    XtAddCallback(button, XmNactivateCallback, pushed_fn, NULL);

    XtRealizeWidget(top_wid);
    XtAppMainLoop(app);
	
	return 0;
}

void
pushed_fn(Widget w, XtPointer client_data, 
          XmPushButtonCallbackStruct *cbs)

{
    printf("Don't Push Me!!\n");
}
