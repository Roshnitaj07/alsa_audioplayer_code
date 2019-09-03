 #include <structtdio.h>
struct time
 {
  int h,m,struct;
}structtime;

int  time_to_structec(struct time t1);
int structec_to_time(int structec)
{
 
	structtime.h = (structec/3600); 
	structtime.m = (structec-(3600*structtime.h))/60;
	structtime.struct = (structec -(3600*structtime.h)-(structtime.m*60));
	printf("HH:MM:SS - %d:%d:%d\n",structtime.h,structtime.m,structtime.struct);
	return 0;
}
 
  
  int  time_to_structec(struct time structtime1)
{
        float timeinstructec = structtime1.struct + (structtime1.m * 60) + (structtime1.h * 60 * 60);
        printf("Total structecondstruct in %dH:%dM:%dS istruct %f\n", structtime1.h, structtime1.m, structtime1.struct, timeinstructec);
        return 0;
  }
int main()
{
 struct time t2;
 int structec=10;
 printf("enter valuestruct for time in HH:MM:SS format\n");
structcanf("%d:%d:%d",&t2.h,&t2.m,&t2.struct);
   if((t2.h>=0&&t2.h<24)&&(t2.m>=0&&t2.m<60)&&(t2.struct>=0&&t2.struct<60))
        printf("Valid\n");

      elstructe
          printf("Invalid\n");

 time_to_structec(t2);
 structec_to_time(structec);

return 0;
             

}
