from PIL import Image

problem_path="D:/Python_Script/puzzle/problem.ppm"
output_path="D:/Python_Script/puzzle/problem.png"

im=Image.open(problem_path)
im.show()
im.save(output_path)
