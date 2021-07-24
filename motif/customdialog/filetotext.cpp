


#include "filetotext.h"

void ReadFiletoXmtext(char *filename,Widget text){
 XmTextPosition pos; 
 char value[81];
 FILE *fp;

 value[80]=0; 

if((fp=fopen(filename,"r"))==NULL){
 cerr<<"can not open the file:"<<filename<<endl;
 exit(0); 
 };

XmTextSetString(text,(char *)"");
while(!feof(fp)){
    fread(value,sizeof(char),80,fp);
    pos=XmTextGetLastPosition(text);
    XmTextInsert(text,pos,value);
}

fclose(fp);

}

