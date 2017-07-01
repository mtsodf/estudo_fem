from numpy import *
import pylab as plt
#import matplotlib.axes3d as p3
import mpl_toolkits.mplot3d.axes3d as p3
import os
import sys
from shutil import copyfile


def ler_nos(filename):
	nos = []
	f=open(filename)
	n = int(f.next())
	for i in range(n):
		l = f.next()
		nos.append([float(x) for x in l.split()[:3]])



def ler_elementos(filename):
	nos = []
	f=open(filename)
	n = int(f.next())
	for i in range(n):
		l = f.next()
		nos.append([float(x) for x in l.split()[:3]])



print "Argumentos: ", sys.argv

if len(sys.argv) > 1:
    nos = sys.argv[1]
    elementos = sys.argv[2]
    copyfile("%s"%nos, "nos.txt")
    copyfile("%s"%elementos, "elementos.txt")
    os.system("./Laplaciano %s" % sys.argv[3])

nos = ler_nos("nos.txt")
#elementos = ler_elementos("elementos.txt")


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



fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
#ax = axarr[0].Axes3D(fig)
ax.scatter3D(ravel(x),ravel(y),ravel(z))
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.savefig("exemplo%s.png" % sys.argv[3])