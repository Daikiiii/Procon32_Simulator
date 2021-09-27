import cv2
from PIL import Image
import matplotlib.pyplot as plt

problem_path="D:/Python_Script/puzzle/problem2.ppm"
output_path="D:/Python_Script/puzzle/problem2.png"

im=Image.open(problem_path)
im.show()
im.save(output_path)

'''
img=cv2.imread(problem_path)
img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
cv2.imwrite(output_path,img)

plt.imshow(img)
plt.show()
'''