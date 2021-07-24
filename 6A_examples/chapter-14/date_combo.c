/* datw_combo.c -- demonstrate the combo box widget */
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/ComboBox.h>

char *months[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char *days[] = {
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
	"11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
	"21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
	"31",
};

static Widget create_combo(Widget parent, char *name, int count, char **values, int columns)
{
	Widget        combo;
	int           i, n = XtNumber (months);
	XmStringTable str_list;

	str_list = (XmStringTable) XtMalloc(count * sizeof (XmString *));

	for (i = 0; i < count; i++)
		str_list[i] = XmStringCreateLocalized (values[i]);

	XtVaCreateManagedWidget (name,
		xmComboBoxWidgetClass, parent,
		XmNcomboBoxType,       XmDROP_DOWN_LIST,
		XmNitemCount,	       count,
		XmNitems,	       str_list,
		XmNcolumns,	       columns,
		NULL);
	
	for (i = 0; i < count; i++)
		XmStringFree(str_list[i]);
	XtFree((XtPointer)str_list);

	return combo;
}

main(int argc, char **argv)
{
	Widget       toplevel, rowcol, year_combo;
	XtAppContext app;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
		&argc, argv, NULL, NULL);

	rowcol = XtVaCreateWidget ("rowcol",
		xmRowColumnWidgetClass, toplevel,
		XmNorientation, XmHORIZONTAL,
		NULL);
	
	create_combo (rowcol, "months", XtNumber(months), months, 3);
	create_combo (rowcol, "days", XtNumber(days), days, 2);
	
	XtManageChild (rowcol);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
