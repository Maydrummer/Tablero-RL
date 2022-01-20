#include <TimerOne.h>
unsigned long counter=0;

int outputValue=0;
int pwmout=3;
int inicio=4;
int parada=5;
int selector=6; // 0-->manual mode  1--->automatico
int variableSelector;
int aux; 
int aux2;

void docount(){ //conteo desde el sensor 
  counter++;  //incrementa el valor del contador en 1
 }
 
void timerIsr(){
  Timer1.detachInterrupt();//paramos el timer1
  Serial.print("$");
  Serial.print(counter);
  Serial.print(",");
  Serial.println(outputValue);
  counter=0; //reseteamos el contador
  Timer1.attachInterrupt(timerIsr); //volvemos a habilitar la interrupcion del timer1
   
  }
void setup() {
  Serial.begin(9600);
  Timer1.initialize(100000);// Accedemos al temporizador 1 y establecemos el tiempo de 100ms
  attachInterrupt(0, docount,RISING);//configuramos la interrupción por entrada ascendente en el pin2, esta
                                     //interrupción llama a la función docount
  Timer1.attachInterrupt( timerIsr );// configura interrupción
                                    //del timer1
}

void loop() {
  variableSelector=digitalRead(selector);
  //Serial.print("PWM actual: ");
  //Serial.println(outputValue);
  delay(100);
  if(variableSelector==0){
    int estadoInicio=digitalRead(inicio);
    int estadoParada=digitalRead(parada);
    //Serial.println(outputValue);
    if(estadoInicio==HIGH){
      aux=1;
        }
    else if(aux==1 && estadoInicio==LOW){
      outputValue+=1;
      analogWrite(pwmout,outputValue);
      aux=0;        
        }
    else if(estadoParada==LOW){
      aux2=1;
        }
    else if(aux2==1 && estadoParada==HIGH){
      outputValue-=1;
      analogWrite(pwmout,outputValue);
      aux2=0;        
    }
    else if(outputValue>29){
      outputValue=30;
      analogWrite(pwmout,outputValue);
      
      }
    else if(outputValue< 0){
      outputValue=0;     
      analogWrite(pwmout,outputValue);
      } }
    else{
      outputValue=0;
      analogWrite(pwmout,outputValue);
      }
}
