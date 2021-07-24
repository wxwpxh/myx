#include "scrolllist.h"
#include <iostream>  
using namespace std; 


void scrolllist_add_callbk(Widget w,XtPointer client_data,XtPointer call_data)
{
 Widget father;
 father=XtParent(w);
 XtUnmanageChild(father);  
} 
 
void scrolllist_chg_callbk(Widget w,XtPointer client_data,XtPointer call_data) 
{
 Widget list1=w,list2;
 XmString item; 
 XmListCallbackStruct *cbs;
   
 list2=(Widget)client_data;
 cbs=(XmListCallbackStruct *)call_data;
 item=cbs->item;   
 XmListAddItem(list2,item,0);
 XmListDeleteItem(list1,item);
 
} 

 
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
    };
  
    cout<<"cbs->reason:"<<reason<<",cbs->value:"<<cbs->value
        <<",cbs->pixel"<<cbs->pixel<<endl;  
 

  } 




Widget  create_scrolled_list(Widget parent)
{
   Widget  list_w,sb,selecform,dia_shell,add_but,list_w1,back_but;   
   char  *items[]={(char *)"choise0",(char *)"choice1",(char *)"choice2",(char *)"choise3",(char *)"choice4",(char *)"choice5",
                (char *)"choice6",(char *)"choice7",(char *)"choice8",(char *)"choise9",(char *)"choice10",
                (char *)"choice11",(char *)"choice12",(char *)"choice13",(char *)"choice14"};
   Arg args[10]; 
   int i,n,num;  
   XmStringTable s_table;
  
   i=0;
   dia_shell=XmCreateDialogShell(parent,(char *)"COPY FILE",args,i);  


 
  i=0; 
  XmString bnt_text=XmStringCreateSimple((char *)"Edit Dialog");
  XtSetArg(args[i],XmNdialogTitle,bnt_text);i++; 
  XtSetArg(args[i],XmNautoUnmanage,False);i++;  
  XtSetArg(args[i],XmNwidth,220);i++; 
  XtSetArg(args[i],XmNheight,120);i++; 
  selecform=XmCreateForm(dia_shell,(char *)"edit_dia",args,i);
  XmStringFree(bnt_text);

   num=XtNumber(items);
   s_table=(XmStringTable)XtMalloc(num*sizeof(XmString*));
   for(n=0;n<num;n++)
      s_table[n]=XmStringCreateSimple(items[n]);


    i=0; 
    XtSetArg(args[i],XmNitemCount,15);i++; 
    XtSetArg(args[i],XmNvisibleItemCount,5);i++; 
    XtSetArg(args[i],XmNitems,s_table);i++;  
    XtSetArg(args[i],XmNleftAttachment,XmATTACH_FORM);i++;
    XtSetArg(args[i],XmNleftOffset,10);i++;  
    XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++;  
    XtSetArg(args[i],XmNtopOffset,10);i++;     
    list_w=XmCreateScrolledList(selecform,(char *)"list_w",args,i);   
    XtManageChild(list_w);   

   //pay attention:
  //  list_w parent is scrolled_window and scrolled_window parent is parent 

   XtVaGetValues(XtParent(list_w),XmNverticalScrollBar,&sb,NULL);
 

   XtAddCallback(sb,XmNvalueChangedCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNdragCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNincrementCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNdecrementCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNpageIncrementCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNpageDecrementCallback,scrollaction_callbk,NULL); 
   XtAddCallback(sb,XmNtoTopCallback,scrollaction_callbk,NULL);  
   XtAddCallback(sb,XmNtoBottomCallback,scrollaction_callbk,NULL); 
 
   
  bnt_text=XmStringCreateSimple((char *)"ADD->");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNleftWidget,XtParent(list_w));i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++;  
  XtSetArg(args[i],XmNtopOffset,15);i++;     
  add_but=XmCreatePushButton(selecform,(char *)"add_but",args,i);  
  XtAddCallback(add_but,XmNactivateCallback,scrolllist_add_callbk,NULL);
  XtManageChild(add_but);  
  XmStringFree(bnt_text);
  

  
  bnt_text=XmStringCreateSimple((char *)"<-BACK");
  i=0;  
  XtSetArg(args[i],XmNlabelString,bnt_text);i++; 
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNleftWidget,XtParent(list_w));i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_WIDGET);i++;  
  XtSetArg(args[i],XmNtopWidget,add_but);i++; 
  XtSetArg(args[i],XmNtopOffset,10);i++;     
  back_but=XmCreatePushButton(selecform,(char *)"back_but",args,i);  
  XtAddCallback(back_but,XmNactivateCallback,scrolllist_add_callbk,NULL);
  XtManageChild(back_but);  
  XmStringFree(bnt_text);

  
  i=0; 
  XtSetArg(args[i],XmNitemCount,0);i++; 
  XtSetArg(args[i],XmNvisibleItemCount,5);i++;  
  XtSetArg(args[i],XmNtopAttachment,XmATTACH_FORM);i++;
  XtSetArg(args[i],XmNtopOffset,10);i++;     
  XtSetArg(args[i],XmNleftAttachment,XmATTACH_WIDGET);i++;
  XtSetArg(args[i],XmNleftWidget,add_but);i++; 
  XtSetArg(args[i],XmNleftOffset,10);i++;    
  list_w1=XmCreateScrolledList(selecform,(char *)"list_w1",args,i);   
  XtManageChild(list_w1); 

  
 
  XtAddCallback(list_w,XmNdefaultActionCallback,scrolllist_chg_callbk,list_w1);
  XtAddCallback(list_w1,XmNdefaultActionCallback,scrolllist_chg_callbk,list_w);
 

  return selecform;

}
 
