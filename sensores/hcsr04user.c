/*usuario.c*/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>

static int end;

static void endme(int dummy){
				end=1;
			   }
int main(void)
{
	int cmd0,cmd1;
	int h1,h2;
	signal(SIGINT , endme);

	if ((cmd0 = open("/dev/rtf/1",O_RDONLY))<0)
			{
			fprintf(stderr,"ERROR ABRIENDO /dev/rtf/1\n");	
			exit(1);			
			}


	if ((cmd1 = open("/dev/rtf/2",O_RDONLY))<0)
		{
		fprintf(stderr,"ERROR ABRIENDO /dev/rtf/2\n");	
		exit(1);			
		}
	while(!end){
		read(cmd0,&h1,sizeof(h1));
		read(cmd1,&h2,sizeof(h2));
		printf("H1(cm):  %.1f, H2(cm):  %.1f\n",(h1/58000.0),(h2/58000.0)); 
		}

	}