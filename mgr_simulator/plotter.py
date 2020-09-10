import matplotlib.pyplot as plt
import csv

dt = 0.001

x0 = []
x1 = []
x2 = []
x3 = []
x4 = []
x5 = []
setp = []
setp2 = []
T = []
t = 0


#################### SERIES RLC ####################
with open('./data/rlc.txt', 'r') as csvfile:
    dc_plot = csv.reader(csvfile, delimiter=',')
    for row in dc_plot:
        t = t + dt
        T.append(t) 
        x0.append(float(row[0]))
        x1.append(float(row[1]))
        
plt.plot(T, x0, label="Capacitor Voltage")
plt.plot(T, x1, label="Circuit Current")
plt.grid(linestyle=':')
leg = plt.legend()


#################### DC MOTOR ####################
plt.figure()
x0 = []
x1 = []
x2 = []
x3 = []
x4 = []
x5 = []
setp = []
setp2 = []
T = []
t = 0
with open('./data/dc.txt', 'r') as csvfile:
    dc_plot = csv.reader(csvfile, delimiter=',')
    for row in dc_plot:
        t = t + dt
        T.append(t) 
        x0.append(float(row[0]))
        x1.append(float(row[1]))
        x2.append(float(row[2]))
        x3.append(float(row[3]))
        x4.append(float(row[4]))
        setp.append(150)
        

plt.plot(T, x0, label="Rotor Current")
plt.plot(T, x1, label="Angular Velocity")
plt.plot(T, x2, label="NN output = Input Voltage")
plt.plot(T, x3, label="Noised rotor current")
plt.plot(T, x4, label="External torque")
plt.plot(T, setp, label="Setpoint")
plt.grid(linestyle=':')
leg = plt.legend()


#################### PENDULUM ####################
plt.figure()
x0 = []
x1 = []
x2 = []
x3 = []
x4 = []
x5 = []
setp = []
setp2 = []
T = []
t = 0
with open('./data/pen.txt', 'r') as csvfile:
    pen_plot = csv.reader(csvfile, delimiter=',')
    for row in pen_plot:
        t = t + dt
        T.append(t) 
        x0.append(float(row[0]))
        x1.append(float(row[1]))
        x2.append(float(row[2]))
        x3.append(float(row[3]))
        setp.append(3.14)
        setp2.append(-3.14)

plt.plot(T, x0, label="pozycja wózka")
plt.plot(T, x1, label="pozycja ramienia")
plt.plot(T, x2, label="prędkość wózka", linestyle='--')
plt.plot(T, x3, label="prędkość ramienia", linestyle='--')
plt.plot(T, setp, label="Pi", linestyle=':', color="grey")
plt.plot(T, setp2, label="-Pi", linestyle=':', color="grey")
plt.grid(linestyle=':')
leg = plt.legend()



#################### DOUBLE PENDULUM ####################
plt.figure()
x0 = []
x1 = []
x2 = []
x3 = []
x4 = []
x5 = []
setp = []
setp2 = []
T = []
t = 0
with open('./data/dip.txt', 'r') as csvfile:
    dip_plot = csv.reader(csvfile, delimiter=',')
    for row in dip_plot:
        t = t + dt
        T.append(t) 
        x0.append(float(row[0]))
        x1.append(float(row[1]))
        x2.append(float(row[2]))
        x3.append(float(row[3]))
        x4.append(float(row[4]))
        x5.append(float(row[5]))
        setp.append(3.14)
        setp2.append(-3.14)

plt.plot(T, x0, label="pozycja wózka")
plt.plot(T, x1, label="pozycja ramienia 1")
plt.plot(T, x2, label="pozycja ramienia 2")
plt.plot(T, x3, label="prędkość wózka", linestyle='--')
plt.plot(T, x4, label="prędkość ramienia 1", linestyle='--')
plt.plot(T, x5, label="prędkość ramienia 2", linestyle='--')
plt.plot(T, setp, label="Pi", linestyle=':', color="grey")
plt.plot(T, setp2, label="-Pi", linestyle=':', color="grey")
plt.grid(linestyle=':')
leg = plt.legend()








plt.show()
