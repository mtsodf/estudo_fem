
nos = open("nos.txt", "w")

n = 25
n2 = 9
nos.write("%d\n" % 576)

x_inic = -1.0
y_inic = -1.0
h = 2.0/(n-1)

indice = {}
cont = 0
tipo = 0
contorno = 0.0
for j in range(9):
    for i in range(n):
        x, y = x_inic+i*h, y_inic+j*h
        if i == 0 or i == n - 1 or j == 0 or j == n - 1:
            tipo = 1
            contorno = x/(x*x+y*y)
        elif(i>=8 and i<=16 and j>=8 and j<=16):
            tipo = 1
            contorno = x/(x*x+y*y)
        else:
            tipo = 0
            contorno = 0.0
        nos.write("%10.8f %10.8f %d %f\n"%(x,y, tipo, contorno))
        indice[x,y] = cont
        cont+=1

for j in range(9, 16):
    for i in range(n2):
        x, y = x_inic+i*h, y_inic+j*h
        if i == 0 or i == n - 1 or j == 0 or j == n - 1:
            tipo = 1
            contorno = x/(x*x+y*y)
        elif(i>=8 and i<=16 and j>=8 and j<=16):
            tipo = 1
            contorno = x/(x*x+y*y)
        else:
            tipo = 0
            contorno = 0.0
        nos.write("%10.8f %10.8f %d %f\n"%(x,y, tipo, contorno))
        indice[x,y] = cont
        cont+=1


for j in range(9, 16):
    for i in range(16, n):
        x, y = x_inic+i*h, y_inic+j*h
        if i == 0 or i == n - 1 or j == 0 or j == n - 1:
            tipo = 1
            contorno = x/(x*x+y*y)
        elif(i>=8 and i<=16 and j>=8 and j<=16):
            tipo = 1
            contorno = x/(x*x+y*y)
        else:
            tipo = 0
            contorno = 0.0
        nos.write("%10.8f %10.8f %d %f\n"%(x,y, tipo, contorno))
        indice[x,y] = cont
        cont+=1


for j in range(16, 25):
    for i in range(n):
        x, y = x_inic+i*h, y_inic+j*h
        if i == 0 or i == n - 1 or j == 0 or j == n - 1:
            tipo = 1
            contorno = x/(x*x+y*y)
        elif(i>=8 and i<=16 and j>=8 and j<=16):
            tipo = 1
            contorno = x/(x*x+y*y)
        else:
            tipo = 0
            contorno = 0.0
        nos.write("%10.8f %10.8f %d %f\n"%(x,y, tipo, contorno))
        indice[x,y] = cont
        cont+=1


nos.close()

elementos = open("elementos.txt", "w")

#elementos.write("%d\n"%(2*8*(n-1) + 2*8*(n2-1)))

elementos.write("%d\n"%(1024))

for j in range(n-1):
    for i in range(n-1):
        if(i>=8 and i<16 and j>=8 and j<16):
            continue
        x0,y0=x_inic+i*h, y_inic+j*h
        x1,y1=x_inic+(i+1)*h, y_inic+(j+1)*h

        p1 = (x0,y0); p2=(x1,y0); p3=(x1,y1)

        if p1 in indice and p2 in indice and p3 in indice:
            ind1=indice[p1]; ind2=indice[p2]; ind3=indice[p3]
            elementos.write("%d %d %d\n"%(ind1,ind2,ind3))

        p1 = (x0,y0); p2=(x1,y1); p3=(x0,y1)

        if p1 in indice and p2 in indice and p3 in indice:
            ind1=indice[p1]; ind2=indice[p2]; ind3=indice[p3]
            elementos.write("%d %d %d\n"%(ind1,ind2,ind3))

#inic = 0
#for j in range(8):
#   for i in range(inic, inic + (n-1)):
#       elementos.write("%d %d %d\n"%(i,i+1,i+n))
#       elementos.write("%d %d %d\n"%(i+1,i+1+n,i+n))
#   inic += n
#
#for j in range(8):
#   for i in range(inic, inic + n2-1):
#       elementos.write("%d %d %d\n"%(i,i+1,i+n2))
#       elementos.write("%d %d %d\n"%(i+1,i+1+n2,i+n2))
#   inic += n2

elementos.close()