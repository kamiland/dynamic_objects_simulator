import matplotlib.pyplot as plt
import csv

x0 = []
x1 = []
x2 = []
x3 = []
x4 = []
setp = []
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
        x2.append(float(row[2]))
        x3.append(float(row[3]))
        x4.append(float(row[4]))
        setp.append(150)
        

plt.plot(T, x0, label="Angular Velocity")
plt.plot(T, x1, label="Rotor Current")
plt.plot(T, x2, label="NN output = Input Voltage")
plt.plot(T, x3, label="Noised rotor current")
plt.plot(T, x4, label="External torque")
plt.plot(T, setp, label="Setpoint")
plt.grid(linestyle=':')
leg = plt.legend()
plt.show()
