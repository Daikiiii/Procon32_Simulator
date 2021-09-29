import numpy as np
import cv2


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
if __name__=='__main__':
    img_path="D:/Python_Script/puzzle/puzzle1.jpg"
    pieces=puzzle_generater(img_path,3,4,1)
    img=np.vstack(( np.hstack((pieces[0:4])),
                    np.hstack((pieces[4:8])),
                    np.hstack((pieces[8:12]))))
    print(img.shape)
    cv2.imwrite("D:/Python_Script/puzzle/puzzle4.jpg",cv2.cvtColor(img,cv2.COLOR_BGR2RGB))    
'''