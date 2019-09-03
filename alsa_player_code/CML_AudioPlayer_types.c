/******************************************************************************
 *
 *
 *   ALLGO EMBEDDED SYSTEMS CONFIDENTIAL PROPRIETARY
 *
 *    (C) 2019 ALLGO EMBEDDED SYSTEMS PVT. LTD.
 *
 *   FILENAME        - CML_types.h
 *
 *   COMPILER        - gcc 5.4.0
 *
 ******************************************************************************
 *  CHANGE HISTORY
 *   ---------------------------------------------------------------------------
 *   DATE           REVISION      		AUTHOR                  COMMENTS
 *   ---------------------------------------------------------------------------
 *   26/08/2019     1.0         	Ankit Raj,Roshini         Initial Version
 *   					  	Taj,Aishwarya M
 *  
 ********************************************************************************
 *   DESCRIPTION
 *   This file contains all audio player related internal header functions implimentation.
 *   
 ******************************************************************************/
#include"CML_types.h"
#include"CML_AudioPlayer_types.h"


CML_VOID swap (struct CML_myplaylist *a, struct  CML_myplaylist *b)  
{  
  struct  CML_myplaylist temp = *a;  
    *a = *b; 
    *b = temp;  
}
CML_VOID cml_shuffle(CML_VOID)
{
  srand (time(NULL));  
  for (int i = sCurFile.icount - 1; i >= 0; i--)  
    {
   int j = rand() % (i + 1); 
          swap(&samyplaylist[i], &samyplaylist[j]);  
    }   
}

CML_UINT32 cml_checkinput(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[])
{
    CML_UINT32 i;
    CML_CHAR *pc;
    CML_INT32 iret;
    if(iNoOfargs<2)
    {
       
        iret = -1;
    }
    else
    {
        if((strcmp(pacargslist[1],"-s"))==0 || (strcmp(pacargslist[1],"-S"))==0)
        
            {
                pc=strrchr(pacargslist[2],'.');
                if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
                {
                   iret = 1;
                }
                else if(NULL == pacargslist[2])
                {
                    
                    iret = -1 ;
                }
                   
            }
        else
        {
            pc=strrchr(pacargslist[1],'.');
            if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
            {
                iret=0;
            }
            

        }
    }
    if(-1 == iret)
    {
     
    }
    return iret;
}


CML_VOID CML_myplaylist(CML_INT32 iNoOfargs,CML_CHAR *pacargslist[])
{
     CML_FILE *fp;
     CML_CHAR str[256]; 
    
     sCurFile.icount=0;
     CML_CHAR *pfext,fline[256];
     CML_CHAR clen;

     for(i=1;i<iNoOfargs;i++)
     {
         pfext=strrchr(pacargslist[i],'.');
         if(strcmp(pfext,".wav")==0)
           
         {
             
             clen=strlen(pacargslist[i]);
             samyplaylist[sCurFile.icount].pfilepath=(char*)malloc((clen+1)*sizeof(char));
             if(NULL == samyplaylist[sCurFile.icount].pfilepath)
             {
                 printf("Memory not allocated");
                 continue;
             }
             strncpy( samyplaylist[sCurFile.icount++].pfilepath,pacargslist[i],clen);
               
         }
         else if(strcmp ( pfext,".txt" ) == 0)
         {
             fp=fopen(pacargslist[i],"r");
             if(NULL == fp)
             {
                 printf("couldn,t open file empty file at %dth arguments\n",i);
                 continue;
             }
             else
             {
                
                 while(fgets ( fline, sizeof (fline), fp ) != NULL)
                 {
                     if(strcmp( fline,"\n")!=0)
                     {
                         pfext=strrchr(fline, '.');
                         if(strcmp(pfext, ".wav\n") == 0)
                         {
                             fline[strlen(fline)-1]='\0';
                             clen=strlen(fline);
                             samyplaylist[sCurFile.icount].pfilepath=(char*)malloc((clen+1)*sizeof(char));
                              if(NULL == samyplaylist[sCurFile.icount].pfilepath)
                                   {
                                      printf("Memory not allocated");
                                        continue;
                                    }
                             strncpy( samyplaylist[sCurFile.icount++].pfilepath, fline,clen);
                             
                         }
                     }
                 }
             }
             
             fclose(fp);
         } 
         else
        {
            printf("invalid file extension at %dth arguments\n",i);
        }
     }
           
}


CML_VOID cml_extractheaderinfo(CML_VOID)
{
   // CML_UINT32 iheader;
    CML_CHAR *cheader;
    cheader=(CML_CHAR*)malloc(44);
     if(NULL == cheader)
             {
                 printf("Memory not allocated");
                 return;
             }
    
    //iheader=open(samyplaylist[sCurFile.icurfileindex].pfilepath,O_RDONLY);

    if(read(sfiledesc.ifiledesc,cheader,44)!=44)
    {
        printf("Unable to read header data\n");
          return;
    }
               /*overall size*/
    swavheaderinfo.isize=(*(CML_UINT32 *)(cheader+4));
	
			   /*no of channels*/
	swavheaderinfo.uwchannel=(*(CML_UINT32 *)(cheader+22));
	
			  /*Sample Rate*/
	swavheaderinfo.isamplerate=(*(CML_UINT32 *)(cheader+24));
	 
			    /*BYTE RATE*/
	swavheaderinfo.ibyterate=(*(CML_UINT32 *)(cheader+28));
	
			     /*Time*/
	swavheaderinfo.iduaration= (int)(swavheaderinfo.isize)/(swavheaderinfo.ibyterate);
	
    //close(iheader);
    free(cheader);

}
