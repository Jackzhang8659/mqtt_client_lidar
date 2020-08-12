import statistics
i = [1,2,5,6,12,19,21,27,45,75,150]
AVG = sum(i)/len(i)
dev = statistics.stdev(i)
print(AVG)
print(dev)
l = []
for I in i:
    l.append((I-AVG)/dev+10)
norm = [float(i)/sum(l)*100 for i in l]
print(l)