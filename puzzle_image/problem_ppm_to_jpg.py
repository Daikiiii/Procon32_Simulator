import cv2
import matplotlib.pyplot as plt

problem_path="D:/procon32/procon_image/puzzle_image/problem.ppm"
output_path="D:/procon32/procon_image/puzzle_image/problem.jpg"

img=cv2.imread(problem_path)
cv2.imwrite(output_path,img)

plt.imshow(cv2.cvtColor(img,cv2.COLOR_BGR2RGB))
plt.show()