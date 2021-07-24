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

int days_per_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 31, 31
};

XmStringTable day_list = (XmStringTable) 0 ;

static void sel_callback(Widget combo, XtPointer client_data, XtPointer call_data)
{
	XmComboBoxCallbackStruct *cbs = (XmComboBoxCallbackStruct *)call_data;

	if (cbs->reason == XmCR_SELECT) {
		Widget day_combo = (Widget)client_data;

		XtVaSetValues(day_combo,
			XmNitems,     day_list,
			XmNitemCount, days_per_month[cbs->item_position],
			NULL);
	}
}

main(int argc, char **argv)
{
	Widget        toplevel, rowcol, month_combo, day_combo;
	XtAppContext  app;
	int           i;
	XmStringTable month_list;

	XtSetLanguageProc (NULL, NULL, NULL);

	toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
		&argc, argv, NULL, NULL);

	month_list = (XmStringTable) XtMalloc(12 * sizeof (XmString *));

	for (i = 0; i < 12; i++)
		month_list[i] = XmStringCreateLocalized (months[i]);

	day_list = (XmStringTable) XtMalloc(31 * sizeof (XmString *));

	for (i = 0; i < 31; i++)
		day_list[i] = XmStringCreateLocalized (days[i]);

	rowcol = XtVaCreateWidget ("rowcol",
		xmRowColumnWidgetClass, toplevel,
		XmNorientation, XmHORIZONTAL,
		NULL);
	
	month_combo = XtVaCreateManagedWidget ("months",
		xmComboBoxWidgetClass, rowcol,
		XmNcomboBoxType,	XmDROP_DOWN_COMBO_BOX,
		XmNitemCount,	12,
		XmNitems,		month_list,
		XmNcolumns,		3,
		NULL);
	
	day_combo = XtVaCreateManagedWidget ("days",
		xmComboBoxWidgetClass, rowcol,
		XmNcomboBoxType,	XmDROP_DOWN_COMBO_BOX,
		XmNitemCount,	31,
		XmNitems,		day_list,
		XmNcolumns,		2,
		NULL);
	
	for (i = 0; i < 12; i++)
		XmStringFree(month_list[i]);
	XtFree((XtPointer)month_list);

	XtAddCallback (month_combo, XmNselectionCallback, sel_callback, (XtPointer)day_combo);

	XtManageChild (rowcol);
	
	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}
