from numpy import *
import pylab as plt
#import matplotlib.axes3d as p3
import mpl_toolkits.mplot3d.axes3d as p3
import os
import sys
from shutil import copyfile


def ler_nos(filename):
	nos = []
	nos.append([])
	nos.append([])
	f=open(filename)
	n = int(f.next())
	for i in range(n):
		l = f.next()
		x,y = [float(x) for x in l.split()[:2]]

		nos[0].append(x)
		nos[1].append(y)


	return nos

def ler_elementos(filename):
	elementos = []
	f=open(filename)
	n = int(f.next())
	for i in range(n):
		l = f.next()
		elementos.append([int(x) for x in l.split()])


	return elementos

print "Argumentos: ", sys.argv

if len(sys.argv) > 1:
    nos = sys.argv[1]
    elementos = sys.argv[2]
    copyfile("%s"%nos, "nos.txt")
    copyfile("%s"%elementos, "elementos.txt")
    os.system("./Laplaciano %s" % sys.argv[3])

nos = ler_nos("nos.txt")
elementos = ler_elementos("elementos.txt")


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



fig = plt.figure(figsize=(30,15))
ax = fig.add_subplot(121, projection='3d')
#ax = axarr[0].Axes3D(fig)
ax.scatter3D(ravel(x),ravel(y),ravel(z))
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax = fig.add_subplot(122)

ax.scatter(nos[0], nos[1],s=10)

for elem in elementos:
	
	xs = []
	ys = []
	
	for i in range(len(elem)):
		xs.append(nos[0][elem[i]])
		ys.append(nos[1][elem[i]])
	
	xs.append(nos[0][elem[0]])
	ys.append(nos[1][elem[0]])
	
	ax.plot(xs, ys, 'k-')


plt.savefig("exemplo%s.png" % sys.argv[3])