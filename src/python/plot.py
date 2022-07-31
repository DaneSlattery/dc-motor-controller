from matplotlib.lines import Line2D
import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt
from matplotlib.axes import Axes
import serial

import time


class SerialPlot:
    def __init__(self) -> None:
        self.serial_port = serial.Serial()
        self.serial_port.port = 'COM5'
        self.serial_port.baudrate = 9600
        self.serial_port.timeout = 10

        self.serial_port.open()
        if self.serial_port == True:
            print("Serial port open. Configuration:\n")
            print(self.serial_port, "\n")  # print serial parameters
            if self.serial_port.in_waiting:
                self.serial_port.flush()

        self.fig, ax = plt.subplots(nrows=2, ncols=1)

        self.ax1: Axes = ax[0]
        self.ax2: Axes = ax[1]
        self.dt = 0.05
        self.maxt = 10
        self.tdata = [0]
        self.ydata = [0]
        self.y2data = [0]
        self.y3data = [0]
        self.line = Line2D(self.tdata, self.ydata, color='r')
        self.line2 = Line2D(self.tdata, self.y2data)
        self.line3 = Line2D(self.tdata, self.y3data, color='r')
        self.ax1.add_line(self.line)
        self.ax1.add_line(self.line2)
        self.ax1.set_ylim(-.1, 1.1)
        self.ax1.set_xlim(0, self.maxt)
        self.ax1.set_xlabel('Time [s]')

        self.ax2.add_line(self.line3)
        self.ax2.set_ylim(-.1, 1.1)
        self.ax2.set_xlim(0, self.maxt)

        self.fig.canvas.mpl_connect('close_event', self.on_close)

    def on_close(self, event):
        self.serial_port.close()

    def run(self):
        anim = animation.FuncAnimation(
            self.fig, self.animate, self.get_data, interval=self.dt*1000)
        plt.show()

    def get_data(self):
        try:
            if self.serial_port.in_waiting:
                line = self.serial_port.readline().decode('ascii')
                # print(line)
                line_as_list = line.split(':')
                pot = float(line_as_list[1].strip().split(',')[0])
                pwm = float(line_as_list[2].strip().split(',')[0])
                isr = float(line_as_list[3].strip(' \n\r').split(',')[0])
                yield pot, pwm, pwm
        except IndexError:
            pass

    def animate(self, vals):
        pot, pwm, isr = vals
        lastt = self.tdata[-1]
        t = lastt+self.dt
        if lastt > self.maxt:
            self.ax1.set_xlim(lastt, lastt + self.maxt)
            self.ax2.set_xlim(lastt, lastt + self.maxt)

            self.maxt += lastt

        # Add x and y to lists
        self.tdata.append(t)
        self.ydata.append(pot)
        self.y2data.append(pwm)
        self.y3data.append(isr)

        self.line.set_data(self.tdata, self.ydata)
        self.line2.set_data(self.tdata, self.y2data)
        self.line3.set_data(self.tdata, self.y3data)


my_ser = SerialPlot()
my_ser.run()
