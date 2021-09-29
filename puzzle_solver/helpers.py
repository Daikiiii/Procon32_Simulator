import numpy as np
import codecs
from PIL import Image

def make_solution(save_path,Place_mat,Rotmat,puzzle_inf):
    width,height=puzzle_inf[:2]
    solution=[str(format(i,'x'))+str(format(j,'x')) for i in range(height)  for j in range(width)]
    solution=np.reshape(solution,(height,width))
    rot_mat=np.zeros((height,width),dtype=np.int32)
    for i in range(height):
        for j in range(width):
            piece1=Place_mat[i,j]//width
            piece2=Place_mat[i,j]%width
            solution[i,j]=str(format(piece1,'x'))+str(format(piece2,'x'))
            rot_mat[i,j]=Rotmat[i,j]
    #print(solution)
    #print(rot_mat)
    text_file=open(save_path+"puzzle_solution.txt","wt")
    text_file.write(str(width)+" "+str(height)+"\n")
    text_file.write(str(puzzle_inf[2])+"\n")
    text_file.write(str(puzzle_inf[3])+" "+str(puzzle_inf[4])+"\n")

    for i in range(height):
            for j in range(width):
                    text_file.write(solution[i,j])
                    if j!=width-1:
                        text_file.write(" ")
            text_file.write("\n")
    for i in range(height):
            for j in range(width):
                    text_file.write(str(rot_mat[i,j]))
                    if j!=width-1:
                        text_file.write(" ")
            text_file.write("\n")
    text_file.close()

def take_problem_inf(problem_path):
    def splite(str_inf,num):
        for i in range(2,len(str_inf)):
            if num==2 and str_inf[i]==' ':
                return str_inf[2:i+1],str_inf[i+1:len(str_inf)-1]
            elif num==1:
                return str_inf[2:len(str_inf)-1]
            
    with codecs.open(problem_path,'r',encoding='utf-8') as f:
        line=f.readline()
        divide=f.readline()
        select_count=f.readline()
        rate=f.readline()
        width,height=splite(divide,2)[:2]
        select_count=splite(select_count,1)
        select_rate,change_rate=splite(rate,2)[:2]
        #print(width,height)
        #print(select_count)
        #print(select_rate,change_rate)
        f.close()
    return [int(width),int(height),int(select_count),int(select_rate),int(change_rate)]
def problem_ppm_png(input_path,output_path):
    im=Image.open(input_path)
    im.show()
    im.save(output_path)
