#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jun 29 09:31:48 2018

@author: eduardo-ssr
"""
import sys
import serial
import glob
import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec 
from drawnow import drawnow
from time import sleep
from datetime import datetime
import timeit
from scipy import signal

%matplotlib inline

read0 = []
read1 = []
read2 = []
read3 = []
read4 = []
read5 = []
read6 = []
read7 = []

acelX = []
acelY = []
acelZ = []
gyroX = []
gyroY = []
gyroZ = []

fx = []
#constant_Calib_Acel = (16384/9.81)
constant_Calib_Acel = (16384/9.81)
constant_Calib_Gyro = 131


os.system('clear')
print ("======= Lista de dispositivos USB =======")
serial_ports = glob.glob('/dev/ttyUSB*')
while(len(serial_ports)==0):
    print ("Conecte o Arduino...\n")
    sleep(5);
    serial_ports = glob.glob('/dev/ttyUSB*')
        
for i in range(len(serial_ports)): 
    print (i, " - ", serial_ports[i])
    port = input("Escolha a porta do Arduino (e.g. 0): ")
        
ser = serial.Serial(serial_ports[int(port)], 2000000,timeout=1)

def main():
    global contador
    global t
    global constant_Calib_Acel
    global constant_Calib_Gyro
    
    if ser.is_open:
        print("Comunicacao Serial Estabelecida.\n")
    else :
        print("Erro na Comunicacao Serial.\n")
        
        
    input("Pressione 'Enter' para iniciar a leitura e \"I\" duas vezes para pausar: \n")
    
    
    i = 0
    inicio = timeit.default_timer()

    while(True):    
        try:
            
            line = ser.readline().decode("utf-8")
            print(line)
            
            try:
                entry = line.split("\t")
                if len(entry) != 8 :
                    print('Erro de valor.')
                else:    
                    n = np.int(entry[0])
                    AcX = np.float(entry[1])
                    AcY = np.float(entry[2])
                    AcZ = np.float(entry[3])
                    temp = np.float(entry[4])
                    Gx = np.float(entry[5])
                    Gy = np.float(entry[6])
                    Gz = np.float(entry[7])

                    ACX = AcX/constant_Calib_Acel  # entre--20 m/s2 e + 20 m/s2 
                    ACY = AcY/constant_Calib_Acel  # entre--20 m/s2 e + 20 m/s2 
                    ACZ = AcZ/constant_Calib_Acel  # entre--20 m/s2 e + 20 m/s2

                    Temp = temp/340.00 + 36.53

                    GX = Gx/constant_Calib_Gyro # entre +250deg/s e -250deg/s
                    GY = Gy/constant_Calib_Gyro # entre +250deg/s e -250deg/s
                    GZ = Gz/constant_Calib_Gyro # entre +250deg/s e -250deg/s
                    
                    read0.append(n)
                    read1.append(AcX) 
                    read2.append(AcY)
                    read3.append(AcZ)
                    read4.append(Gx)
                    read5.append(Gy)
                    read6.append(Gz)
                    read7.append(temp)

                    acelX.append(ACX)
                    acelY.append(ACY)
                    acelZ.append(ACZ)
                    gyroX.append(GX)
                    gyroY.append(GY)
                    gyroZ.append(GZ)
                    
                    fx.append(i) 
                    i = i+1
                    
            except (ValueError):
                print("Erro de valor.")
                pass
        
        except (KeyboardInterrupt):
            fim = timeit.default_timer()
            tempo = fim - inicio
            now = datetime.now()
            
            print('Tempo medio de', i ,'execucoes foi de ',tempo/i,' s', '\n')
            print('O tempo total foi:',tempo)
            
            print ("Voce interrompeu este programa.\n")

            ser.close()
            
            
            x = np.vstack((read0, read1,read2,read3,read4,read5,read6,read7))
            y = np.vstack((read0, acelX,acelY,acelZ,gyroX,gyroY,gyroZ))
            
            r = input('Deseja salvar os dados adquiridos (S/N)? ')
            if r == 'S' or r == 's':
            
                np.savetxt('Dados_Brutos_%s.csv'%(str(now)[:-7]), np.transpose(x), delimiter=';')
                np.savetxt('Dados_Fisicos_%s.csv'%(str(now)[:-7]), np.transpose(y), delimiter=';') 
                print ('Seus dados foram salvos como: Dados_Fisicos_%s.csv'%(str(now)[:-7]))
            
            
            break
        
            
if __name__ == "__main__":
    main()