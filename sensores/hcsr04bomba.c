/*usuario.c*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sched.h>
#include <time.h>

#define MUESTRAS 250

static int end;

static void endme(int dummy){
				end=1;
			   }
int main(int argc,char *argv[])
{
	/*ARCHIVO*/
	int contador=0;
	FILE *cfPtr;
	int tiempo;
	/*SENSORES*/
	int cmd0,cmd1;
	int h1,h2;
	/*actuador*/
	int cmd3;
	int fd4;
	int entero,decimal,temp,count=0;
	int dato,voltage;
	float value,conversion,evaluate;
	int i=0;
	/*tiempo*/
	time_t timer;
    char buffer[25];
    struct tm* tm_info;
    /**/
	if(argc < 3)
	{
		printf("adcbombauser entero decimal\n");
		return -1;
	}
    if (argc != 3) 
    {
        printf("Usage: %s entero decimal \n",argv[0]);
        return 1;
    }

	entero=atoi(argv[1]);
	decimal=atoi(argv[2]);
	temp=decimal;
	while(temp)
	{
      temp=temp/10;
      count++;
  	}
  	printf("count %d\n",count);
  	evaluate = (float) decimal;

  	for(i=0;i<count; i=i+1)
  	{
  		evaluate =evaluate*0.1;
  	}
	value= entero*1.0+evaluate;
	printf("entero %d, decimal %d\n",entero,decimal);
	printf("value %f\n",value);
	conversion=(value*4096.0/5.0);
	printf("conversion %f\n",conversion);
	voltage = (int)conversion;
	printf("voltage %d\n",voltage);
	if (voltage==4096)
	{
		voltage=4095;
	}
	if (voltage>4096)
	{
		voltage=0;
	}

	/**/
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


	if ((cmd3 = open("/dev/rtf/3",O_RDONLY))<0)
			{
			fprintf(stderr,"ERROR ABRIENDO /dev/rtf/3\n");	
			exit(1);			
			}

	if((fd4=open("/dev/rtf/4",O_WRONLY))<0)
	{
		fprintf(stderr,"Error abriendo /dev/rtf/4 \n");
		exit(1);
			}
	if ((cfPtr = fopen("SCOPE","w")) == NULL)
		printf("El archivo no puede ser abierto\n");
	else{
		while(!end){
			write(fd4, &voltage, sizeof(voltage));
			time(&timer);
			tm_info=localtime(&timer);
			read(cmd3,&dato,sizeof(dato));
			printf("ValSend(V.): %3f,De0-4096Send: %d, De0-4096Recv:%d ,DACRecev(V.): %f\n",value,voltage,dato,(dato*5.0)/(4096.0)); 
			read(cmd0,&h1,sizeof(h1));
			read(cmd1,&h2,sizeof(h2));
			printf("H1(cm):  %.1f, H2(cm):  %.1f\n",(h1/58000.0),(h2/58000.0)); 
			//ESCRIBE EN EL ARCHIVO
			if(contador < MUESTRAS){
				    strftime(buffer, 25, "%Y:%m:%d  %H:%M:%S",tm_info);
				    puts(buffer);
					fprintf(cfPtr,"T: %s     h(cm): %.1f\n",buffer,(h2/58000.0));
					contador++;
					}
			if (contador=MUESTRAS){
				contador=0;
			}
			}
		}	
	close(fd4);
	return 0;
	}