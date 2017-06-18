import unittest
from FEM import *
from math import sin, pi
import numpy.linalg as LA

class TestStringMethods(unittest.TestCase):

    def test_upper(self):
        elementos, nos = criarMalha(4)

        for elem in elementos:
            self.assertEqual(elem.qtdNos(), 4)

        elem = elementos[10]



        self.assertEqual(elem.nos[0].g_ind, 12)
        self.assertEqual(elem.nos[1].g_ind, 13)
        self.assertEqual(elem.nos[2].g_ind, 18)
        self.assertEqual(elem.nos[3].g_ind, 17)

        self.assertTrue(nos[12] is elem.nos[0])



        fem = FemProblem(elementos, nos)

        self.assertEqual(fem.calcGrausDeLiberdade(), 9)

        self.assertTrue(nos[0].matrizPos is None)
        self.assertTrue(nos[9].matrizPos is None)
        self.assertTrue(nos[10].matrizPos is None)
        self.assertTrue(nos[24].matrizPos is None)

        self.assertEqual(nos[11].matrizPos, 3)
        self.assertEqual(nos[18].matrizPos, 8)


        x,y = elem.coordGlobal(-1,-1)
        self.assertAlmostEqual(x, 0.5, 10)
        self.assertAlmostEqual(y, 0.5, 10)

        x,y = elem.coordGlobal(1,-1)
        self.assertAlmostEqual(x, 0.75, 10)
        self.assertAlmostEqual(y, 0.5, 10)

        x,y = elem.coordGlobal(1,1)
        self.assertAlmostEqual(x, 0.75, 10)
        self.assertAlmostEqual(y, 0.75, 10)

        x,y = elem.coordGlobal(-1,1)
        self.assertAlmostEqual(x, 0.5, 10)
        self.assertAlmostEqual(y, 0.75, 10)



        contribuicoes = elem.rigidezContribuicao()

        self.assertEqual(len(contribuicoes), 16)

        self.assertEqual(contribuicoes[0][0], (4, 4))
        self.assertEqual(contribuicoes[1][0], (4, 5))
        self.assertEqual(contribuicoes[2][0], (4, 8))
        self.assertEqual(contribuicoes[3][0], (4, 7))

        self.assertEqual(contribuicoes[4][0], (5, 4))
        self.assertEqual(contribuicoes[5][0], (5, 5))
        self.assertEqual(contribuicoes[6][0], (5, 8))
        self.assertEqual(contribuicoes[7][0], (5, 7))

        self.assertEqual(contribuicoes[ 8][0], (8, 4))
        self.assertEqual(contribuicoes[ 9][0], (8, 5))
        self.assertEqual(contribuicoes[10][0], (8, 8))
        self.assertEqual(contribuicoes[11][0], (8, 7))

        self.assertEqual(contribuicoes[12][0], (7, 4))
        self.assertEqual(contribuicoes[13][0], (7, 5))
        self.assertEqual(contribuicoes[14][0], (7, 8))
        self.assertEqual(contribuicoes[15][0], (7, 7))


        self.assertAlmostEqual(contribuicoes[0][1], 2.0/3, 10)
        self.assertAlmostEqual(contribuicoes[2][1], -1.0/3, 10)


        D = elem.coordMatriz()
        self.assertEqual(D[(0,0)], 0.50); self.assertEqual(D[(0,1)], 0.50); 
        self.assertEqual(D[(1,0)], 0.75); self.assertEqual(D[(1,1)], 0.50);
        self.assertEqual(D[(2,0)], 0.75); self.assertEqual(D[(2,1)], 0.75);
        self.assertEqual(D[(3,0)], 0.50); self.assertEqual(D[(3,1)], 0.75);

        print elem.calcJac(-1,-1)
        print elem.calcJacInv(-1,-1)


        print D

        matriz = fem.calcMatrizRigidez()

        print matriz

        ld_func = lambda x,y: -2*pi*pi*sin(pi*x)*sin(pi*y)
        
        ld = fem.calcLD(ld_func)
        print ld

        print LA.solve(matriz, -ld)


if __name__ == '__main__':
    unittest.main()