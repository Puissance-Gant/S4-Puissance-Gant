import unittest
import math
import numpy as np


class VectorTester(unittest.TestCase):

    def assertVectorEqual(self, expectedVector, currentVector, delta=0.001):
        # self.assertAlmostEqual(expectedVector[0], currentVector[0], delta=delta)
        # self.assertAlmostEqual(expectedVector[1], currentVector[1], delta=delta)
        # self.assertAlmostEqual(expectedVector[2], currentVector[2], delta=delta)
        for i in range(len(expectedVector)):
            self.assertAlmostEqual(expectedVector[i], currentVector[i], delta=delta)


    def assertMatrixEqual(self, expectedMatrix, currentMatrix, decimal = 3):
        np.testing.assert_array_almost_equal(expectedMatrix,currentMatrix, decimal=decimal)

    def assertPoseVectorEqual(self,expectedVector, currentVector, delta=0.001):
        self.assertVectorEqual(expectedVector[0:3],currentVector[0:3])
        self.assertTrue(self.isAngleAlmostEqual(expectedVector[3],currentVector[3],delta))
        self.assertTrue(self.isAngleAlmostEqual(expectedVector[4], currentVector[4], delta))
        self.assertTrue(self.isAngleAlmostEqual(expectedVector[5], currentVector[5], delta))

    def isAngleAlmostEqual(self,expected, current,delta = 0.001):
        if expected <= delta:
            if current >= 2*math.pi -(delta-expected):
                return True

        if expected >= 2*math.pi-delta:
            if current < expected -(2*math.pi-delta):
                return True

        if math.fabs(expected-current) <= delta:
            return True
        else:
            return False
