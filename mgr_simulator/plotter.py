import matplotlib.pyplot as plt
import csv

x0 = []
x1 = []
T = []
dt = 0.001
t = 0

with open('./data/dc.txt', 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        t = t + dt
        T.append(t) 
        x0.append(float(row[0]))
        x1.append(float(row[1]))
        

plt.plot(T, x0)
plt.plot(T, x1)
plt.grid(linestyle=':')
plt.show()
