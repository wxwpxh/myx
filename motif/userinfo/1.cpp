#include <stdio.h> 
#include <stdlib.h>
#include <X11/Intrinsic.h>     
#include <Xm/XmAll.h>
#include <pwd.h>



typedef struct{
	String login;
	int    uid;
	String name;
	String homedir;
}UserInfo;

UserInfo * getusers(){
	//extern   char* strcpy();
	struct   passwd  *pw;
	UserInfo         *users=NULL;
	int              n;


	setpwent();
	for(n=0;pw=getpwent();n++){
		users=(UserInfo*)XtRealloc((char*)users,(n+1)*sizeof(UserInfo));
		users[n].login=strcpy(XtMalloc(strlen(pw->pw_name)+1),pw->pw_name);
		users[n].name=strcpy(XtMalloc(strlen(pw->pw_gecos)+1),pw->pw_gecos);
		users[n].homedir=strcpy(XtMalloc(strlen(pw->pw_dir)+1),pw->pw_dir);
		users[n].uid=pw->pw_uid; 
	}
	users=(UserInfo*)XtRealloc((char*)users,(n+1)*sizeof(UserInfo));
	users[n].login=NULL;
	endpwent();
	return users;
} 



int main( int argc,char **argv)

{

	Widget toplevel,sw,main_rc,form,toggle;  
	XtAppContext  app_context; 
	UserInfo *users;
	toplevel=XtVaAppInitialize(&app_context,"Demos",NULL,0,&argc,argv,NULL,NULL);    //X  initialize  


	sw=XtVaCreateManagedWidget("scrolled_w",xmScrolledWindowWidgetClass,toplevel,
				XmNwidth,500,
				XmNheight,300,
				XmNscrollingPolicy,XmAPPLICATION_DEFINED,//XmAUTOMATIC,
				XmNvisualPolicy,XmVARIABLE,
				NULL
				);  // create X component--------- myform

	main_rc=XtVaCreateWidget("main_rc",xmRowColumnWidgetClass,sw,NULL);
	if(!(users=getusers())){
		perror("Can't read user data info.");
		exit(1);
	}

	while(users->login){
		char uid[8];
		form=XtVaCreateWidget(NULL,xmFormWidgetClass,main_rc,NULL);
		XtVaCreateManagedWidget(users->login,
					xmToggleButtonWidgetClass,form,      
					XmNalignment              ,XmALIGNMENT_BEGINNING,
					XmNtopAttachment          ,XmATTACH_FORM,
					XmNbottomAttachment       ,XmATTACH_FORM,
					XmNleftAttachment         ,XmATTACH_FORM,
					XmNrightAttachment        ,XmATTACH_POSITION,
					XmNrightPosition          ,15,
					NULL
					);
		sprintf(uid,"%d",users->uid);

		XtVaCreateManagedWidget(uid,
					xmLabelGadgetClass,
					//xmToggleButtonWidgetClass
					form,      
					XmNalignment              ,XmALIGNMENT_END,
					XmNtopAttachment          ,XmATTACH_FORM,
					XmNbottomAttachment       ,XmATTACH_FORM,
					XmNleftAttachment         ,XmATTACH_POSITION,
					XmNleftPosition          ,15,
					XmNrightAttachment        ,XmATTACH_POSITION,
					XmNrightPosition          ,20,
					NULL
					);

		XtVaCreateManagedWidget(users->name,
					xmLabelGadgetClass,
					//xmToggleButtonWidgetClass
					form,      
					XmNalignment              ,XmALIGNMENT_BEGINNING,
					XmNtopAttachment          ,XmATTACH_FORM,
					XmNbottomAttachment       ,XmATTACH_FORM,
					XmNleftAttachment         ,XmATTACH_POSITION,
					XmNleftPosition           ,22,
					XmNrightAttachment        ,XmATTACH_POSITION,
					XmNrightPosition          ,40,
					NULL
					);




		XtVaCreateManagedWidget(users->homedir,
					xmTextWidgetClass,form,XmNeditable,False,XmNcursorPositionVisible,False, 
					XmNalignment              ,XmALIGNMENT_BEGINNING,
					XmNtopAttachment          ,XmATTACH_FORM,
					XmNbottomAttachment       ,XmATTACH_FORM,
					XmNleftAttachment         ,XmATTACH_POSITION,
					XmNleftPosition           ,45,
					XmNrightAttachment        ,XmATTACH_FORM,
					XmNvalue                  ,users->homedir,
					NULL
					);

		XtManageChild(form);
		users++;
	};
	XtManageChild(main_rc);
	XtRealizeWidget(toplevel); 
	XtAppMainLoop(app_context);

	return 0;
}




