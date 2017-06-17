import unittest
from FEM import *


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


        contribuicoes = elem.rigidezContribuicao()

        self.assertEqual(len(contribuicoes), 16)

        self.assertEqual(contribuicoes[0][0], (4, 4))
        self.assertEqual(contribuicoes[1][0], (4, 5))
        self.assertEqual(contribuicoes[2][0], (4, 8))
        self.assertEqual(contribuicoes[3][0], (4, 7))


        D = elem.coordMatriz()
        self.assertEqual(D[(0,0)], 0.50); self.assertEqual(D[(0,1)], 0.50); 
        self.assertEqual(D[(1,0)], 0.75); self.assertEqual(D[(1,1)], 0.50);
        self.assertEqual(D[(2,0)], 0.75); self.assertEqual(D[(2,1)], 0.75);
        self.assertEqual(D[(3,0)], 0.50); self.assertEqual(D[(3,1)], 0.75);

        print elem.calcJac(-1,-1)


        print D

        matriz = fem.calcMatrizRigidez()

        print matriz


if __name__ == '__main__':
    unittest.main()