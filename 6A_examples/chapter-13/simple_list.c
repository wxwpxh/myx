/* simple_list.c -- introduce the List widget. Lists present
** a number of compound strings as choices. Therefore, strings
** must be converted before set in lists. Also, the number of
** visible items must be set or the List defaults to 1 item.
*/

#include <Xm/List.h>

char *months[] = {"January", "February", "March", "April", "May", "June", "July", 
		  "August", "September", "October", "November", "December"};

main (int argc, char *argv[])
{
	Widget        toplevel, list;
	XtAppContext  app;
	int           i, n = XtNumber (months);
	XmStringTable str_list;
	Arg           args[4];
	
	XtSetLanguageProc (NULL, NULL, NULL);
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					sessionShellWidgetClass, NULL);
					
	str_list = (XmStringTable) XtMalloc (n * sizeof (XmString));
	
	for (i = 0; i < n; i++)
		str_list[i] = XmStringCreateLocalized (months[i]);
		
	i = 0;
	XtSetArg (args[i], XmNvisibleItemCount, n); i++;
	XtSetArg (args[i], XmNitemCount, n); i++;
	XtSetArg (args[i], XmNitems, str_list); i++;
	list = XmCreateList (toplevel, "Hello", args, i);
	
	for (i = 0; i < n; i++)
		XmStringFree (str_list[i]);
	XtFree ((char *) str_list);
	
	XtManageChild (list);
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
