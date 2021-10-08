from itertools import product
from puzzle_generater import puzzle_generater
from edges_match import __compute_weights,__initial_positions,__place_pieces,TrimPuzzle,__fill_unused,__paste_pieces
from helpers import make_solution,take_problem_inf,problem_ppm_png,make_dummy
import time

img_path="D:/Python_Script/puzzle/problem.png"
save_path="D:/Python_Script/puzzle_text/"
puzzle_inf=take_problem_inf("D:/Python_Script/puzzle/problem.ppm")
assembled_width,assembled_height=puzzle_inf[:2]
print("分割数=横×縦")
print(assembled_width,assembled_height)
pieces_count=assembled_height*assembled_width
pieces_range=range(pieces_count)
relations=range(16)
suffle_key=0
problem_ppm_png("D:/Python_Script/puzzle/problem.ppm",img_path)
npieces=puzzle_generater(img_path,assembled_height,assembled_width,suffle_key)

start=time.time()
edges = [(i, j, rel)for (i, j, rel) 
        in product(pieces_range, pieces_range, relations)
        if i != j]

weights=__compute_weights(npieces,pieces_count,edges)
edges.sort(key=lambda x: weights[x])
weight_time=time.time()-start
print("weight_time:{0}".format(weight_time)+"[sec]")

cluster,positions,rotations,img = __initial_positions(npieces,pieces_count,assembled_width,assembled_height,edges)
''' '''
height=20
width=20
rotation_place = __place_pieces(positions,rotations,width,height,cluster[0,0])
print("パズル組み立て")
print(cluster)
print(rotation_place)
Block,RotBlock=TrimPuzzle(cluster,rotation_place,assembled_height,assembled_width,0)
print("トリミング")
print(Block)
print(RotBlock)
unused_pieces = set(pieces_range) - set(Block.flatten())

piece_placement,rot_placement = __fill_unused(Block,RotBlock,unused_pieces,assembled_height,assembled_width,weights)
print("穴埋め")
print(piece_placement)
print(rot_placement)
solution=__paste_pieces(npieces, piece_placement,rot_placement,assembled_width,assembled_height)
solution.save("D:/Python_Script/puzzle/solution.png")

make_solution(save_path,piece_placement,rot_placement,puzzle_inf)
make_dummy(save_path,piece_placement,rot_placement,puzzle_inf)
elapsed_time=time.time()-start
print("実行時間:cost計測~パズル組み立て~解答保存")
print("elapsed_time:{0}".format(elapsed_time)+"[sec]")
print("assemble_time:{0}".format(elapsed_time-weight_time)+"[sec]")
solution.show()