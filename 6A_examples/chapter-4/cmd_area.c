/* cmd_area.c -- use a ScrolledText object to view the 
** output of commands input by the user in a Command window. 
*/

#include <Xm/Text.h>
#include <Xm/MainW.h>
#include <Xm/Command.h>
#include <stdio.h>         /* For popen() */

/* main() -- initialize toolkit, create a main window, menubar,
** a Command Area and a ScrolledText to view the output of commands. 
*/

main (int argc, char *argv[])
{
	Widget       toplevel, main_w, menubar, menu, command_w, text_w;    
	XtAppContext app;    
	XmString     file, quit;    
	void         exec_cmd(Widget, XtPointer, XtPointer);
	void         exit(int);    
	Arg          args[5];   
	int          n = 0; 
	
	XtSetLanguageProc (NULL, NULL, NULL); 
	
	/* initialize toolkit and create toplevel shell */
	
	toplevel = XtVaOpenApplication (&app, "Demos", NULL, 0, &argc, argv, NULL, 
					 sessionShellWidgetClass, NULL); 
					 
	(void) close (0); /* don't let commands read from stdin */    
	
	/* MainWindow for the application -- contains menubar, ScrolledText     
	** and CommandArea (which prompts for filename).     
	*/ 
	
	n = 0;
	XtSetArg(args[n], XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE); n++;
	main_w = XmCreateMainWindow (toplevel, "main_w", args, n); 
	
	/* Create a simple MenuBar that contains one menu */    
	file = XmStringCreateLocalized ("File");    
	menubar = XmVaCreateSimpleMenuBar (main_w, "menubar", 
					   XmVaCASCADEBUTTON, file, 'F', NULL);    
	XmStringFree (file); 
	
	/* "File" menu has only one item (Quit), so make callback exit() */    
	quit = XmStringCreateLocalized ("Quit");    
	menu = XmVaCreateSimplePulldownMenu (menubar, "file_menu", 0, (void (*)()) exit, 
					     XmVaPUSHBUTTON, quit, 'Q', NULL, NULL, NULL);    
	XmStringFree (quit); 
	
	/* Menubar is done -- manage it */    
	
	XtManageChild (menubar);   
	
	/* Create ScrolledText -- this is work area for the MainWindow */ 
	n = 0;
	XtSetArg (args[n], XmNrows, 24); n++;    
	XtSetArg (args[n], XmNcolumns, 80); n++;    
	XtSetArg (args[n], XmNeditable, False); n++;   
	XtSetArg (args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;    
	text_w = XmCreateScrolledText (main_w, "text_w", args, n);    
	XtManageChild (text_w);
	
	/* store text_w as user data in "File" menu for file_cb() callback */    
	XtVaSetValues (menu, XmNuserData, text_w, NULL); 
	
	/* Create the command area -- this must be a Command class widget */    
	file = XmStringCreateLocalized ("Command:"); 
	n = 0;
	XtSetArg(args[n], XmNpromptString, file); n++;
	command_w = XmCreateCommand (main_w, "command_w", args, n);
	XmStringFree (file);    
	
	XtAddCallback (command_w, XmNcommandEnteredCallback, exec_cmd, text_w);
	XtVaSetValues (command_w, XmNmenuBar, menubar, 
				  XmNcommandWindow, command_w, 
				  XmNworkWindow, XtParent (text_w),
				  NULL);
				  
	XtManageChild (command_w);
	XtManageChild (main_w);
	XtRealizeWidget (toplevel);    
	XtAppMainLoop (app);
}

/* execute the command and redirect output to the ScrolledText window */
void exec_cmd (Widget    cmd_widget,  /* the command widget itself, not its Text widget */
               XtPointer client_data, /* passed the text_w as client_data */
	       XtPointer call_data)
{
	char          *cmd, buf[BUFSIZ];    
	XmTextPosition pos;    
	FILE          *pp, *popen();    
	Widget         text_w = (Widget) client_data;    
	XmCommandCallbackStruct *cbs = (XmCommandCallbackStruct *) call_data;
	
	cmd = (char *) XmStringUnparse (cbs->value, XmFONTLIST_DEFAULT_TAG, 
					XmCHARSET_TEXT, XmCHARSET_TEXT,
					(XmParseTable) 0, 0, XmOUTPUT_ALL); 
					
	if (!cmd || !*cmd) {
		/* nothing typed? */        
		
		if (cmd)            
			XtFree (cmd);        
			
		return;    
	}    
	
	/* make sure the file is a regular text file and open it */    
	if ((pp = popen (cmd, "r")) == (FILE *) 0)        
		perror (cmd);    

	XtFree (cmd);    
		
	if (pp == (FILE *) 0)        
		return;
			
	/* Clear output from any previous command */
		
	XmTextSetString (text_w, "");
	
	/* put the output of the command in the Text widget by reading     
	** until EOF (meaning that the command has terminated).     
	*/    
	
	for (pos = 0; fgets (buf, sizeof (buf), pp) != (char *) 0; pos += strlen (buf))
		XmTextReplace (text_w, pos, pos, buf);

	(void) pclose (pp);
}
