import serial
import time
import pygame

pygame.display.init()
pygame.joystick.init()

if (pygame.joystick.get_count() != 1):
    print("No joystick connected or too many")
js=pygame.joystick.Joystick(0)
js.init()
print(js.get_name())
x=0

#Axis 0: d-pad horizontal
#Axis 1: d-pad vertical
#Axis 2: left trigger
#Axis 3: right stick horizontal
#Axis 4: right stick vertical
#Axis 5: right trigger
#Hat 0: left stick
while True:
    pygame.event.pump()
    ser=serial.Serial('/dev/ttyACM1',9600)#sometimes 0 or 1
    
    left = js.get_axis(1)
    right = js.get_axis(4)
    left=left*1000.0
    right=right*1000.0
    left=int(left)
    right=int(right)
    left=left/1000.0
    right=right/1000.0

    

    data=str(left)+str(right)
    #print(data)
    data=data.encode('utf-8')
    ser.write(data)

    print(ser.read().decode())
    
    '''
    x+=1
    if (x>100):
        break
'''

'''
ser=serial.Serial("/dev/ttyACM0",9600)  #change ACM number as found from ls /dev/tty/ACM*
ser.baudrate=9600

data='hello'.encode('utf-8')
ser.write(data)
'''
