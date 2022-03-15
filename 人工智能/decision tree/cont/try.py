file = open('traindata.txt', "r",encoding='utf8')
dataSet=[]
for line in file.readlines():
    line=line.strip('\n')
    line=line.split("\t")
    line=list(map(float,line))
    dataSet.append((line))
for sub in dataSet:
    last=len(sub)
    sub[last-1]=str(sub[last-1])
#print(dataSet)

# for i in range(4):
#     print(i)

# for sub in dataSet:
#     if sub[2]>4.75:
#         print(sub)
# for sub in dataSet:
#     if sub[2]>4.75:
#         count=0
#         for i in sub:
#             count+=1
#             temp=float(i)
#             if count==5:
#                 print(temp)
#             else:
#                 print(temp,end=' ')