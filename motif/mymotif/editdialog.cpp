#include "editdialog.h"


void edit_ok_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{
 Widget father;
 father=XtParent(w);
 XtUnmanageChild(father);   
} 

void set_color_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{
 Widget father;
 char*  color;
 if(XmToggleButtonGetState(w)){
         color=(char*)client_data;
         father=XtParent(w); 
         father=XtParent(father);  
         XtVaSetValues(father,XmNwidth,650,XtVaTypedArg,
                        XtNbackground,XtRString,color,strlen("red")+1,NULL); 
         XtManageChild(father);  
         }  
} 



Widget create_edit_dialog(Widget father)
{ 
 Widget dia_shell,form,name_label,name_text,addr_label,addr_text; //,age_label
 Widget birth_label,birth_text,toggle_box,tog1,tog2,tog3,sep;
 Widget mark_label,scrolled_text,ok_but,apply_but,cancel_but,help_but;  
 XmString bnt_text;  
 Arg args[10];    
 int i; 


 i=0;
 dia_shell=XmCreateDialogShell(father,(char*)"COPY FILE",args,i);

 
 i=0; 
 bnt_text=XmStringCreateSimple((char*)"Edit Dialog");
 XtSetArg(args[i],XmNdialogTitle,bnt_text);i++; 
 XtSetArg(args[i],XmNautoUnmanage,False);i++;  
 XtSetArg(args[i],XmNwidth,310);i++; 
 form=XmCreateForm(dia_shell,(char*)"edit_dia",args,i);
 XmStringFree(bnt_text);


  bnt_text=XmStringCreateSimple((char*)"The age is:");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  name_label=XmCreateLabel(form,(char*)"name_label",args,i);  
  XtManageChild(name_label); 
  XmStringFree(bnt_text);
 
  i=0;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;   
  XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNrightOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNleftWidget,name_label);i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  name_text=XmCreateTextField(form,(char*)"name_text",args,i);  
  XtManageChild(name_text);  
 
  bnt_text=XmStringCreateSimple((char*)"the birth is:");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,name_label);i++; 
  XtSetArg(args[i],XmNtopOffset,25);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  birth_label=XmCreateLabel(form,(char*)"birth_label",args,i);  
  XtManageChild(birth_label); 
  XmStringFree(bnt_text);
 
  i=0;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNtopWidget,name_text);i++;
  XtSetArg(args[i],XmNtopOffset,10);i++;   
  XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNrightOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNleftWidget,birth_label);i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  birth_text=XmCreateTextField(form,(char*)"birth_text",args,i);  
  XtManageChild(birth_text);  

  i=0;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNtopWidget,birth_text);i++;
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNorientation,XmHORIZONTAL);i++; 
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++; 
  toggle_box=XmCreateRadioBox(form,(char*)"radio_box",args,i);  
  XtManageChild(toggle_box);  

  bnt_text=XmStringCreateLtoR((char*)"RED",(char*)XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++;   
  tog1=XtCreateManagedWidget("tog1",xmToggleButtonWidgetClass
                          ,toggle_box,args,i);
  XmStringFree(bnt_text);
  XtAddCallback(tog1,XmNvalueChangedCallback,(XtCallbackProc)set_color_callbk,(XtPointer)"red");

  bnt_text=XmStringCreateLtoR((char*)"BLUE",(char*)XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++;   
  tog2=XtCreateManagedWidget("tog2",xmToggleButtonWidgetClass
                          ,toggle_box,args,i);
  XmStringFree(bnt_text);
  XtAddCallback(tog2,XmNvalueChangedCallback,set_color_callbk,(XtPointer)"blue");

  bnt_text=XmStringCreateLtoR((char*)"YELLOW",(char*)XmSTRING_DEFAULT_CHARSET);
  i=0;
  XtSetArg(args[i],XmNlabelString,bnt_text);i++;   
  tog3=XtCreateManagedWidget("toggle",xmToggleButtonWidgetClass
                          ,toggle_box,args,i);
  XmStringFree(bnt_text);
  XtAddCallback(tog3,XmNvalueChangedCallback,set_color_callbk,(XtPointer)"yellow");

  bnt_text=XmStringCreateSimple((char*)"Address is:");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,toggle_box);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  addr_label=XmCreateLabel(form,(char*)"addr_label",args,i);  
  XtManageChild(addr_label); 
  XmStringFree(bnt_text);
 
  i=0;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNtopWidget,toggle_box);i++;
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++; 
  XtSetArg(args[i],XmNrightOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++; 
  XtSetArg(args[i],XmNleftWidget,addr_label);i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  addr_text=XmCreateTextField(form,(char*)"addr_text",args,i);  
  XtManageChild(addr_text); 


  bnt_text=XmStringCreateSimple((char*)"Remakes:");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,addr_label);i++; 
  XtSetArg(args[i],XmNtopOffset,25);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  mark_label=XmCreateLabel(form,(char*)"mark_label",args,i);  
  XtManageChild(mark_label); 
  XmStringFree(bnt_text); 

  i=0;     
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,mark_label);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++;   
  XtSetArg(args[i],XmNwidth,300);i++; 
  XtSetArg(args[i],XmNheight,180);i++;  
  XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++;  
  scrolled_text=XmCreateScrolledText(form,(char*)"scolled_text",args,i);
  XtManageChild(scrolled_text);
 
  i=0;   
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,XtParent(scrolled_text));i++; 
  XtSetArg(args[i],XmNtopOffset,30);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,0);i++;
  XtSetArg(args[i],XmNrightAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNrightOffset,0);i++;   
  sep=XmCreateSeparator(form,(char*)"sep",args,i);  
  XtManageChild(sep);  

 
  bnt_text=XmStringCreateSimple((char*)"OK");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,sep);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNleftOffset,10);i++; 
  XtSetArg(args[i],XmNbottomAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNbottomOffset,20);i++;   
  ok_but=XmCreatePushButton(form,(char*)"ok_but",args,i);  
  XtAddCallback(ok_but,XmNactivateCallback,edit_ok_callbk,NULL);
  XtManageChild(ok_but);  
  XmStringFree(bnt_text);


 
  bnt_text=XmStringCreateSimple((char*)"APPLY");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,sep);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;  
  XtSetArg(args[i],XmNleftWidget,ok_but);i++; 
  XtSetArg(args[i],XmNleftOffset,35);i++; 
  XtSetArg(args[i],XmNbottomAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNbottomOffset,20);i++;   
  apply_but=XmCreatePushButton(form,(char*)"apply_but",args,i);  
  XtManageChild(apply_but); 
  XmStringFree(bnt_text);


 
  bnt_text=XmStringCreateSimple((char*)"CANCEL");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,sep);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;  
  XtSetArg(args[i],XmNleftWidget,apply_but);i++; 
  XtSetArg(args[i],XmNleftOffset,35);i++; 
  XtSetArg(args[i],XmNbottomAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNbottomOffset,20);i++;   
  cancel_but=XmCreatePushButton(form,(char*)"cancel_but",args,i);  
  XtManageChild(cancel_but); 
  XmStringFree(bnt_text);
 
  bnt_text=XmStringCreateSimple((char*)"HELP");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNtopWidget,sep);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;  
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;  
  XtSetArg(args[i],XmNleftWidget,cancel_but);i++; 
  XtSetArg(args[i],XmNleftOffset,35);i++; 
  XtSetArg(args[i],XmNbottomAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNbottomOffset,20);i++;   
  help_but=XmCreatePushButton(form,(char*)"help_but",args,i);  
  XtManageChild(help_but); 
  XmStringFree(bnt_text);
 
  return form; 

}
