CONFIGURACION DEL VARIADOR DE FRECUENCIA 


NOTA IMPORTANTE :
 AL TRATAR DE CONTROLAR LA BOMBA SIN ANTES HABER VERIFICADO QUE EL CEBADO DE LA MISMA HA SIDO
 REALIZADO SATISFACTORIAMENTE RESULTA PRACTICAMENTE IMPOSIBLE Y TOMA  MUCHO TIEMPO OBTENER UN BUEN
 FUNCIONAMIENTO, ES POR ELLO QUE ANTES DE HACER CUALQUIER COSA, DEBEMOS CONECTAR LA BOMBA DIRECTAMENTE
 A 220VOLTIOS Y VERIFICAR QUE FUNCIONA  BIEN  Y RECIEN PROCEDER A CONTROLAR LA BOMBA CON EL VARIADOR.


VERIFICAR SIEMPRE LOS PARAMETROS ANTES DEL ENCENDIDO A MANERA DE PREVENCION Y PROTECCION DE LA BOMBA DE AGUA.

ejemplo de configuración:

PARAMETRO

PR1 0  = 60HZ
PR4-00 = 0 HZ
PR4-01 = 0 BIAS POLARITY
PR4-02 = 200 %POT FREQ GAIN

PR4-03 = 0 POT REV MOTOPM EMABLE


PR 4-02 = (10/5) *100%



PARAMETRO SEGUN MANUAL  QUE YO CONSIDERO DEBEMOS VERIFICAR  Y CALIBRAR SIEMPRE PARA NO QUEMAR LA BOMBA
***************************************************************
GRUPO1
***************************************************************
1-00
MAXIMUN OUTPUT FREQUENCY------------> 60 HZ  ---d60

1-01
MAXIMUM VOTAGE FREQUENCY------------> 60 HZ  ---d60

1.02
MAX OUTPUT VOLTAGE       -----------> 220 V------d220

1-03
MID POINT FREQUENCY       ---------> 1HZ ----- d1.0


1-04
mid point voltage         ---------> 65v ------d65.0

1-05
MINIMUN OUTPUT FREQUENCY  ---------> 1HZ ----- d1.0


1-06
MINIMUN OUTPUT VOLTAGE  ---------> 65 V  ----- d65.0

CURVA VOLTAJE VS FREQUENCIA


pr1-02   |       /
         |      /
         |     /
         |    / 
pr1-04   |   /
pr1-06   |  /
         -------------
         pr1-03		 pr1-00


         pr1-05		 pr1-01 

***************************************************************
GRUPO2
***************************************************************
		CONTROL DE FRECUENCIA
2-00  d0 keypad nunca lo uso
	  d1 DC 0-10 V ESTE LO USO PERO EN MI CASO SOLO TENGO PARA VARIAR DE 0 A 		5VOLTIOS
	  d3 POTENTIOMETRO QUE ESTA EN EL VARIADOR PARA PRUEBAS Y CALIBRACIONES EXCELENTE

	  CONTROL DEL VARIADOR
2-01  d0 todo desde el key pad



***************************************************************
GRUPO4
***************************************************************
GANANCIA
 4-02 D200 PARA QUE TENGAMOS UN VOLTAJE DE CONTROL DE 0-10V cuando solo tenemos 			para dar 5 voltios


***************************************************************
GRUPO6
***************************************************************
6-00 OVER VOLTAGE PROTECTION d390
6-01 PARAMETRO MUY IMPORTANTE QUE ME SALVARA DE NO QUEMAR LA BOMBA
		 PONER APROXIMADAMENTE 53s%
		 	d53

***************************************************************
GRUPO7
***************************************************************
este PARAMETRO AUN NO ESTOY MUY SEGURO


7-00 YO LO VOY A CONSIDERAR COMO 52%
 PARAMETRO LLAMADO RATED CURRENT  LIMITA LA SALIDA DE CORRIENTE DEL DRIVE PARA PREVENIR EL SOBRECALENTAMIENTO DEL MOTOR

