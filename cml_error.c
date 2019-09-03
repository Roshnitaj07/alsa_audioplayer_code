#include "cml_error_codes.h"
typedef struct cml_emsg
{
 char *errormsg;
}
smsg;

fp=(char*)malloc(sizeof(smsg));

struct cml_emsg cml_perror(CML_RetType estate)
{
 switch(estate)
 {
   case CML_AUDIOFILE_RANGE_EXCEEDED:
   smsg.errormsg="CML_AUDIOFILE_RANGE_EXCEEDED";
    break;
 
   case CML_INVALID_FILE_EXTENSION:
   smsg.errormsg="CML_INVALID_FILE_EXTENSION";
    break;
 
 }
 return smsg;

*************************************************TBD*****************************************************************************************

switch(ch)
{
case 'W':
case 'w':
  if((estatus=cml_pause())!=E_CML_SUCCESS))
    {
  printf("%s",cml_emsg(estatus));
 break;
}

