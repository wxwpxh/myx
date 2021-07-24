#include<stdio.h>
#include<stdlib.h>
#include <Xm/XmAll.h>

#include<iostream>
using namespace std;

 
void scrollaction_callbk(Widget sb,XtPointer client_data,XtPointer call_data)
{  
   char reason[20];
   XmScrollBarCallbackStruct *cbs;
   
    
   cbs=(XmScrollBarCallbackStruct *)call_data;
   switch(cbs->reason){
    case XmCR_DRAG :strcpy(reason,"darg");break;
    case XmCR_VALUE_CHANGED :strcpy(reason,"value changed");break; 
    case XmCR_INCREMENT :strcpy(reason,"increment");break;
    case XmCR_DECREMENT :strcpy(reason,"decrement");break;
    case XmCR_PAGE_INCREMENT :strcpy(reason,"page increament");break;
    case XmCR_PAGE_DECREMENT :strcpy(reason,"page decreament");break;
    case XmCR_TO_TOP :strcpy(reason,"top");break;
    case XmCR_TO_BOTTOM :strcpy(reason,"bottom");break;
    }
  
    cout<<"cbs->reason:"<<reason<<",cbs->value:"<<cbs->value
        <<",cbs->pixel"<<cbs->pixel<<endl;  
  
 };

 
int main( int argc,char **argv)

 {
 
  Widget toplevel,list_w,sb;  
  XtAppContext  app_context;
  char  *items[]={(char *)"choise0",(char *)"choice1",(char *)"choice2",(char *)"choise3",(char *)"choice4",(char *)"choice5",
                (char *)"choice6",(char *)"choice7",(char *)"choice8",(char *)"choise9",(char *)"choice10",
                (char *)"choice11",(char *)"choice12",(char *)"choice13",(char *)"choice14"};
   Arg args[10]; 
   int i,n,num;  
   XmStringTable s_table;

  toplevel=XtVaAppInitialize(&app_context,"Demos",NULL,0,&argc,argv,NULL,NULL);
   //X  initialize    


 num=XtNumber(items);
 s_table=(XmStringTable)XtMalloc(num*sizeof(XmString*));
 for(n=0;n<num;n++)
   s_table[n]=XmStringCreateSimple(items[n]);


 i=0; 
 XtSetArg(args[i],XmNitemCount,15);i++; 
 XtSetArg(args[i],XmNvisibleItemCount,5);i++; 
 XtSetArg(args[i],XmNitems,s_table);i++;  
 list_w=XmCreateScrolledList(toplevel,(char *)"list_w",args,i);  
 XtManageChild(list_w);



 XtVaGetValues(XtParent(list_w),XmNverticalScrollBar,&sb,NULL);
 

 XtAddCallback(sb,XmNvalueChangedCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNdragCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNincrementCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNdecrementCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNpageIncrementCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNpageDecrementCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNtoTopCallback,scrollaction_callbk,NULL); 
 XtAddCallback(sb,XmNtoBottomCallback,scrollaction_callbk,NULL); 
 
  for(n=0;n<num;n++)
     XmStringFree(s_table[n]);

  XtRealizeWidget(toplevel); 
  XtAppMainLoop(app_context);
  
  return 0;
}




