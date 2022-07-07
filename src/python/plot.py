from glob import glob
import random
import numpy as np
from matplotlib import style
import matplotlib.animation as animation
import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

import time


# initialize serial port
ser = serial.Serial()
ser.port = 'COM3'  # Arduino serial port
ser.baudrate = 9600
ser.timeout = 10  # specify timeout when using readline()
ser.open()
if ser.is_open == True:
    print("\nAll right, serial port now open. Configuration:\n")
    print(ser, "\n")  # print serial parameters

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []  # store trials here (n)
ys = []  # store relative frequency here
pwm = []
global count
count = 0
# This function is called periodically from FuncAnimation


def animate(i, xs, ys, pwm):
    global count
    # Aquire and parse data from serial port
    line = ser.readline().decode('ascii')
    # print(line)
    line_as_list = line.split(':')
    count += 1
    relProb = line_as_list[1]
    relProb_as_list = relProb.split()
    relProb_float = float(relProb_as_list[0])

    # Add x and y to lists
    xs.append(count)
    ys.append(relProb_float)
    pwm.append(float(line_as_list[2].split(',')[0]))

    # Limit x and y lists to 20 items
    # xs = xs[-20:]
    # ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys, label="pot")
    ax.plot(xs, pwm, label="pwm")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('This is how I roll...')
    plt.ylabel('float')
    plt.legend()
    plt.axis([1, None, 0, 1.1])  # Use for arbitrary number of trials
    # plt.axis([1, 100, 0, 1.1])  # Use for 100 trial demo


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(
    fig, animate, fargs=(xs, ys, pwm), interval=10)

plt.show()
ser.close()             # close port
