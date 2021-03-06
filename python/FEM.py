"""
Estudos de Elementos Finitos.
Resolucao do laplaciano em 2 dimensoes
"""

import numpy as np
import numpy.linalg as LA
from math import *


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


        m_rigidez = np.zeros((self._graus, self._graus))

        for elem in self.elementos:
            for contribuicao in elem.rigidezContribuicao():
                m_rigidez[contribuicao[0]] += contribuicao[1]

        return m_rigidez

    def calcLD(self, ld_func):
        ld = np.zeros(self._graus)

        for elem in self.elementos:
            pontos, pesos = elem.pontosIntegracao()
            for i, no in enumerate(elem.nos):
                if no.livre:
                    for nk in range(len(pontos)):
                        ep, mu = pontos[nk][0], pontos[nk][1]
                        x, y = elem.coordGlobal(ep, mu)
                        detjac = LA.det(elem.calcJac(ep, mu))
                        ld[no.matrizPos] += pesos[i]*ld_func(x,y)*elem.phis()[i](ep, mu)*abs(detjac)

        return ld
             

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

    def pontosIntegracao(self):

        x = sqrt(3)/3

        pontos = [(-x,-x), (x,-x), (x,x), (-x,x)]
        pesos = [1,1,1,1]

        return pontos, pesos

    def coordGlobal(self, ep, mu):
        x = 0
        y = 0

        for i,phi in enumerate(self.phis()):
            x += self.nos[i].x*phi(ep,mu)
            y += self.nos[i].y*phi(ep,mu)

        return x, y

    def rigidezContribuicao(self):
        contribuicoes = []


        for i, no1 in enumerate(self.nos):
            if no1.livre:
                for j, no2 in enumerate(self.nos):
                    if no2.livre:

                        integral = 0.0

                        pontos, pesos = self.pontosIntegracao()

                        for nk in range(len(pontos)):
                            ep, mu = pontos[nk]

                            devGlobal = self.calcDevGlobal(ep, mu)


                            jac = self.calcJac(ep, mu)

                            integral += pesos[nk] * (devGlobal[(0,i)]*devGlobal[(0,j)] + devGlobal[(1,i)]*devGlobal[(1,j)]) * abs(LA.det(jac))


                        contribuicoes.append([(no1.matrizPos, no2.matrizPos), integral])

        return contribuicoes

    def calcJac(self, ep, mu):
        nosCoords = self.coordMatriz()
        dev = self.calcDevLocal(ep, mu)

        return np.dot(dev, nosCoords)

    def calcDevGlobal(self, ep, mu):
        return np.dot(self.calcJacInv(ep, mu), self.calcDevLocal(ep, mu))

    def calcJacInv(self, ep, mu):
        return LA.inv(self.calcJac(ep, mu))


    def calcDevLocal(self, ep, mu):
        d = np.zeros((2, self.qtdNos()))
        dt = 0.0001
        for i, phi in enumerate(self.phis()): 
            d[(0, i)] = (phi(ep+dt, mu) - phi(ep-dt, mu)) / (2*dt)
            d[(1, i)] = (phi(ep, mu+dt) - phi(ep, mu-dt)) / (2*dt)

        return d

    def coordMatriz(self):
        nosCoords = np.zeros((self.qtdNos(), 2))
        for i, no in enumerate(self.nos):
            nosCoords[(i, 0)] = no.x
            nosCoords[(i, 1)] = no.y

        return nosCoords

    def phis(self):
        phi0 = lambda ep, mu: 0.25*(1-ep)*(1-mu)
        phi1 = lambda ep, mu: 0.25*(1+ep)*(1-mu)
        phi2 = lambda ep, mu: 0.25*(1+ep)*(1+mu)
        phi3 = lambda ep, mu: 0.25*(1-ep)*(1+mu)
        return [phi0, phi1, phi2, phi3]

    def devPhi(self):
        dev_phi0 = lambda ep, mu: 0.25*(1-ep)*(1-mu)
        dev_phi1 = lambda ep, mu: 0.25*(1+ep)*(1-mu)
        dev_phi2 = lambda ep, mu: 0.25*(1+ep)*(1+mu)
        dev_phi3 = lambda ep, mu: 0.25*(1-ep)*(1+mu)
        return [phi0, phi1, phi2, phi3]


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


def resolver(n, sol_func, ld_func):
    elementos, nos = criarMalha(n)
    fem = FemProblem(elementos, nos)
    
    matriz = fem.calcMatrizRigidez()
    ld = fem.calcLD(ld_func)

    x = LA.solve(matriz, -ld)

    livres = fem.calcGrausDeLiberdade()
    erro = 0.0
    for no in nos:
        if no.livre:
            erro += abs(x[no.matrizPos] - sol_func(no.x, no.y))


    print "erro = ", erro/livres

if __name__ == '__main__':
    sol_func = lambda x,y: sin(pi*x)*sin(pi*y)
    ld_func = lambda x,y: -2*pi*pi*sin(pi*x)*sin(pi*y)
    resolver(20, sol_func, ld_func)