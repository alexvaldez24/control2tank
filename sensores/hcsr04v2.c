#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>
#include <math.h>

MODULE_LICENSE("GPL");

/*PARAMETROS DE CONFIGURACION DE LA TAREA TIEMPO REAL*/
#define STACK_SIZE  2000
#define TICK_PERIOD 100000   //  1 ms
#define PERIODE_CONTROLS 250000000 //10ms
#define PERIODE_CONTROLD 100000000
#define N_BOUCLE 10000000
#define NUMERO 1
#define PRIORITE1  0
#define PRIORITE2  1
#define TACHE_ENTREE_ID  1
/*CREACION DE FIFOS*/
#define FIFO_0 1
#define FIFO_1 2
#define FIFO_3 3
#define FIFO_4 4
#define FIFO_5 5
/*REGISTRO DEL DAC*/
#define AD_BASE8 0X11E00100
#define AD_BASE16 0X21E00100
#define AD_LENGHT 0X1E
#define AD_BASE8 0X11E00100
#define AD_JUMPER (address)+0x01
/* PARAMETROS DAC*/
volatile u32 *address;
volatile u32 *address16;
u32 ADC_BASE8 = 0;
u32 ADC_BASE16 = 0;
u16 ADleido = 0;
unsigned int DACsalida = 0;
u8 channel = 0;
/*PARAMETROS SENSORES*/
u16 status=0;
volatile unsigned int *portB_ddr;
volatile unsigned int *portB_data;
RTIME now1;
RTIME now2;
RTIME now3;
RTIME now4;
RTIME now5;
static RT_TASK tache_horloge;
static RT_TASK task1;

int lire_sensor(void){
	int p,h1,h2;
	p=0;
	int save4;

	while(1){
/***********************************************************/
		rt_task_wait_period();		
		if(p==0){
			printk("..SENSOR 1..!\n");
			writeb(0x00,portB_data);
			rt_sleep(nano2count(2000));
			writeb(0xF0,portB_data);
			rt_sleep(nano2count(10000));
			writeb(0x00,portB_data);
			status=(readb(portB_data) & 0x01);
			printk("portb_data: %x \n",readb(portB_data));
			printk("status:  %d\n",status);
			now1=rt_get_time_ns();
			while(status == 0){
	         					status=readb(portB_data) & 0x01;
	         					now2=rt_get_time_ns();
	         					if((now2-now1)>500000)
	         					{
	         						break;
	         					}
			 				  }
			now3=rt_get_time_ns();
			printk("n1 %u\n",now3-now1);
			now4=rt_get_time_ns();
			while(status ==1){
	       				     status= readb(portB_data) & 0x01; 		  
	         				 }
	        now5=rt_get_time_ns();
	        h1=now5-now4;
	       	printk("Tiempo 1 %d\n\n", h1);
   	    	rtf_put(FIFO_0, &h1, sizeof(h1));   
	       	p=1;
		}else{ 
		if(p==1){	
	/***********************************************************/
	       	printk("..SENSOR 2!..\n");
		    writeb(0x00,portB_data);
		    rt_sleep(nano2count(2000));
		    writeb(0xF0,portB_data);
		    rt_sleep(nano2count(10000));
		    writeb(0x00,portB_data);
		    status=(readb(portB_data) & 0x02);
		    printk("portb_data: %x \n",readb(portB_data));
		    printk("status2:  %d\n",status);
		    now1=rt_get_time_ns();
		    while(status == 0){
		                  status=readb(portB_data) & 0x02;
		                  now2=rt_get_time_ns();
		                  if((now2-now1)>500000)
		                  {
		                    break;
		                  }
		              }
		    now3=rt_get_time_ns();
		    printk("n2 %u\n",now3-now1);
			now4=rt_get_time_ns();
		    while(status ==2){
	                    	status= readb(portB_data) & 0x02;
		                 	}
		    now5=rt_get_time_ns();
	        h2=now5-now4;
	       	printk("Tiempo 2 %d\n\n", h2);
	       	rtf_put(FIFO_1, &h2, sizeof(h2)); 

	    	p=0;}
		}
	  }
} 


