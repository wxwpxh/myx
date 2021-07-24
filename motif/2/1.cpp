/* header file for frame stuff */
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <stdio.h>

/* Prototype callback */
void pushed_fn(Widget , XtPointer , XtPointer);


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


    button = XmCreatePushButton(frame, (char *)"ush_me",
        NULL, 0);

    XtManageChild(button);

    XtAddCallback(button, XmNactivateCallback, pushed_fn, NULL);

    XtRealizeWidget(top_wid);
    XtAppMainLoop(app);

    return 0;		
}

void
pushed_fn(Widget w, XtPointer client_data,XtPointer call_data)

{
    printf("Don't Push Me!!\n");
}
