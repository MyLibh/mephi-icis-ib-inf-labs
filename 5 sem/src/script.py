from os import write
import matplotlib.pyplot as plt
import re
from statistics import mean
import sys
import csv

INPUT_FILE = "log.txt"
REGEXP = r"Test \#(.*?) threads_num: (.*?) time: (.*?)s\n"
MAX_THREADS_NUM = 16
OUTPUT_PATH = "../images/"
EXT_FILENAME = ""

def write_csv(table):
    with open("data" + EXT_FILENAME + ".csv", "w", newline="") as csvfile:
        writer = csv.writer(csvfile)

        headers = ["Threads", "Avg"]
        tests = [j + 1 for j in range(10)]
        headers.extend(tests)
        writer.writerow(headers)

        for i in range(MAX_THREADS_NUM):
            tmp = [i + 1, mean(table[i])]
            tmp.extend(table[i])
            writer.writerow(tmp)

def load_table(input_filename, regexp, max_threads):
    with open(input_filename, "r") as input:
        content = input.read()

    data = re.findall(regexp, content)

    table = [[] for i in range(max_threads)]
    for str in data:
        test, threads, time = str
        table[int(threads) - 1].append(float(time))

    return table

def make_plots(x, y, y_theor, legend, f_name, x_min=1, x_max=MAX_THREADS_NUM, y_min=0, y_max=1.5):
    plt.axis([x_min, x_max, y_min, y_max])
    plt.plot(x, y)
    plt.plot(x, y_theor)
    plt.legend(legend)
    plt.savefig(OUTPUT_PATH + f_name + EXT_FILENAME + ".png")
    plt.clf()


if __name__ == "__main__":
    if len(sys.argv) == 2:
        EXT_FILENAME = sys.argv[1]

    table = load_table(INPUT_FILE, REGEXP, MAX_THREADS_NUM)

    time = [mean(table[i]) for i in range(MAX_THREADS_NUM)]
    time_theor = [time[0] / (i + 1) for i in range(MAX_THREADS_NUM)]
    x = [(i + 1) for i in range(MAX_THREADS_NUM)]
    make_plots(x, time, time_theor, ["time", "base"], "time", y_max=max(time))

    accel = [time[0] / time[i] for i in range(MAX_THREADS_NUM)]
    accel_theor = [time_theor[0] / time_theor[i] for i in range(MAX_THREADS_NUM)]
    make_plots(x, accel, accel_theor, ["speedup factor", "base"], "accel", y_min=0.5)

    eff = [accel[i] / (i + 1) for i in range (MAX_THREADS_NUM)]
    eff_theor = [accel_theor[i] / (i + 1) for i in range (MAX_THREADS_NUM)]
    make_plots(x, eff, eff_theor, ["efficiency", "base"], "eff")

    write_csv(table)