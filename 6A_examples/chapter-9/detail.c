/* detail.c -- demonstrate the container and icon gadget
** in Detail layout, with a selection callback printing
** out the selected IconGadget data
*/

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Container.h>
#include <Xm/IconG.h>
#include <Xm/ScrolledW.h>
#include <Xm/XmosP.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

static char *column_headings[] = { "Name", "Size", "Modified" } ;

static Pixmap large_folder_icon = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_folder_icon = XmUNSPECIFIED_PIXMAP ;
static Pixmap large_folder_mask = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_folder_mask = XmUNSPECIFIED_PIXMAP ;
static Pixmap large_file_icon   = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_file_icon   = XmUNSPECIFIED_PIXMAP ;
static Pixmap large_file_mask   = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_file_mask   = XmUNSPECIFIED_PIXMAP ;

/* Create the Icongadget children at a given level in the
** file system. This time, Detail is added about the files.
*/
static void add_directory(char *path, Widget parent_entry, Widget container)
{
	DIR           *dir ;
	struct dirent *entry ;
	Arg            args[12] ;
	int            n ;

	/* Loop through the directory entries */
	dir = opendir(path) ;

	while ((entry = readdir(dir)) != NULL) {
		char name[256];
		struct stat sb;

		/* Get the file details. Note: no real error checking */
		(void) sprintf(name, "%s/%s", path, entry->d_name);

		if (stat(name, &sb) == 0) {
			XmString      s ;
			XmStringTable details ;
			char          buf[20] ;
			Widget        item ;
			int           isDirectory ;

			if ((strcmp (entry->d_name, ".") == 0) || (strcmp (entry->d_name, "..") == 0)) {
				continue ;
			}
			
			s = XmStringCreateLocalized (entry->d_name) ;

			isDirectory = ((sb.st_mode & S_IFDIR) != 0) ;
			
			/* Create the details array */
			details = (XmStringTable) XtMalloc(2 * sizeof (XmString *)) ;

			(void) sprintf(buf, "%d", sb.st_size) ;
			details[0] = XmStringCreateLocalized (buf) ;
			details[1] = XmStringCreateLocalized ((char *) ctime(&sb.st_mtim.tv_sec)) ;

			/* Create the IconGadget */
			n = 0 ;
			XtSetArg (args[n], XmNlabelString, s) ; n++;

			if (isDirectory) {
				XtSetArg (args[n], XmNlargeIconPixmap, large_folder_icon) ; n++ ;
			}
			else {
				XtSetArg (args[n], XmNlargeIconPixmap, large_file_icon) ; n++ ;
			}
			
			if (isDirectory) {
				XtSetArg (args[n], XmNlargeIconMask, large_folder_mask) ; n++ ;
			}
			else {
				XtSetArg (args[n], XmNlargeIconMask, large_file_mask) ; n++ ;
			}
			
			if (isDirectory) {
				XtSetArg (args[n], XmNsmallIconPixmap, small_folder_icon) ; n++ ;
			}
			else {
				XtSetArg (args[n], XmNsmallIconPixmap, small_file_icon) ; n++ ;
			}
			
			if (isDirectory) {
				XtSetArg (args[n], XmNsmallIconMask, small_folder_mask) ; n++ ;
			}
			else {
				XtSetArg (args[n], XmNsmallIconMask, small_file_mask) ; n++ ;
			}

			XtSetArg (args[n], XmNentryParent, parent_entry) ; n++ ;
			XtSetArg (args[n], XmNdetail, details) ; n++ ;
			XtSetArg (args[n], XmNdetailCount, 2) ; n++ ;

			item = XmCreateIconGadget (container, "icon", args, n) ;
			XtManageChild (item) ;

			XmStringFree(details[0]) ;
			XmStringFree(details[1]) ;
			XtFree((XtPointer)details) ;

			/* Recurse to subdirectories */
			if (entry->d_name[0] != '.') {
				if (isDirectory) {
					add_directory(name, item, container) ;
				}
			}
		}
	}

	(void) closedir(dir) ;
}

static char *GetBitmapPath (Display *display, char *file)
{
	char           *bmPath    = (char *) 0 ;
	char           *name      = (char *) 0 ;
	Boolean         user_path = False ;
	SubstitutionRec subs ;

	subs.substitution = file ;

	bmPath = (char *) _XmOSInitPath(file, "XBMLANGPATH", &user_path) ;
	
	if (user_path) subs.match = 'B';
	else           subs.match = 'P' ;

	name = XtResolvePathname(display, "bitmaps", file, NULL, bmPath, &subs, 1, NULL) ;

	if (bmPath) {
		/* Some XtResolvePathname() can return non-heap copy of parameter  */
		/* This can cause serious memory corruption if freed inadvertently */
		
		if (name != bmPath) {
			XtFree (bmPath) ;
		}
	}

	return name ;
}
	
