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

constant_Calib_Acel = (16384/9.81)
constant_Calib_Gyro = 131

os.system('clear')
print ("======= List of USB devices =======")
serial_ports = glob.glob('/dev/ttyUSB*')
while(len(serial_ports)==0):
    print ("Connect the Arduino...\n")
    sleep(5);
    serial_ports = glob.glob('/dev/ttyUSB*')
        
for i in range(len(serial_ports)): 
    print (i, " - ", serial_ports[i])
    port = input("Choose the arduino port (e.g. 0): ")
        
ser = serial.Serial(serial_ports[int(port)], 115200)

def main():
    global contador
    #ser.open()
    if ser.is_open:
        print("Established serial communication\n")
    else :
        print("Serial communication error\n")
    
   # ax.clear()
    input("Pressione 'Enter' para iniciar a leitura e \"Ctrl+C\" para pausar: \n")
    
    
    
    while(True):    
        try:
            line = ser.readline().decode("utf-8")
            sleep(0.0001)
            print(line)
            try:
                entry = line.split("\t")
                AcX = np.float(entry[0])
                AcY = np.float(entry[1])
                AcZ = np.float(entry[2])
                temp = np.float(entry[3])
                Gx = np.float(entry[4])
                Gy = np.float(entry[5])
                Gz = np.float(entry[6])
                
                #print(AcX, AcY, AcZ, temp, Gx, Gy, Gz)
                
                ACX = AcX/constant_Calib_Acel -9.81 # entre--20 m/s² e + 20 m/s² 
                ACY = AcY/constant_Calib_Acel -9.81 # entre--20 m/s² e + 20 m/s² 
                ACZ = AcZ/constant_Calib_Acel -9.81 # entre--20 m/s² e + 20 m/s² 
                
                GX = Gx/constant_Calib_Gyro # entre +250º/s e -250º/s
                GY = Gy/constant_Calib_Gyro # entre +250º/s e -250º/s
                GZ = Gz/constant_Calib_Gyro # entre +250º/s e -250º/s
                
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
                
               # print(ACX,ACY,ACZ,GX,GY,GZ)
                
                
            except (ValueError):
                print("Erro de valor.")
                pass
        
        except (KeyboardInterrupt):
            now = datetime.now()
            print ("Voce pressionou Ctrl+C para interromper este programa! Seus dados foram salvos em 'Dados_%s.csv'"%(str(now)[:-7]))
            ser.close()
            #plt.plot(acelX, '-r')
            plt.plot(gyroX)
            plt.show()
           # x = np.vstack((read1,read2,read3,read4,read5,read6,read7))
           # np.savetxt('Dados_%s.csv'%(str(now)[:-7]), np.transpose(x), delimiter=';')  
            break
if __name__ == "__main__":
    main()
    