#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 18 10:32:24 2018

@author: eduardo-ssr
"""

import  serial # carrega a biblioteca Serial para
               # comunicações série
import numpy as np# carrega a biblioteca numpy

import matplotlib.pyplot as plt #carrega a
                                #biblioteca pyplot
import matplotlib.gridspec as gridspec #carrega a
                                       # biblioteca gridspec
from drawnow import drawnow #carrega a biblioteca
                      # drawnow
#from array import array #carrega a biblioteca
                        # array
#instanciação dos objetos
#dados obtidos através do MPU6050
AcX = np.array('f')
AcY = np.array('f')
#AcZ = np.array('f')
#Tmp = np.array('f')
#GyX = np.array('f')
#GyY = np.array('f')
#GyZ = np.array('f')

#comunicacao
arduinoData = serial.Serial('/dev/ttyUSB0', 9600) #Cria um objecto do tipo serial chamado arduinoData
plt.ion()
cnt=0

def makeFig():
 gs = gridspec.GridSpec(3, 3) #gridspec 3x3
 #Plot 1
 plt.subplot(gs[0, :])#posicao do subplot
 plt.ylim([-20000,20000])#valor min e max de y
 plt.title('Variação do Acelerômetro eixo x')#titulo
 plt.grid(True)
 plt.ylabel('Acx')#etiquetas do eixo y
 #plt.plot(tempF, 'ro-', label='temperatura em graus') #plot de temperature
 plt.plot(AcX, 'ro-', label='variação Acx') #plot de temperature
 plt.legend(loc='upper left')#plot da legenda

 
 #mainProgram
while True:
  while (arduinoData.inWaiting()==0):
    pass
  arduinoString = arduinoData.readline()
  print(arduinoString)
 # splitedArray = [float(s) for s in arduinoString.split(',')]
    
 # acelx = splitedArray[0]
 # acely = splitedArray[1]
 # AcX.append(acelx)
 # AcY.append(acely)
  
  #drawnow(makeFig)
  #plt.pause(.000005)
 # cnt=cnt+1
#  if(cnt>50):
   #AcX.pop(0)
   #AcY.pop(0)