from itertools import product

import numpy as np
import mgc
from PIL import Image
from skimage import io
import matplotlib.pyplot as plt
import copy
import scipy

EMPTY_SLOT = -1
INFINITY = 10**9
EPSILON = 0.1**6

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3
cl_save=[]
pos_save=[]
rot_save=[]
node_save=[]
trees_save=[]
def __compute_weights(pieces,pieces_count,edges):
    '''
    S(xi,xj,erel)==S(K,K,16) 3D array
    K is piece count
    relation=erel%4
    rotation=erel/4
    '''
    dissimilarity=mgc.rgb_mgc
    weights = np.zeros((pieces_count, pieces_count, 16), dtype=np.float)
    weights.fill(INFINITY)
    for i, j, erel in edges:
        weights[i, j, erel] = dissimilarity(pieces[i], pieces[j], erel)

    normalized_weights = np.array(weights)
    for i, j, erel in edges:
        #min_weight=10**9
        min_weight = min(weights[i, :, erel].min(), weights[:, j, erel].min())
        normalized_weights[i, j, erel] = weights[i, j, erel] / (min_weight + EPSILON)
        #print(normalized_weights[i, j, erel])
    return normalized_weights
def __initial_positions(pieces,pieces_count,width,height,edges):
    height=16
    width=16
    def initial_puzzle_data(width,height):
        puzzle_cluster={}
        puzzle_cluster_pisition={}
        puzzle_cluster_rotation={}
        for i in range(width*height):
            puzzle_cluster_pisition[i]=np.zeros((height*width,2),dtype=np.int32)
            puzzle_cluster_rotation[i]=np.zeros((height*width),dtype=np.int32)
            puzzle_cluster[i]=np.empty((height,width),dtype=np.int32)
            puzzle_cluster[i].fill(-1)
            puzzle_cluster[i][0,0]=i
        return puzzle_cluster,puzzle_cluster_pisition,puzzle_cluster_rotation
    def rotation_j(cluster_j,rot_j,pj,rel,trees_j):
        rot=rot_j[pj]

        rot_j_c=np.empty((height*width),dtype=np.int32)
        for i,r in enumerate(rot_j):
            rot_j_c[i]=r
        rot=(4-rot)
        rot+=(rel//4)
        rot%=4

        row=0
        col=0
        for i in range(height):
            if np.any(cluster_j[i,:]!=-1):
                row+=1
        for i in range(width):
            if np.any(cluster_j[:,i]!=-1):
                col+=1

        if rot==0 or rot==2:
            rot_cluster_j=np.empty((row,col),dtype=np.int32)
            rot_cluster_j.fill(-1)
            rot_cluster_j=np.rot90(cluster_j[:row,:col],-rot)

        if rot==1 or rot==3:  
            rot_cluster_j=np.empty((col,row),dtype=np.int32)
            rot_cluster_j.fill(-1)
            rot_cluster_j=np.rot90(cluster_j[:row,:col],-rot)
    
        rot_cluster_matrix=np.empty((height,width),dtype=np.int32)
        rot_cluster_matrix.fill(-1)
        position_j=np.zeros((height*width,2),dtype=np.int32)
    
        if rot==0 or rot==2:
            for i in range(row):
                for j in range(col):
                    if rot_cluster_j[i,j] in trees_j:
                        rot_cluster_matrix[i,j]=rot_cluster_j[i,j]
                        position_j[rot_cluster_j[i,j]]=[i,j]
                        rot_j_c[rot_cluster_j[i,j]]+=rot
    
        if rot==1 or rot==3:
            for i in range(col):
                for j in range(row):
                    if rot_cluster_j[i,j] in trees_j:
                        rot_cluster_matrix[i,j]=rot_cluster_j[i,j]
                        position_j[rot_cluster_j[i,j]]=[i,j]
                        rot_j_c[rot_cluster_j[i,j]]+=rot
        #print("j",rot_cluster_matrix.shape)
        return rot_cluster_j,rot_cluster_matrix,position_j,rot_j_c%4
    def conflict_check(position_i,position_j,rel,i,j,trees,tree_i,tree_j):
                                    #L     R      U     D
        relation_offset=np.array([[0,1],[0,-1],[1,0],[-1,0]])   
        offset_j=position_i[i]-position_j[j]+relation_offset[rel%4]
        
        took_same_place = (np.all(position_i[node_1] == position_j[node_2]+offset_j)
                       for node_1, node_2 in product(trees[tree_i], trees[tree_j]))                        

        return took_same_place,offset_j

    def change_matrix_j(offset_j,cluster_j,change_position_j,rel,trees_j,):
        cluster_j=change_position_j
        for node in trees_j:
            cluster_j[node]+=offset_j
    
        return cluster_j
    def combine(puzzle_i,cluster_i,cluster_j,cluster_rot_i,cluster_rot_j,trees,node_i,node_j):
        trees[node_i]=trees[node_i].union(trees[node_j])
        for node in trees[node_j]:
            node_tree[node]=node_i
            cluster_rot_i[node]=cluster_rot_j[node]
            cluster_i[node]=cluster_j[node]
            puzzle_i[tuple(cluster_i[node])]=node
        
        return puzzle_i,cluster_i,cluster_rot_i
    def shift_cluster_i(position_i,cluster_i,offset_j,trees_i):
        shift=[0,0]
        if offset_j[0]<0:
            shift[0]+=abs(offset_j[0])
        if offset_j[1]<0:
            shift[1]+=abs(offset_j[1])

        if offset_j[0]>=0 and offset_j[1]>=0:
            return cluster_i

        cluster=cluster_i
        cluster.fill(-1)
        for node in trees_i:
            cluster[tuple(position_i[node])]=node
    
        cluster=np.roll(cluster,tuple(shift),axis=(0,1))
        position_i.fill(0)
        for i in range(height):
            for j in range(width):
                if cluster[i,j] in trees_i:
                    position_i[cluster[i,j]]=[i,j]
    
        return cluster
    def rotation_i(cluster,cluster_pos,cluster_rot_i,pi,trees_i):
        rot=cluster_rot_i[pi]

        if rot==0:
            return cluster,cluster_pos,cluster_rot_i
        row=0
        col=0
        rot_c=np.empty((height*width),dtype=np.int32)
        for i,r in enumerate(cluster_rot_i):
            rot_c[i]=r

        for i in range(height):
            if np.any(cluster[i,:]!=-1):
                row+=1
        for i in range(width):
            if np.any(cluster[:,i]!=-1):
                col+=1

        if rot==2:
            rot_cluster=np.empty((row,col),dtype=np.int32)
            rot_cluster.fill(-1)
            rot_cluster=np.rot90(cluster[:row,:col],rot)

        elif rot==1 or rot==3:  
            rot_cluster=np.empty((col,row),dtype=np.int32)
            rot_cluster.fill(-1)
            rot_cluster=np.rot90(cluster[:row,:col],rot)
    
        rot_cluster_matrix=np.empty((height,width),dtype=np.int32)
        rot_cluster_matrix.fill(-1)
        position_i=np.zeros((height*width,2),dtype=np.int32)
    
        if rot==2:
            for i in range(row):
                for j in range(col):
                    if rot_cluster[i,j] in trees_i:
                        rot_cluster_matrix[i,j]=rot_cluster[i,j]
                        position_i[rot_cluster[i,j]]=[i,j]
                        rot_c[rot_cluster[i,j]]+=(4-rot)
    
        elif rot==1 or rot==3:
            for i in range(col):
                for j in range(row):
                    if rot_cluster[i,j] in trees_i:
                        rot_cluster_matrix[i,j]=rot_cluster[i,j]
                        position_i[rot_cluster[i,j]]=[i,j]
                        rot_c[rot_cluster[i,j]]+=(4-rot)
        #print("i",rot_cluster_matrix.shape)
        return rot_cluster_matrix,position_i,rot_c%4
    def fix_puzzle(puzzle,puzzle_rot,position):
        rot=puzzle_rot[0]

        if rot==0:
            return
        puzzle=np.rot90(puzzle,rot)

        puzzle_rot+=(4-rot)
        puzzle_rot%=4

    def save_cluster(puzzle_cluster,puzzle_cluster_position,puzzle_cluster_rotation,node_tree,trees):
        cl_save=copy.deepcopy(puzzle_cluster)
        pos_save=copy.deepcopy(puzzle_cluster_position)
        rot_save=copy.deepcopy(puzzle_cluster_rotation)
        node_save=copy.deepcopy(node_tree)
        trees_save=copy.deepcopy(trees)
        #print_clu(cl_save,pos_save,rot_save,node_save,trees_save)
    def print_clu(cl_save,pos_save,rot_save,node_save,trees_save):
        node_list=np.unique(node_save)
            
        for i in node_list:
            rotation_place = __place_pieces(pos_save[i],rot_save[i],height,width)
            solution=__paste_pieces(pieces,cl_save[i],rotation_place,width,height)
            solution.show()

    puzzle_cluster,puzzle_cluster_position,puzzle_cluster_rotation=initial_puzzle_data(width,height)
    trees = [{i} for i in range(pieces_count)]
    node_tree = [i for i in range(pieces_count)]                            
    conflict=0
    for i, j, rel in edges:
        tree_i, tree_j = node_tree[i], node_tree[j]
        
        if tree_i == tree_j:
            continue
        rot_cluster,rot_cluster_matrix,rot_position,rot_j=rotation_j(puzzle_cluster[tree_j],puzzle_cluster_rotation[tree_j],j,rel,trees[tree_j])
        rot_c_i,rot_posi_i,rot_i=rotation_i(puzzle_cluster[tree_i],puzzle_cluster_position[tree_i],puzzle_cluster_rotation[tree_i],i,trees[tree_i])

        '''ピースの重なりがないか判定 '''
        took_same_place,offset_j=conflict_check(rot_posi_i,rot_position,rel,i,j,trees,tree_i,tree_j)
        if any(took_same_place):
            #print("conflict")
            if conflict==0:
                save_cluster(puzzle_cluster,puzzle_cluster_position,puzzle_cluster_rotation,node_tree,trees)
            conflict+=1
            continue  

        puzzle_cluster[tree_i]=rot_c_i
        puzzle_cluster_position[tree_i]=rot_posi_i
        puzzle_cluster_rotation[tree_i]=rot_i
        puzzle_cluster_rotation[tree_j]=rot_j

        puzzle_cluster_position[tree_j]=change_matrix_j(offset_j,puzzle_cluster_position[tree_j],rot_position,rel,trees[tree_j])
        
        puzzle_cluster[tree_i],puzzle_cluster_position[tree_i],puzzle_cluster_rotation[tree_i]=combine(puzzle_cluster[tree_i],
                                                                        puzzle_cluster_position[tree_i],puzzle_cluster_position[tree_j],
                                                                        puzzle_cluster_rotation[tree_i],puzzle_cluster_rotation[tree_j],
                                                                        trees,tree_i,tree_j)

        puzzle_cluster[tree_i]=shift_cluster_i(puzzle_cluster_position[tree_i],puzzle_cluster[tree_i],offset_j,trees[tree_i])
        
        '''
        rotation_place = __place_pieces(puzzle_cluster_position[tree_i],puzzle_cluster_rotation[tree_i],height,width)
        solution=__paste_pieces(pieces,puzzle_cluster[tree_i],rotation_place,width,height)
        solution.show()
        '''
    rotation_place = __place_pieces(puzzle_cluster_position[tree_i],puzzle_cluster_rotation[tree_i],height,width)
    solution=__paste_pieces(pieces,puzzle_cluster[tree_i],rotation_place,width,height)
    solution.show()

    return puzzle_cluster[tree_i],puzzle_cluster_position[tree_i],puzzle_cluster_rotation[tree_i]
    
def __place_pieces(positions,rotations,puzzle_height,puzzle_width):

    rotation_place = np.zeros((puzzle_height, puzzle_width), dtype=np.int32)
    for i, position in enumerate(positions):
        rotation_place[tuple(position)]=rotations[i]

    return rotation_place
def TrimPuzzle(cluster,rot_cl,nr,nc,rotFlag):

    ss=cluster.shape
    newBlock=copy.deepcopy(cluster)
    newRotBlock=copy.deepcopy(rot_cl)
    if max(ss)<=max((nr,nc)) and min(ss)<=min((nr,nc)):
        okey=1
    else:
        rowMarg=np.sum(cluster>-1,axis=1)
        colMarg=np.sum(cluster>-1,axis=0)
        totalChopped1,rs1,cs1=findBestCrop(rowMarg,colMarg,nr,nc)
        totalChopped2,rs2,cs2=findBestCrop(rowMarg,colMarg,nr,nc)

        if totalChopped1>0 and (totalChopped1<totalChopped2 or (rotFlag==0)):
            ss=cluster.shape
            newBlock=copy.deepcopy(cluster)

            newBlock=copy.deepcopy(newBlock[rs1:min(rs1+nr,ss[0]),cs1:min(cs1+nc,ss[1])])
            newRotBlock=copy.deepcopy(rot_cl[rs1:min(rs1+nr,ss[0]),cs1:min(cs1+nc,ss[1])])

        elif totalChopped2>0 and (totalChopped2<totalChopped1 and (rotFlag==1)):
            ss=cluster.shape
            newBlock=copy.deepcopy(cluster)
            newBlock=newBlock[rs2:min(rs2+nc,ss[0]),cs2:min(cs2+nr,ss[1])]
            newRotBlock=rot_cl[rs2:min(rs2*nc,ss[0]),cs2:min(cs2+nr,ss[1])]
    return newBlock,newRotBlock
def findBestCrop(rowMarg,colMarg,nr,nc):
    totalCopped=0
    totalPieces=np.sum(rowMarg)
    rs=0
    cs=0
    if len(rowMarg)>nr:
        startR=range(len(rowMarg)-nr)

        size=len(rowMarg)-nr
        piecesKeptR=np.empty((size),dtype=np.int32)
        piecesKeptR.fill(-1)
        for i in startR:
            piecesKeptR[i]=np.sum(rowMarg[i:i+nr])
        bbR=np.argmax(piecesKeptR)
        aa=np.max(piecesKeptR)
        #print(aa)
        choppedR=totalPieces-aa
        totalCopped=totalCopped+choppedR
        rs=bbR
    if len(colMarg)>nc:
        startC=range(len(colMarg)-nc)
        size=len(colMarg)-nc
        piecesKeptC=np.empty((size),dtype=np.int32)
        piecesKeptC.fill(-1)
        for i in startC:
            piecesKeptC[i]=np.sum(colMarg[i:i+nc])
        bbc=np.argmax(piecesKeptC)
        aa=np.max(piecesKeptC)
        #print(aa)
        choppedC=totalPieces-aa
        totalCopped=totalCopped+choppedC
        cs=bbc
    return totalCopped,rs,cs
def __paste_pieces(npieces, piece_placement,rotation_place,puzzle_width,puzzle_height):
    piece_size=npieces[0].shape[0]
    image = Image.new('RGB', (puzzle_width*piece_size, puzzle_height*piece_size))
    for i in range(puzzle_height):
        for j in range(puzzle_width):
            if piece_placement[i,j]!=EMPTY_SLOT:
                piece=piece_placement[i,j]
                rot=rotation_place[i,j]
                img=np.rot90(npieces[piece],-rot)
                box=(j,i,j+1,i+1)
                box = list(map(lambda v: v*piece_size, box))
                img=Image.fromarray(img)
                image.paste(img, box)

    return image

def __fill_unused(piece_placement,rot_placemant, unused_pieces,puzzle_height,puzzle_width,weight):
    def is_nonempty(x, y):
        h, w = piece_placement.shape
        return 0 <= x < w and 0 <= y < h and piece_placement[y, x] != EMPTY_SLOT

    def get_neighbours(x, y):
        if is_nonempty(x-1, y):#左にピースがある
            rot=rot_placemant[y,x-1]
            if rot==0:
                yield (0,4,8,12), piece_placement[y, x-1]
            elif rot==1:
                yield (2,6,10,14), piece_placement[y, x-1]
            elif rot==2:
                yield (1,5,9,13), piece_placement[y, x-1]
            elif rot==3:
                yield (3,7,11,15), piece_placement[y, x-1]
        if is_nonempty(x+1, y):#右にピースがある
            rot=rot_placemant[y,x+1]
            if rot==0:
                yield (1,5,9,13), piece_placement[y,x+1]
            elif rot==1:
                yield (3,7,11,15), piece_placement[y,x+1]
            elif rot==2:
                yield (0,4,8,12), piece_placement[y,x+1]
            elif rot==3:
                yield (2,6,10,14), piece_placement[y,x+1]
        if is_nonempty(x, y-1):#上にピースがある
            rot=rot_placemant[y-1,x]
            if rot==0:
                yield (2,6,10,14), piece_placement[y-1,x]
            elif rot==1:
                yield (1,5,9,13), piece_placement[y-1,x]
            elif rot==2:
                yield (3,7,11,15), piece_placement[y-1,x]
            elif rot==3:
                yield (0,4,8,12), piece_placement[y-1,x]
        if is_nonempty(x, y+1):#下にピースがある
            rot=rot_placemant[y+1,x]
            if rot==0:
                yield (3,7,11,15), piece_placement[y+1,x]
            elif rot==1:
                yield (0,4,8,12), piece_placement[y+1,x]
            elif rot==2:
                yield (2,6,10,14), piece_placement[y+1,x]
            elif rot==3:
                yield (1,5,9,13), piece_placement[y+1,x]
    def get_neighbour_dissimilarity(piece, neighbours):

        dissimilarities = [get_dissimilarity(nb, piece,r,weight) for rel, nb in neighbours for r in rel]
        dissimilarities=np.array(dissimilarities)

        dissimilarity = np.sum(dissimilarities[:,0],axis=0)/len(dissimilarities[:,0])
        if dissimilarity < EPSILON:
            return INFINITY
        b=np.argmin(dissimilarities[:,0])

        return dissimilarities[b]
    def find_bestfit(unused_pieces,neighbours):
        candidate_piece=np.array([float('inf'),-1,-1])
        for p in unused_pieces:
            candidate_piece=np.vstack((candidate_piece,get_neighbour_dissimilarity(p,neighbours)))

        b=np.argmin(candidate_piece[:,0],axis=0)
        rot=candidate_piece[b,1]
        best_fit=candidate_piece[b,2]

        return best_fit,rot

    while unused_pieces:
        empty_slots=set()
        for x,y in product(range(puzzle_width),range(puzzle_height)):
            if piece_placement[y,x]==EMPTY_SLOT:
                if y==0 and x==0:
                    if piece_placement[y+1,x]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT:
                        empty_slots.add((x,y))
                elif y==puzzle_height-1 and x==0:
                    if piece_placement[y-1,x]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT:
                        empty_slots.add((x,y))
                elif y==0 and x==puzzle_width-1:
                    if piece_placement[y,x-1]!=EMPTY_SLOT or piece_placement[y+1,x]!=EMPTY_SLOT:
                        empty_slots.add((x,y))
                elif y==puzzle_height-1 and x==puzzle_width-1:
                    if piece_placement[y,x-1]!=EMPTY_SLOT or piece_placement[y-1,x]!=EMPTY_SLOT:
                        empty_slots.add((x,y))
                else:
                    if y==0:
                        if piece_placement[y+1,x]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT or piece_placement[y,x-1]!=EMPTY_SLOT:
                            empty_slots.add((x,y))
                    elif y==puzzle_height-1:
                        if piece_placement[y,x-1]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT or piece_placement[y-1,x]!=EMPTY_SLOT:
                            empty_slots.add((x,y))
                    elif x==0:
                        if piece_placement[y-1,x]!=EMPTY_SLOT or piece_placement[y+1,x]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT:
                            empty_slots.add((x,y))
                    elif x==puzzle_width-1:
                        if piece_placement[y+1,x]!=EMPTY_SLOT or piece_placement[y-1,x]!=EMPTY_SLOT or piece_placement[y,x-1]!=EMPTY_SLOT:
                            empty_slots.add((x,y))
                    else:
                        if piece_placement[y+1,x]!=EMPTY_SLOT or piece_placement[y-1,x]!=EMPTY_SLOT or piece_placement[y,x-1]!=EMPTY_SLOT or piece_placement[y,x+1]!=EMPTY_SLOT:
                            empty_slots.add((x,y))

        while empty_slots:
            slots_neighbours = [(x, y, list(get_neighbours(x, y))) for x, y in empty_slots]
            x, y, neighbours = max(slots_neighbours, key=lambda nbs: len(nbs))

            best_fit,rot=find_bestfit(unused_pieces,neighbours)
            piece_placement[y, x] = best_fit
            rot_placemant[y,x]=rot//4
            unused_pieces.remove(best_fit)
            empty_slots.remove((x, y))

    return piece_placement

def get_dissimilarity( p_1, p_2, rel,weights):
    return weights[p_1, p_2, rel],rel,p_2
def save_comp():
    return cl_save,pos_save,rot_save,node_save,trees_save