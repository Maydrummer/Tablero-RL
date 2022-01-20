import RPi.GPIO as GPIO
import os
import sys
import paho.mqtt.client as mqtt
import json
import time
import serial
import requests
GPIO.setmode(GPIO.BCM)
gpio_out=[5,6,13,16,19,20]
for gpio in gpio_out:
    GPIO.setup(gpio,GPIO.OUT)
THINGSBOARD_HOST="mqtt.thingsboard.cloud"
ACCESS_TOKEN="hola123"
datos_sensor={'RPM': 0,'PWM': 0}
client=mqtt.Client() #Cliente a utilizar
client.username_pw_set(ACCESS_TOKEN) #Acceso al token mediante el metodo username 
client.connect(THINGSBOARD_HOST,1883,60)#Nos conectamos a traves de mqtt a nuestro host d
ser = serial.Serial('/dev/ttyUSB0',9600,timeout = 3.0) #Establecemos comunicación serial
client.loop_start()
try:
  while True:
    ser.flush()
    DataRead=str(ser.readline().strip(),encoding = 'utf-8')
    DataRead=DataRead[DataRead.find("$")+1:len(DataRead)]
    Datos=DataRead
    var1=int(DataRead[0:DataRead.find(",")])
    var2=int(DataRead[DataRead.find(",")+1:len(DataRead)])
    #print(u"RPM: {:g}, PWM: {:g}".format(var1,var2))
    var3=var1*600
    var4=var2*3.3
    switch (var4) {
        case var4<= 16:  
            GPIO.output(5,1);
            GPIO.output(6,0);
            GPIO.output(13,0);
            GPIO.output(16,0);
            GPIO.output(19,0);
            GPIO.output(20,0);
            break;
        case var4>16 and var4<=30:  
            GPIO.output(5,1);
            GPIO.output(6,1);
            GPIO.output(13,0);
            GPIO.output(16,0);
            GPIO.output(19,0);
            GPIO.output(20,0);
            break;
        }
    print(u"RPM: {:g}, PWM: {:g}".format(var3,var4))
    datos_sensor['RPM']=var3
    datos_sensor['PWM']=var4
    client.publish("v1/devices/me/telemetry",json.dumps(datos_sensor),1) #publica datos en label RPM y PWM

except keyboardInterrupt: 
  pass
client.loop_stop()
client.disconnect()