static void dacsin(int arg) {
int b=4095;
       while(1) {
  			   rtf_get(FIFO_4,&b,sizeof(b));
// Inicializacion de Maquina ADC ----------------- COMPULSOR
               writeb(0xE1,ADC_BASE8+0x02); // Prendo el AD
               writeb(0x01,ADC_BASE8+0x03);
               DACsalida = b;
               writeb((DACsalida & 0x0ff),ADC_BASE8+0x0E);
               //printk("DACCMD SOLO 0X0E registro de 8 bits=%x\n",readb(ADC_BASE8+0x0E));
               writeb(0x20 + (DACsalida >> 8),ADC_BASE8+0x0F);
// Apagado de Mquina AD ---------------------- COMPULSORY
               writeb(0xF0,ADC_BASE8+0x02); // Apago el AD
               writeb(0x01,ADC_BASE8+0x03);                          
/*************TODO ***************************/
               //printk("W ADC es igual a: %d   Z DAC es igual a    %d\n",w,z);
               printk("DACsalida   %d\n",b);
               if(b>4095)
               {
                b=0;
               }
               rtf_put(FIFO_3, &b, sizeof(b));
               rt_task_wait_period();
       }
}

int init_module(void){
	printk("[START] Application\n");
	int ierr;
	RTIME now;
// Mapeando memoria REGISTO  GPIO -------------------------- COMPULSORY

	portB_ddr= __ioremap(0x80840014,1,NULL); //mapeo DDR_PORTB	 DIRECTION REGISTER
	portB_data=__ioremap(0x80840004,1,NULL); //mapeo data register
	writeb(0xF0,portB_ddr);
// Mapeando memoria REGISTRO DAC -------------------------- COMPULSORY
   address = __ioremap(0x11E00100, 0x1E, 0);
   address16 = __ioremap(0x21E00100, 0x1E, 0);
   printk("Mapped register for 8 bit: %x\n",address);
   printk("Mapped register for 16 bit: %x\n",address16);
   ADC_BASE8 = address;
   ADC_BASE16 = address16;
// Escribiendo al PACER_CLOCK DAC---------------- COMPULSORY
   writeb(0x40,ADC_BASE8+0x06);
   writeb(0x01,ADC_BASE8+0x07);
   writeb(0x00,ADC_BASE8+0x04);
   writeb(0x00,ADC_BASE8+0x05);

	rt_set_oneshot_mode();
  	rt_task_init(&task1,dacsin,TACHE_ENTREE_ID,STACK_SIZE, PRIORITE2, 0, 0);
   	ierr = rt_task_init(&tache_horloge,lire_sensor,TACHE_ENTREE_ID,STACK_SIZE, PRIORITE2, 0, 0);
   	start_rt_timer(nano2count(TICK_PERIOD));

	now = rt_get_time();
	rt_task_make_periodic(&task1, now, nano2count(PERIODE_CONTROLD));
	rt_task_make_periodic(&tache_horloge, now, nano2count(PERIODE_CONTROLS));
    rtf_create(FIFO_0,20000);
    rtf_create(FIFO_1,20000);
  //  rtf_create(FIFO_3,1);

	return(0);
}

void cleanup_module(void){
	printk("[END] Application\n");
	__iounmap(address);
	__iounmap(address16);
	stop_rt_timer();
	rt_task_delete(&task1);
	rt_task_delete(&tache_horloge);
 //   rtf_destroy(FIFO_3);
	rtf_destroy(FIFO_0);
	rtf_destroy(FIFO_1);
}

u16 ADC_fifocount(){
       return ((readb(ADC_BASE8 + 0x09)<<2)|((readb(ADC_BASE8 + 0x08))>>6));
}

u8 ADC_fifohead(){
       return (((readb(ADC_BASE8 + 0x08))>>1) & 0x1f);
}