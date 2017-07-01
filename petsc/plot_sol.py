from numpy import *
import pylab as p
#import matplotlib.axes3d as p3
import mpl_toolkits.mplot3d.axes3d as p3
import os
import sys
from shutil import copyfile

print "Argumentos: ", sys.argv

if len(sys.argv) > 1:
    nos = sys.argv[1]
    elementos = sys.argv[2]
    copyfile("%s"%nos, "nos.txt")
    copyfile("%s"%elementos, "elementos.txt")
    os.system("./Laplaciano")

f = open("saida.txt")

x = []
y = []
z = []

for l in f:
    xv,yv,zv = [float(i) for i in l.split()]
    x.append(xv)
    y.append(yv)
    z.append(zv)

x = array(x)
y = array(y)
z = array(z)



fig=p.figure()
ax = p3.Axes3D(fig)
ax.scatter3D(ravel(x),ravel(y),ravel(z))
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
p.savefig("%s.png" % sys.argv[3])