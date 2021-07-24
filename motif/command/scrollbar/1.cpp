#include<iostream.h>
#include<stdio.h>
#include <Xm/XmAll.h>
 
void ReadFiletoXmtext(char *filename,Widget text){
 XmTextPosition pos; 
 char value[2]="p";
 FILE *fp;

if((fp=fopen(filename,"r"))==NULL){
 cerr<<"can not open the file:"<<filename<<endl;
 exit(0); 
 };

while(!feof(fp)){
fread(value,sizeof(char),1,fp); 
pos=XmTextGetLastPosition(text);
XmTextInsert(text,pos,value);
}

fclose(fp);

}


 
void get_sb(Widget pb,caddr_t client_data,caddr_t call_data)
{
 Widget scrollbar;
 int inc=0,max=0,min=0,pg_inc=0,slider_size=0,value=0;

 scrollbar=(Widget)client_data;
   
 XtVaGetValues(scrollbar,
 XmNincrement,&inc,
 XmNmaximum,&max,
 XmNminimum,&min,
 XmNpageIncrement,&pg_inc,
 XmNsliderSize,&value,
 NULL
 );

cout<<"increment="<<inc<<",max="<<max<<",mini="<<min<<",page="<<pg_inc<<",slider="<<slider_size<<
    ",value="<<value<<endl;
} 

void sb_inc(Widget w,caddr_t client_data,caddr_t call_data)
{
 /*
 Widget text;
 XmTextPosition pos;
 text=(Widget)client_data;
 
 pos=XmTextGetLastPosition(text);
 XmTextInsert(text,pos,"hellow world\n");
*/
 printf("you click to inc the vertical bar.\n");
} 

 
void main( int argc,char **argv)

 {
 
  Widget toplevel,rowcol,text_w,pb,sb;  
  XtAppContext  app_context; 
  Arg args[10];
  int i;
  toplevel=XtVaAppInitialize(&app_context,"Demos",NULL,0,&argc,argv,NULL,NULL);    //X  initialize  
 

 rowcol=XtVaCreateWidget("rolcol",xmRowColumnWidgetClass,toplevel,NULL);

 i=0; 
 XtSetArg(args[i],XmNrows,10);i++; 
 XtSetArg(args[i],XmNcolumns,80);i++;
 XtSetArg(args[i],XmNeditMode,XmMULTI_LINE_EDIT);i++; 
 XtSetArg(args[i],XmNscrollHorizontal,False);i++; 
 XtSetArg(args[i],XmNwordWrap,True);i++;  
 text_w=XmCreateScrolledText(rowcol,"text_w",args,i);  
 XtManageChild(text_w);

 XtVaGetValues(XtParent(text_w),XmNverticalScrollBar,&sb,NULL);
 pb=XtVaCreateManagedWidget("Scrollbar",xmPushButtonGadgetClass,
  rowcol,NULL
  );

 XtAddCallback(pb,XmNactivateCallback,get_sb,sb); 

 XtAddCallback(sb,XmNincrementCallback,sb_inc,text_w); 
 
   
  XtManageChild(rowcol);
  
  ReadFiletoXmtext("/root/motif/command/makefile",text_w);

  XtRealizeWidget(toplevel); 
  XtAppMainLoop(app_context);
  

}




