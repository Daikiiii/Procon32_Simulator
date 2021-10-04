from itertools import product
from puzzle_generater import puzzle_generater
from edges_match import __compute_weights,__initial_positions,__place_pieces,TrimPuzzle,__fill_unused,__paste_pieces
from helpers import make_solution,take_problem_inf
import time

img_path="C:/Procon32_Simulator/puzzle_solver/puzzle/puzzle1.png"
save_path="C:/Procon32_Simulator/puzzle_solver/puzzle_text/"
puzzle_inf=take_problem_inf("C:/Procon32_Simulator/puzzle_solver/puzzle/problem.ppm")
assembled_width,assembled_height=puzzle_inf[:2]
pieces_count=assembled_height*assembled_width
pieces_range=range(pieces_count)
relations=range(16)
suffle_key=0
npieces=puzzle_generater(img_path,assembled_height,assembled_width,suffle_key)

start=time.time()
edges = [(i, j, rel)for (i, j, rel) 
        in product(pieces_range, pieces_range, relations)
        if i != j]

weights=__compute_weights(npieces,pieces_count,edges)
edges.sort(key=lambda x: weights[x])
weight_time=time.time()-start
print("weight_time:{0}".format(weight_time)+"[sec]")

cluster,positions,rotations = __initial_positions(npieces,pieces_count,assembled_width,assembled_height,edges)

rotation_place = __place_pieces(positions,rotations,16,16)
Block,RotBlock=TrimPuzzle(cluster,rotation_place,assembled_height,assembled_width,0)
print(Block)
print(RotBlock)
unused_pieces = set(pieces_range) - set(Block.flatten())

piece_placement,rot_placement = __fill_unused(Block,RotBlock,unused_pieces,assembled_height,assembled_width,weights)
print(piece_placement)
    
solution=__paste_pieces(npieces, piece_placement,rot_placement,assembled_width,assembled_height)
solution.save("C:/Procon32_Simulator/puzzle_solver/puzzle/solution.png")

make_solution(save_path,piece_placement,rot_placement,puzzle_inf)
elapsed_time=time.time()-start
print("実行時間:cost計測~パズル組み立て~解答保存")
print("elapsed_time:{0}".format(elapsed_time)+"[sec]")
print("assemble_time:{0}".format(elapsed_time-weight_time)+"[sec]")
solution.show()