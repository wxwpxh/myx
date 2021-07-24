/* multi_font.c -- create three compound strings using 12, 14 and 18
** point fonts. The user can specify resources so that each of the strings
** use different fonts by setting resources similar to that shown
** by the fallback resources.
*/

#include <Xm/Label.h>

String fallbacks[] =
{
	"*.renderTable: TAG1, TAG2, TAG3",
	"*.fontType: FONT_IS_FONT",
	"*TAG1.fontName: -*-courier-*-r-*--*-120-*",
	"*TAG2.fontName: -*-courier-bold-o-*--*-140-*",
	"*TAG3.fontName: -*-courier-medium-r-*--*-180-*",
	NULL
};

main (int argc, char *argv[])
{
	Widget       toplevel, label;
	XtAppContext app;
	XmString     s1, s2, s3, text, tmp;
	Arg          args[2];
	String string1 = "This is a string ",
	       string2 = "that contains three ",
	       string3 = "separate fonts.";

	XtSetLanguageProc (NULL, NULL, NULL);
	toplevel = XtVaOpenApplication (&app, "Demo", NULL, 0, &argc, argv, fallbacks, 
					sessionShellWidgetClass, NULL);

	s1 = XmStringGenerate ((XtPointer) string1, NULL, XmCHARSET_TEXT, "TAG1");
	s2 = XmStringGenerate ((XtPointer) string2, NULL, XmCHARSET_TEXT, "TAG2");
	s3 = XmStringGenerate ((XtPointer) string3, NULL, XmCHARSET_TEXT, "TAG3");
	
	/* concatenate the 3 strings on top of each other, but we can only
	** do two at a time. So do s1 and s2 onto tmp and then do s3.
	*/
	tmp = XmStringConcatAndFree (s1, s2);
	text = XmStringConcatAndFree (tmp, s3);
	XtSetArg (args[0], XmNlabelString, text);
	label = XmCreateLabel (toplevel, "widget_name", args, 1);
	XtManageChild (label);

	XmStringFree (text);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
