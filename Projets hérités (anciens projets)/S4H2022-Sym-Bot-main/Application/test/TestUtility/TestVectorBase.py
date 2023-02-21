import unittest
import math
from Cinematic.vectorBase import *
from test.TestCinematic.VectorTester import *


class testVectorBase(VectorTester):

    def test_rotateX(self):
        base = VectorBase(0, 1, 0)
        base.rotateX(math.pi/2)

        self.assertVectorEqual([1.0, 0.0, 0], base.getXaxis())
        self.assertVectorEqual([0.0, 0.0, 1.0], base.getYaxis())
        self.assertVectorEqual([0.0, -1.0, 0], base.getZaxis())

    def test_rotateY(self):
        base = VectorBase(0, 0, 1)
        base.rotateY(math.pi / 2)

        self.assertVectorEqual([0.0, 0.0, -1.0], base.getXaxis())
        self.assertVectorEqual([0.0, 1.0, 0], base.getYaxis())
        self.assertVectorEqual([1.0, 0, 0], base.getZaxis())

    def test_rotateZ(self):
        base = VectorBase(1, 0, 0)
        base.rotateZ(math.pi / 2)

        self.assertVectorEqual([0.0, 1.0, 0.0], base.getXaxis())
        self.assertVectorEqual([-1.0, 0.0, 0], base.getYaxis())
        self.assertVectorEqual([0.0, 0.0, 1.0], base.getZaxis())

