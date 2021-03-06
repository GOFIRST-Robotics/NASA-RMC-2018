import pygame
import math

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
fig = plt.figure()
ax=Axes3D(fig)
#ax=plt.axes(projection='3d')
swidth=1000
sheight=1000
screen=10
#screen=pygame.display.set_mode((swidth,sheight))
#pygame.display.flip()
color=[240,240,240]
def heightcolor(z):
    color=[250,250,250]
    if z > 1.5:
        color=[0,250,250]
    if z>2.5:
        color=[250,250,0]
    if z>3.5:
        color=[0,250,0]
    if z>4.5:
        color=[250,0,0]
    return color
def plot(screen,x,y,z,color):
    #pygame.draw.circle(screen,color, (x,y), 2,1)
    
    ax.scatter(x, y, z)

def polartocart(r,theta,z):
    x=2.0*r*math.cos(theta)# theta must be in radians
    y=2.0*r*math.sin(theta)
    coords=[x,y,z]
    return coords
print(polartocart(30.0,3.1415926/180.0,20))
toggle=0
with open('datatest1.dat','r') as f:
    for line in f:
        for word in line.split():
            num=toggle%4
            if num ==0:
                theta=(float(word)/1000.0)*math.pi/180.0
            elif num==1:
                distance=float(word)
            elif num ==2:
                strength=float(word)
            else:
                height=float(word)
                coords=polartocart(distance,theta,height)
                plot(screen, int(coords[0]+600), int(coords[1]+600),int(coords[2]),heightcolor(height))
            toggle+=1
        if toggle >1000:
            break
f.close()
fig.show()
plt.pause(5)
#pygame.display.flip()

