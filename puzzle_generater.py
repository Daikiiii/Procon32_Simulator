import numpy as np
import cv2
from skimage import io
import matplotlib.pyplot as plt

import os

def puzzle_generater(img_path,puzzle_height,puzzle_width,suffle_key):
    
    image=cv2.cvtColor(cv2.imread(img_path), cv2.COLOR_BGR2RGB)
    puzzle_size=image.shape[0]//puzzle_height

    pieces = [image[puzzle_size * i : puzzle_size * (i + 1)
                    ,puzzle_size * j : puzzle_size * (j + 1)] 
                    for i in range(puzzle_height) for j in range(puzzle_width)]
    if suffle_key:
        np.random.shuffle(pieces)
        for i,piece in enumerate(pieces):
            if i==0:
                pass
            else:
                pieces[i]=np.rot90(piece,np.random.randint(0, 4))
    
    return pieces
    
'''
if __name__== '__main__':
    input_path="D:/Python_Script/puzzle/puzzle1.png"
    output_path="D:/Python_Script/test_img/"
    puzzle_height=4
    puzzle_width=10
    image=cv2.cvtColor(cv2.imread(input_path),cv2.COLOR_BGR2RGB)
    pieces=puzzle_generater(image,puzzle_height,puzzle_width)

    for i, piece in enumerate(pieces):
        #piece=Image.fromarray(piece)
        piece_path = os.path.join(output_path,'%i.png' % (i,))
        io.imsave(piece_path,piece)
'''