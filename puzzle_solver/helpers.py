import numpy as np
import codecs
from PIL import Image
#回転情報だけあっているダミーの情報
def make_solution(save_path,Place_mat,Rotmat,puzzle_inf):
    width,height=puzzle_inf[:2]
    solution=[str(format(j,'x'))+str(format(i,'x')) for i in range(height)  for j in range(width)]
    solution=np.reshape(solution,(height,width))

    rot_mat=np.zeros((height,width),dtype=np.int32)

    for i in range(height):
        for j in range(width):
            piece1=Place_mat[i,j]//width
            piece2=Place_mat[i,j]%width
            solution[piece1,piece2]=str(format(j,'x'))+str(format(i,'x'))
            rot_mat[piece1,piece2]=Rotmat[i,j]
    
    text_file=open(save_path+"puzzle_solution.txt","wt")
    text_file.write(str(width)+" "+str(height)+"\n")
    text_file.write(str(puzzle_inf[2])+"\n")
    text_file.write(str(puzzle_inf[3])+" "+str(puzzle_inf[4])+"\n")

    for i in range(height):
        for j in range(width):
                text_file.write(solution[i,j])
                if j!=width-1:
                    text_file.write(" ")
        if i!=height-1:            
            text_file.write(" ")
    
    text_file.write("\n")
    #回転情報追加
    rot_inf=np.zeros((width*height),dtype=np.int32)
    for i in range(height):
        for j in range(width):
            rot_inf[Place_mat[i,j]]=rot_mat[i,j]
    for i in range(width*height):
        text_file.write(str(rot_inf[i]))
    text_file.write("\n")
    text_file.close()
    print("解答")
    print(solution)
    print(rot_mat)

def take_problem_inf(problem_path):
    def splite(str_inf,num):
        for i in range(2,len(str_inf)):
            if num==2 and str_inf[i]==' ':
                return str_inf[2:i+1],str_inf[i+1:len(str_inf)-1]
            elif num==1:
                return str_inf[2:len(str_inf)-1]
            
    with codecs.open(problem_path,'r',encoding='Shift_JIS') as f:
        line=f.readline()
        divide=f.readline()
        select_count=f.readline()
        rate=f.readline()
        width,height=splite(divide,2)[:2]
        select_count=splite(select_count,1)
        select_rate,change_rate=splite(rate,2)[:2]

        f.close()
    return [int(width),int(height),int(select_count),int(select_rate),int(change_rate)]
def problem_ppm_png(input_path,output_path):
    im=Image.open(input_path)
    im.show()
    im.save(output_path)

#回転情報だけあっているダミーの情報
def make_dummy(save_path,Place_mat,rot_mat,puzzle_inf):
    text_file=open(save_path+"solution.txt","wt")
    width,height=puzzle_inf[:2]
    rot_buf=rot_mat.flatten()
    rot_inf=np.zeros((width*height),dtype=np.int32)
    for i in range(height):
        for j in range(width):
            rot_inf[Place_mat[i,j]]=rot_mat[i,j]
    for i in range(width*height):
        text_file.write(str(rot_inf[i]))
    text_file.write("\n")
    text_file.write("1")
    text_file.write("\n")
    text_file.write("00")
    text_file.write("\n")
    text_file.write("1")
    text_file.write("\n")
    text_file.write("R")

    print("疑似解答保存")
    text_file.close()

'''
if __name__=='__main__':
    make_solution("D:/Python_Script/puzzle_text")
'''