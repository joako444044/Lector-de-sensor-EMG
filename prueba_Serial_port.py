import serial
import time

I_potenciometer = serial.Serial("COM3",9600, timeout = 1)
while (True):
    try:
        Value_P = I_potenciometer.readline()
        if (Value_P):
            print("Este es el valor", int(Value_P))
    except Exception as E:
        print(E)
    #time.sleep(500)


