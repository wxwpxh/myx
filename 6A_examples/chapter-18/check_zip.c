#include <Xm/Xm.h>

/* check_zip() -- limit the user to entering a ZIP code. */
void check_zip (Widget text_w, XtPointer client_data, XtPointer call_data)
{
	XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) call_data; 
	int                         len = XmTextGetLastPosition (text_w);
	
	if (cbs->startPos < cbs->currInsert) /* backspace */
		return;

	if (len == 5) {
		cbs->doit = False;
		return;
	}
	
	/* check that the new additions won't put us over 5 */
	if (len + cbs->text->length > 5) {
		cbs->text->ptr[5 - len] = 0;
		cbs->text->length = strlen (cbs->text->ptr);
	}
	
	for (len = 0; len < cbs->text->length; len++) {
		/* make sure all additions are digits. */
		
		if (!isdigit (cbs->text->ptr[len])) {
			/* not a digit-- move all chars down one and 
			** decrement cbs->text->length.
			*/
			
			int i;
			
			for (i = len; (i+1) < cbs->text->length; i++)
				cbs->text->ptr[i] = cbs->text->ptr[i+1];

			cbs->text->length--;
			len--;
		}
	}
	
	if (cbs->text->length == 0)
		cbs->doit = False;
}
