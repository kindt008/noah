import matplotlib
import numpy

import matplotlib.pyplot as plt




def similarityat(lst, idx):
    #vals=((a,0),(c,0),(g,0),(t,0))
    vals=[0]*4
    for j in range(len(lst)):
        a=lst[j][idx] #current char being tested
        if a=='A':
            vals[0]+=1
        elif a=='C':
            vals[1]+=1
        elif a=='G':
            vals[2]+=1
        elif a =='T':
            vals[3]+=1
    mi=0
    mv=0
    mc='z'
    for j in range(len(vals)):
        if vals[j]>mv:
            mi=j
            mv=vals[j]
            if j==0:
                mc='A'
            elif j==1:
                mc='C'
            elif j==2:
                mc='G'
            else:
                mc='T'
    return ((vals[mi]/len(lst)),mc)


#begin "main"
infile=open("Homework_4_seqs.fna","r")
seqs=[]
while True:
    s=infile.readline()
    if s=="":
        break
    sq=infile.readline()
    #s is seq id and sq is seq
    seqs.append(sq)
infile.close()
print("hi")
result=open("result.txt","w")
mat=open("mat.txt","w")
for i in range(1474):
    (v,c)=similarityat(seqs,i) # v is percent occurance at index i c is value occuring most
    V=v*100
    sr=str(i)+"\t"+c+"\t"+str(V)+"%\n"
    sm=str(V)+"\n"
    result.write(sr)
    mat.write(sm)
result.close()
mat.close()
