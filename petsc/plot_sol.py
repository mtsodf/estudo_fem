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
    nos_livres = []
    nos_livres.append([])
    nos_livres.append([])
    nos_dirichlet = []
    nos_dirichlet.append([])
    nos_dirichlet.append([])
    nos_neumann = []
    nos_neumann.append([])
    nos_neumann.append([])
    f=open(filename)
    n = int(f.next())
    for i in range(n):
        split = f.next().split()
        x,y = [float(x) for x in split[:2]]
        tipo = int(split[2])
        nos[0].append(x)
        nos[1].append(y)

        if(tipo == 0):
            nos_livres[0].append(x)
            nos_livres[1].append(y)
        elif(tipo == 1):
            nos_dirichlet[0].append(x)
            nos_dirichlet[1].append(y)
        elif(tipo == 2):
            nos_neumann[0].append(x)
            nos_neumann[1].append(y)


    return nos, nos_livres, nos_dirichlet, nos_neumann

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
    chamada = "./Laplaciano %s" % sys.argv[3]
    print chamada
    os.system(chamada)

nos, nos_livres, nos_dirichlet, nos_neumann = ler_nos("nos.txt")
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

for elem in elementos:
    
    xs = []
    ys = []
    
    for i in range(len(elem)):
        xs.append(nos[0][elem[i]])
        ys.append(nos[1][elem[i]])
    
    xs.append(nos[0][elem[0]])
    ys.append(nos[1][elem[0]])
    
    ax.plot(xs, ys, 'k-', linewidth=.5)

ax.scatter(nos_livres[0], nos_livres[1], s=10, c="b")
ax.scatter(nos_dirichlet[0], nos_dirichlet[1], s=10, c="r")
ax.scatter(nos_neumann[0], nos_neumann[1], s=10, c="y")


plt.savefig("exemplo%s.png" % sys.argv[3])
