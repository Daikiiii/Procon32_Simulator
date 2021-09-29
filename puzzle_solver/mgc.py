import math
import numpy as np

#relation D(xi,xj)

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3
dummyDiffs = np.array([[0,0,0],[1,1,1],[-1,-1,-1],[0,0,1],[0,1,0],[1,0,0],[-1,0,0],[0,-1,0],[0,0,-1]])
'''
rotation numbers of xj 
0=0 90=3 180=2 270=1
np配列やskimageの画像回転は反時計回りのため-つける
ROTATION=(0,90,180,270)
'''

def _avg_difference(npiece, side):
    if side == LEFT:
        difference = npiece[:, 0] - npiece[:, 1]
    elif side == RIGHT:
        difference = npiece[:, -1] - npiece[:, -2]
    elif side == UP:
        difference = npiece[0, :] - npiece[1, :]
    else:
        difference = npiece[-1, :] - npiece[-2, :]

    return sum(difference)/float(len(difference))


def _gradient(pieces_difference, average_side_difference):
    grad = pieces_difference - average_side_difference
    #grad_t = np.transpose(np.vstack([grad,dummyDiffs]))
    grad_t=np.transpose(grad)
    cov = np.cov(grad_t)
    try:
        cov_inv = np.linalg.inv(cov)

    except np.linalg.LinAlgError as e:
        cov_inv = np.ones((3, 3))
    #print(cov_inv.shape,grad_t.shape)
    return grad.dot(cov_inv).dot(grad_t)


def mgc(np1, np2, r):
    relation=r%4
    rotation=r//4    
    np2=np.rot90(np2,-rotation)
    if relation == LEFT:
        grad_12 = _gradient(np2[:, 0] - np1[:, -1], _avg_difference(np1, RIGHT))
        grad_21 = _gradient(np1[:, -1] - np2[:, 0], _avg_difference(np2, LEFT))

    elif relation == RIGHT:
        grad_12 = _gradient(np2[:, -1] - np1[:, 0], _avg_difference(np1, LEFT))
        grad_21 = _gradient(np1[:, 0] - np2[:, -1], _avg_difference(np2, RIGHT))

    elif relation==UP:
        grad_12 = _gradient(np2[0, :] - np1[-1, :], _avg_difference(np1, DOWN))
        grad_21 = _gradient(np1[-1, :] - np2[0, :], _avg_difference(np2, UP))
    
    else:
        grad_12 = _gradient(np2[-1, :] - np1[0, :], _avg_difference(np1, UP))
        grad_21 = _gradient(np1[0, :] - np2[-1, :], _avg_difference(np2, DOWN))

    return np.sum(grad_12 + grad_21)


def rgb(np1, np2, r):
    relation=r%4
    rotation=r//4
    np2=np.rot90(np2,-rotation)
    if relation == LEFT:
        difference = np1[:, -1] - np2[:, 0]
    elif relation == RIGHT:
        difference = np1[:, 0] - np2[:, -1]
    elif relation==UP:
        difference = np1[-1, :] - np2[0, :]
    elif relation==DOWN:
        difference = np1[0, :] - np2[-1, :]


    exponent = np.vectorize(lambda x: math.pow(x, 2))
    dissimilarity = np.sum(exponent(difference))
    #print(dissimilarity)
    return math.sqrt(dissimilarity)

'''
rgb_mgc(*args)=dissimilarity(pieces[i], pieces[j], erel)

注意!!!
return rgb(*args)*mgc(*args)だと精度が変わる
RGB値かMGC値どちらか一方か両方使うのかはこれから考える
RGB抜いたほうが精度は高い
'''
def rgb_mgc(*args):
    return rgb(*args)*mgc(*args)
    