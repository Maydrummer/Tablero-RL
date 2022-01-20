#!/usr/bin/env python3  
# -*- coding: utf-8 -*-
#----------------------------------------------------------------------------
# Created By  : Anthony Maisincho, Ulbio Alejandro 
# Created Date: 20/1/2022
# ---------------------------------------------------------------------------
""" Este código de python se encarga de recibir por comunicación serial los
datos sensado por el arduino, a su vez, permite publicarlos a la nube por el 
protocolo MQTT y tambien en función de la variable PWM acciona distintos GPIO,
el objetivo es comprobar el funcionamiento total del tablero e incentivar a los
estudiantes a que realicen sus propias aplicaciones."""  
# ---------------------------------------------------------------------------
# Imports 
# ---------------------------------------------------------------------------

import RPi.GPIO as GPIO #Importamos la libreria GPIO 
import os #
import sys
import paho.mqtt.client as mqtt #Librería cliente para MQTT
import json #Para tratar archivos Json
import time
import serial
import requests
GPIO.setmode(GPIO.BCM) #El modo BCM se basa en el nombre de los GPIO 
gpio_out=[5,6,13,16,19,20] #Lista de GPIO a utilizar con el módulo de reles
for gpio in gpio_out:
    GPIO.setup(gpio,GPIO.OUT)   #Seteamos como salida dichos GPIO
THINGSBOARD_HOST="mqtt.thingsboard.cloud" #Declaramos el host del servidor MQTT
ACCESS_TOKEN="hola123" #Guardamos el Token
datos_sensor={'RPM': 0,'PWM': 0} #Creamos un diccionario con los tópicos a usar
client=mqtt.Client() #Cliente a utilizar
client.username_pw_set(ACCESS_TOKEN) #Acceso al token mediante el metodo username 
client.connect(THINGSBOARD_HOST,1883,60)#Nos conectamos a traves de mqtt a nuestro host d
ser = serial.Serial('/dev/ttyUSB0',9600,timeout = 3.0) #Establecemos comunicación serial con arduino
client.loop_start() #Iniciamos la publicación del cliente en el broker MQTT
try:
  while True:
    ser.flush() #Limpiamos el buffer de la comunicación serial
    DataRead=str(ser.readline().strip(),encoding = 'utf-8') #Leemos la línea enviada por el arduino
    DataRead=DataRead[DataRead.find("$")+1:len(DataRead)] #Extraemos el dato encapsulado
    Datos=DataRead
    var1=int(DataRead[0:DataRead.find(",")])
    var2=int(DataRead[DataRead.find(",")+1:len(DataRead)])
    #print(u"RPM: {:g}, PWM: {:g}".format(var1,var2))
    var3=var1*600 #Convertimos las vueltas a RPM
    var4=var2*3.3 #Escalamos la variable PWM 
    client.publish("v1/devices/me/telemetry",json.dumps(datos_sensor),1) #publica datos en label RPM y PWM
    if(var4<=16):  #Verifica el rango de la variable PWM y en funcion de su valor activa los GPIO
      GPIO.output(5,0);
      GPIO.output(6,1);
      GPIO.output(13,1);
      GPIO.output(16,1);
      GPIO.output(19,1);
      GPIO.output(20,1);
    elif(var4>16 and var4<=30):
      GPIO.output(5,0);
      GPIO.output(6,0);
      GPIO.output(13,1);
      GPIO.output(16,1);
      GPIO.output(19,1);
      GPIO.output(20,1);
    elif(var4>30 and var4<=45):
      GPIO.output(5,0);
      GPIO.output(6,0);
      GPIO.output(13,0);
      GPIO.output(16,1);
      GPIO.output(19,1);
      GPIO.output(20,1);
    elif(var4>45 and var4<=60):
      GPIO.output(5,0);
      GPIO.output(6,0);
      GPIO.output(13,0);
      GPIO.output(16,0);
      GPIO.output(19,1);
      GPIO.output(20,1);
    elif(var4>60 and var4<=75):
      GPIO.output(5,0);
      GPIO.output(6,0);
      GPIO.output(13,0);
      GPIO.output(16,0);
      GPIO.output(19,0);
      GPIO.output(20,1);
    elif(var4>75 and var4<=100):
      GPIO.output(5,0);
      GPIO.output(6,0);
      GPIO.output(13,0);
      GPIO.output(16,0);
      GPIO.output(19,0);
      GPIO.output(20,0);
except keyboardInterrupt: 
  GPIO.cleanup()
  pass
client.loop_stop()
client.disconnect()
