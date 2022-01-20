#include <TimerOne.h>
unsigned long counter=0;

typedef union{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t myvalue;

int pwmout=3;
int factor=60;//para convertir rps a rpm
float factor2=0.104719; //para convertir rpm a rad/sec
int vel=0;
String value;
int duty=0;

void docount(){ //conteo desde el sensor 
  counter++;  //incrementa el valor del contador en 1
 }
 
void timerIsr(){
  Timer1.detachInterrupt();//paramos el timer1
  //Serial.print("RPM: ");
  //int rpm=counter*60*10; //conversion de revoluciones por segundo a min
  //float rads=rpm*factor2; //conversión a radianes por segundo
  //Serial.print("Rad/sec: ");
  //Serial.println(rads);
  //String stringOne=String(rads,3);
  float valor=(float)counter;
  myvalue.number=valor;
  Serial.write('A'); 
  for (int i=0; i<4; i++){
     Serial.write(myvalue.bytes[i]); 
    }
  Serial.print('\n');
  counter=0; //reseteamos el contador
  Timer1.attachInterrupt(timerIsr); //volvemos a habilitar la interrupcion del timer1
   
  }
void setup(){
  Serial.begin(9600);
  Timer1.initialize(100000);// Accedemos al temporizador 1 y establecemos el tiempo de 100ms
  attachInterrupt(0, docount,RISING);//configuramos la interrupción por entrada ascendente en el pin2, esta
                                     //interrupción llama a la función docount
  Timer1.attachInterrupt( timerIsr );// configura interrupción
                                    //del timer1
  
  }

void loop(){
  if (Serial.available() >0){
    value=Serial.readStringUntil('\n');
  }
  vel=value.toInt();
  duty= map(vel,0,12,0,255);
  analogWrite(pwmout,duty);
 
  }
