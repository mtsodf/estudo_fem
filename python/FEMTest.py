import unittest
from FEM import *

class TestStringMethods(unittest.TestCase):

    def test_upper(self):
        elementos, nos = criarMalha(4)

        elem = elementos[10]

        self.assertEqual(elem.qtdNos(), 4)

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




if __name__ == '__main__':
    unittest.main()