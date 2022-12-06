import matplotlib.pyplot as plt
loss=[]
iters=[]
iter_bz4=None
dataset='abalone'
filelist=['CD-'+dataset,'quasi-newton-'+dataset, 'GD-'+dataset]
for func in filelist: #,
    with open(f'{func}.txt') as f:
        lines = f.readlines()
        i=0
        # print(lines)
        for line in lines:
            if lines[i]=='begin training...\n':
                break
            i+=1
        lines = lines[i+1:]
        # if iter_bz4 is None:
        #     iter_bz4 = len(lines) 
        tmp=[ float(line.split(":")[-1]) for line in lines ]
        loss.append(tmp)
        tmp = [ j for j in range(1,len(tmp)+1) ]
        # tmp = tmp[0:20]
        iters.append(tmp)
        f.close()


plt.figure(figsize=(20, 10), dpi=100)
plt.plot(iters[0], loss[0], c='blue', label="Conjugate Gradient")
plt.plot(iters[1], loss[1], c='red', label="Quasi-Newton")
plt.plot(iters[2], loss[2], c='green', label="Gradient Descent")

plt.ylim(0,110)

plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel("iterations", fontdict={'size': 16})
plt.ylabel("loss", fontdict={'size': 16})
if dataset=='body':
    title_insert='Bodyfat'
elif dataset=='housing':
    title_insert='Housing'
elif dataset=='abalone':
    title_insert='Abalone'
plt.title(f"Three Method on {title_insert} Datasets", fontdict={'size': 20})

plt.legend(['Conjugate Gradient','Quasi-Newton','Gradient Descent'],loc=1, prop={'size': 32}) # 'bz=4','bz=8','bz=16',
plt.show()