static Pixmap GetPixmap (Widget widget, char *file)
{
	Screen *screen = XtScreen(widget) ;
	char   *path   = GetBitmapPath (XtDisplay(widget), file) ;

	return XmGetPixmap (screen, path, BlackPixelOfScreen(screen), WhitePixelOfScreen(screen)) ;
}

static Pixmap GetBitmap (Widget widget, char *file)
{
	Pixmap       pixmap = XmUNSPECIFIED_PIXMAP ;
	unsigned int width ;
	unsigned int height ;
	         int hotx ;
	         int hoty ;
	char    *path = GetBitmapPath (XtDisplay(widget), file) ;
	
	XReadBitmapFile (XtDisplay(widget), XtWindow(widget), path, &width, &height, &pixmap, &hotx, &hoty) ;

	return pixmap ;
}


/* Construct a file system path from the selected IconGadget */
/* This is crude, but it works. Optimisation is a reader exercise */
static char *GetPath (Widget w, char *sofar)
{
	static char buffer[512];
	char temp[512], *leaf ;
	Widget parent = NULL ;
	XmString xms ;

	if (sofar == NULL) {
		(void) strcpy (buffer, "") ;
	}

	if (w == NULL) {
		return buffer ;
	}

	(void) strcpy (temp, buffer) ;

	XtVaGetValues (w, XmNentryParent, &parent, XmNlabelString, &xms, NULL) ;

	leaf = (char *) XmStringUnparse (xms, NULL, 
					 XmCHARSET_TEXT, XmCHARSET_TEXT, 
					 NULL, 0, XmOUTPUT_ALL) ;
	
	(void) sprintf (buffer, "%s%s%s", leaf, (sofar ? "/" : ""), temp) ;

	XtFree (leaf) ;

	return GetPath (parent, buffer) ;
}

/* The selection callback: simply prints out 
** the selected IconGadget file system path
*/
static void select_callback (Widget w, XtPointer client_data, XtPointer call_data)
{
	XmContainerSelectCallbackStruct *cptr = (XmContainerSelectCallbackStruct *) call_data ;
	int i ;
	char *path ;

	for (i = 0 ; i < cptr->selected_item_count ; i++) {
		printf ("Selected: %s\n", GetPath (cptr->selected_items[i], NULL)) ;
	}
}

main (int argc, char *argv[])
{
	Widget        toplevel, form, scrolled_win, container;
	XtAppContext  app;
	int           i, n, count;
	Arg           args[12];
	XmStringTable column_headings_table;

	XtSetLanguageProc (NULL, NULL, NULL) ;

	/* Create the top-level shell and two RowColumns */
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL,
					sessionShellWidgetClass, NULL);

	XtVaSetValues (toplevel, XmNtitle, "XmContainer - Outline Layout", NULL);

	form = XmCreateForm (toplevel, "form", NULL, 0) ;

	n = 0;
	XtSetArg (args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;
	XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
	XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
	scrolled_win = XmCreateScrolledWindow (form, "scrolled_win", args, n) ;
	
	/* Create the Container */
	count = XtNumber(column_headings);
	column_headings_table = (XmStringTable) XtMalloc(count * sizeof (XmString *));
	for (i = 0; i < count; i++)
		column_headings_table[i] = XmStringCreateLocalized (column_headings[i]);

	n = 0;
	XtSetArg (args[n], XmNwidth, 600); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	XtSetArg (args[n], XmNdetailColumnHeading, column_headings_table); n++;
	XtSetArg (args[n], XmNdetailColumnHeadingCount, count); n++;
	XtSetArg (args[n], XmNlayoutType, XmDETAIL); n++;
	XtSetArg (args[n], XmNentryViewType, XmSMALL_ICON); n++;
	XtSetArg (args[n], XmNselectionPolicy, XmSINGLE_SELECT); n++;
	container = XmCreateContainer (scrolled_win, "container", args, n);

	XtAddCallback (container, XmNselectionCallback, select_callback, NULL);
	
	for (i = 0; i < count; i++)
		XmStringFree(column_headings_table[i]);
	XtFree((XtPointer)column_headings_table);
	
	XtManageChild (container);
	XtManageChild (scrolled_win);
	XtManageChild (form);
	XtRealizeWidget (toplevel);

	/* Read in the icons and their masks */
	/* The bitmaps require a window, therefore must be done post-realize */
	large_folder_icon = GetPixmap(container, "folder_large.xbm") ;
	small_folder_icon = GetPixmap(container, "folder_small.xbm") ;
	large_file_icon   = GetPixmap(container, "file_large.xbm") ;
	small_file_icon   = GetPixmap(container, "file_small.xbm") ;

	large_folder_mask = GetBitmap(container, "folder_large_mask.xbm");
	small_folder_mask = GetBitmap(container, "folder_small_mask.xbm");
	large_file_mask   = GetBitmap(container, "file_large_mask.xbm");
	small_file_mask   = GetBitmap(container, "file_small_mask.xbm");

	add_directory(".", NULL, container);

	XtAppMainLoop (app);
}
