"""
Estudos de Elementos Finitos.
Resolucao do laplaciano em 2 dimensoes
"""

import numpy


class FemProblem:

    def __init__(self, elementos, nos):
        self.elementos = elementos
        self.nos = nos

        self._graus = None


    def calcGrausDeLiberdade(self):
        if self._graus is None:
            matrizPos = 0
            for no in self.nos:
                if no.livre:
                    no.matrizPos = matrizPos
                    matrizPos+=1

            self._graus = matrizPos

        return self._graus


    def calcMatrizRigidez(self):
        self.calcGrausDeLiberdade()

        for elem in self.elementos:
            pass



class No:

    def __init__(self, g_ind, x, y, livre):
        self.g_ind= g_ind
        self.x = x
        self.y = y
        self.livre = livre
        self.matrizPos = None


    def __str__(self):
        return "No %d (%f, %f) %s" % (self.g_ind, self.x, self.y, "livre" if self.livre else "")




class Elemento():
    def __init__(self, num=-1):
        self.nos = []
        self.num = num


    def addNo(self, no):
        self.nos.append(no)

    def qtdNos(self):
        return len(self.nos)





def criarMalha(N):
    h = 1.0/N
    n = N-1

    nos = []

    num = 0

    for j in range(N+1):
        for i in range(N+1):
            no = No(num, i*h, j*h, not(i==0 or j==0 or i==N or j==N))
            nos.append(no)
            num += 1

    elementos = []

    num = 0
    for j in range(N):
        for i in range(N):
            elem = Elemento(num)
            acimaEsquerdo = j*(N+1) + i
            elem.addNo(nos[acimaEsquerdo])
            elem.addNo(nos[acimaEsquerdo+1])
            elem.addNo(nos[acimaEsquerdo+1+N+1])
            elem.addNo(nos[acimaEsquerdo + N+1])

            elementos.append(elem)
            num+=1





    return elementos, nos


if __name__ == '__main__':
    criarMalha(4)