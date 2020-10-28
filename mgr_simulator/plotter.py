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
try:
    with open('./data/rlc.txt', 'r') as csvfile:
        dc_plot = csv.reader(csvfile, delimiter=',')
        for row in dc_plot:
            t = t + dt
            T.append(t) 
            x0.append(float(row[0]))
            x1.append(float(row[1]))
            
        plt.plot(T, x0, label="Prąd w obwodzie - i(t) [A]")
        plt.plot(T, x1, label="Napięcie na kondensatorze - u(t) [V]")
        plt.grid(linestyle=':')
        leg = plt.legend()
except:
    print("Rlc.txt file not found.")

#################### DC MOTOR ####################
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
try:
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
        
        plt.figure()
        plt.plot(T, x0, label="Prąd wirnika - i(t) [A]")
        plt.plot(T, x1, label="Prędkość obrotowa wirnika - \u03C9(t) [obr/min]")
        plt.plot(T, x2, label="Napięcie wejściowe - u(t) [V]")
        # plt.plot(T, x3, label="Zaszumiony pomiar prądu wirnika - i'(t) [A]")
        # plt.plot(T, x4, label="Moment obciążenia wału - T(t) [Nm]")
        # plt.plot(T, setp, label="Prędkość zadana - S(t) [obr/min]")
        plt.grid(linestyle=':')
        leg = plt.legend()
except:
    print("Dc.txt file not found.")


#################### PENDULUM ####################
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

try:
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

        plt.figure()
        plt.plot(T, x0, label="Pozycja wózka - \u03B80 [m]")
        plt.plot(T, x1, label="Pozycja ramienia - \u03B81 [rad]")
        plt.plot(T, x2, label="Prędkość wózka - \u03B80' [m/s]", linestyle='--')
        plt.plot(T, x3, label="Prędkość ramienia - \u03B81' [rad/s]", linestyle='--')
        plt.plot(T, setp, label="\u03C0; -\u03C0", linestyle=':', color="grey")
        plt.plot(T, setp2, linestyle=':', color="grey")
        plt.grid(linestyle=':')
        leg = plt.legend()
except:
    print("Pen.txt file not found.")


#################### DOUBLE PENDULUM ####################
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
try:
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

        plt.figure()
        plt.plot(T, x0, label="Pozycja wózka - \u03B80 [m]")
        plt.plot(T, x1, label="Pozycja ramienia 1 - \u03B81 [rad]")
        plt.plot(T, x2, label="Pozycja ramienia 2 - \u03B81 [rad]")
        # plt.plot(T, x3, label="Prędkość wózka - \u03B80' [m/s]", linestyle='--')
        # plt.plot(T, x4, label="Prędkość ramienia 1 - \u03B81' [rad/s]", linestyle='--')
        # plt.plot(T, x5, label="Prędkość ramienia 2 - \u03B82' [rad/s]", linestyle='--')
        plt.plot(T, setp, label="\u03C0; -\u03C0", linestyle=':', color="grey")
        plt.plot(T, setp2, linestyle=':', color="grey")
        plt.grid(linestyle=':')
        leg = plt.legend()
except:
    print("Dip.txt file not found.")


# plt.figure()
# plt.plot(T, x3, label="Prędkość wózka - \u03B80' [m/s]")
# plt.plot(T, x4, label="Prędkość ramienia 1 - \u03B81' [rad/s]")
# plt.plot(T, x5, label="Prędkość ramienia 2 - \u03B82' [rad/s]")
# plt.plot(T, setp, label="\u03C0", linestyle=':', color="grey")
# plt.plot(T, setp2, label="-\u03C0", linestyle=':', color="grey")
# plt.grid(linestyle=':')
# leg = plt.legend()



plt.show()
