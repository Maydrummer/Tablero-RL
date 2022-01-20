//***************************************************//
//***************************************************//
//*****   Contador de RPM y controlador de      *****//
//*****   de velocidad mediante la técnica      *****//
//*****   PWM para un motor DC.                 *****//
//*****                                         *****//
//*****                                         *****//
//***** by: Anthony Maisincho Jivaja,           *****//
//*****     Ulbio Alejandro Sanjinés            *****//
//*****                                         *****//
//***************************************************//
//***************************************************//


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
  variableSelector=digitalRead(selector); //Leemos la entrada del selector
  //Serial.print("PWM actual: ");
  //Serial.println(outputValue);
  delay(100);
  if(variableSelector==0){  //Si es cero podemos aumentar o disminuir el pwm de 1 en 1
    int estadoInicio=digitalRead(inicio);
    int estadoParada=digitalRead(parada);
    //Serial.println(outputValue);
    if(estadoInicio==HIGH){ //Si existe un cambio de estado de 0-1
      aux=1; //Aux vale 1
        }
    else if(aux==1 && estadoInicio==LOW){ //Si se deja de presionar y aux fue 1
      outputValue+=1; //Se aumenta la variable PWM en 1
      analogWrite(pwmout,outputValue); //Se manda el PWM al motor
      aux=0; //Reseteamos la variable aux 
        }
    else if(estadoParada==LOW){ //Si ahora en el pulsador Normalmente cerrado se detecta un cambio de 1-0
      aux2=1; //Aux2 se le asigna el 1
        }
    else if(aux2==1 && estadoParada==HIGH){ //Si se deja de presionar el pulsador NC y si Aux2 vale 1 
      outputValue-=1; //El valor de salida de PWM se disminuye en 1
      analogWrite(pwmout,outputValue); //Se escribe el valor actual de PWM
      aux2=0;  //Se reseta la variable
    }
    else if(outputValue>29){ //Si el valor de PWM sobrepasa 29
      outputValue=30;  //Se limita el valor de salida de PWM a 30
      analogWrite(pwmout,outputValue);
      
      }
    else if(outputValue< 0){ //Si es mejor a cero
      outputValue=0;   //Se limita a cero la variable de salida
      analogWrite(pwmout,outputValue);
      } }
    else{//Si el selector pasa a otro estado, se apaga el motor
      outputValue=0;
      analogWrite(pwmout,outputValue);
      }
}
