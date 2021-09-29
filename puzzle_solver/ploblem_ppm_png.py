from PIL import Image

problem_path="C:/Procon32_Simulator/puzzle_solver/puzzle/problem.ppm"
output_path="C:/Procon32_Simulator/puzzle_solver/puzzle/problem.png"

im=Image.open(problem_path)
im.show()
im.save(output_path)
