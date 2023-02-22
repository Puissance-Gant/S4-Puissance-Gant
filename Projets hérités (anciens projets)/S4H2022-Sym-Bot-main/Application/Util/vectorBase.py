import numpy as np
import math
from enum import Enum
from Cinematic.vectorBase import *



class VectorSpaceAxis(Enum):
    X = 0
    Y = 1
    Z = 2

def getRotationMatrixX(theta):
    return np.array([[1, 0, 0]
                            , [0, math.cos(theta), -math.sin(theta)]
                            , [0, math.sin(theta), math.cos(theta)]])

def getRotationMatrixY(theta):
    return np.array([[math.cos(theta), 0, math.sin(theta)]
                            , [0, 1, 0]
                            , [-math.sin(theta), 0, math.cos(theta)]])

def getRotationMatrixZ(theta):
    return np.array([[math.cos(theta), -math.sin(theta), 0]
                                , [math.sin(theta), math.cos(theta), 0]
                                , [0, 0, 1]])

def eulerAngleToRotationMatrix(rotx,roty, rotz):
    return getRotationMatrixX(rotx).dot(getRotationMatrixY(roty).dot(getRotationMatrixZ(rotz)))

def translateX(vector, translationValue):
    return np.array([vector[0]+translationValue, vector[1], vector[2]]).reshape(3,1)

def translateY(vector, translationValue):
    return np.array([vector[0], vector[1]+translationValue, vector[2]]).reshape(3,1)

def translateZ(vector, translationValue):
    return np.array([vector[0], vector[1], vector[2]+translationValue]).reshape(3,1)

class VectorBase:

    def __init__(self,x=0,y=0,z=0, thetax=0, thetay=0, thetaz=0):

        self.xAxis = np.array([1,0,0]).reshape(3,1)
        self.yAxis = np.array([0, 1, 0]).reshape(3, 1)
        self.zAxis = np.array([0, 0, 1]).reshape(3, 1)
        self.thetax = thetax
        self.thetay = thetay
        self.thetaz = thetaz

    def rotateX(self, theta):
        RotMatrix = getRotationMatrixX(theta)
        self.zAxis = RotMatrix.dot(self.zAxis)
        self.yAxis = RotMatrix.dot(self.yAxis)


    def rotateY(self, theta):
        RotMatrix = getRotationMatrixY(theta)
        self.xAxis = RotMatrix.dot(self.xAxis)
        self.zAxis = RotMatrix.dot(self.zAxis)


    def rotateZ(self, theta):
        RotMatrix = getRotationMatrixZ(theta)
        self.xAxis = RotMatrix.dot(self.xAxis)
        self.yAxis = RotMatrix.dot(self.yAxis)

    def getXaxis(self):
        return self.xAxis

    def getYaxis(self):
        return self.yAxis

    def getZaxis(self):
        return self.zAxis


