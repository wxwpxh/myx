/* outline.c -- demonstrate the container and icon gadget in an outline layout */

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Container.h>
#include <Xm/IconG.h>
#include <Xm/ScrolledW.h>
#include <Xm/XmosP.h>
#include <sys/stat.h>
#include <dirent.h>

static Pixmap small_folder_icon = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_folder_mask = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_file_icon   = XmUNSPECIFIED_PIXMAP ;
static Pixmap small_file_mask   = XmUNSPECIFIED_PIXMAP ;

/*
** Adds one level of directory entries to the Container, using
** parent_entry as the XmNentryParent value
*/
static void add_directory(char *path, Widget parent_entry, Widget container)
{
	DIR           *dir ;
	struct dirent *entry ;
	Arg            args[8] ;
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
			Widget        item ;
			int           isDirectory ;

			if ((strcmp (entry->d_name, ".") == 0) || (strcmp (entry->d_name, "..") == 0)) {
				continue ;
			}
			
			s = XmStringCreateLocalized (entry->d_name) ;

			isDirectory = ((sb.st_mode & S_IFDIR) != 0) ;
			
			/* Create the IconGadget */
			n = 0 ;
			XtSetArg (args[n], XmNlabelString, s) ; n++;

			if (isDirectory) {
				XtSetArg (args[n], XmNsmallIconPixmap, small_folder_icon) ; n++ ;
				XtSetArg (args[n], XmNsmallIconMask, small_folder_mask) ; n++ ;
			}
			else {
				XtSetArg (args[n], XmNsmallIconPixmap, small_file_icon) ; n++ ;
				XtSetArg (args[n], XmNsmallIconMask, small_file_mask) ; n++ ;
			}
			
			/* This gives the Tree its structure */
			XtSetArg (args[n], XmNentryParent, parent_entry) ; n++ ;
			
			item = XmCreateIconGadget (container, "icon", args, n) ;
			XtManageChild (item) ;

			/* Recurse to subdirectories */
			/* This item becomes the new XmNentryParent */
			if (entry->d_name[0] != '.') {
				if (isDirectory) {
					add_directory(name, item, container) ;
				}
			}
		}
	}

	(void) closedir(dir) ;
}

/* Utility: set up a path to find bitmaps and pixmaps */
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
	
/* Utility: load a Pixmap file for the IconGadget image */
static Pixmap GetPixmap (Widget widget, char *file)
{
	Screen *screen = XtScreen(widget) ;
	char   *path   = GetBitmapPath (XtDisplay(widget), file) ;

	return XmGetPixmap (screen, path, BlackPixelOfScreen(screen), WhitePixelOfScreen(screen)) ;
}

/* Utility: load a Bitmap file for the IconGadget image mask */
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

main (int argc, char *argv[])
{
	Widget        toplevel, form, scrolled_win, container;
	XtAppContext  app;
	int           n;
	Arg           args[8];

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

	n = 0;
	XtSetArg (args[n], XmNwidth, 600); n++;
	XtSetArg (args[n], XmNheight, 500); n++;
	XtSetArg (args[n], XmNlayoutType, XmOUTLINE); n++;
	XtSetArg (args[n], XmNentryViewType, XmSMALL_ICON); n++;
	container = XmCreateContainer (scrolled_win, "container", args, n);
	
	XtManageChild (container);
	XtManageChild (scrolled_win);
	XtManageChild (form);
	XtRealizeWidget (toplevel);

	/* Read in the icons and their masks */
	/* The bitmaps require a window, therefore must be done post-realize */
	small_folder_icon = GetPixmap(container, "folder_small.xbm") ;
	small_file_icon   = GetPixmap(container, "file_small.xbm") ;

	small_folder_mask = GetBitmap(container, "folder_small_mask.xbm");
	small_file_mask   = GetBitmap(container, "file_small_mask.xbm");

	/* Load the Icon Gadgets */
	add_directory(".", NULL, container);

	XtAppMainLoop (app);
}
