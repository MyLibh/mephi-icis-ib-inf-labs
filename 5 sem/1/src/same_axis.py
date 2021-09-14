import csv
from script import make_plots, MAX_THREADS_NUM

def get_data(filename):
    data = []
    with open(filename + ".csv", newline="\n") as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',')
        for row in spamreader:
            try:
                data.append(float(row[2]))
            except ValueError:
                continue

    return data[1:]

data = get_data("data")
data_same = get_data("data_same")
x = [(i + 1) for i in range(MAX_THREADS_NUM)]
make_plots(x, data, data_same, ["diff_time", "same_time"], "comp_time", y_max=max(max(data), max(data_same)))

accel = [data[0] / data[i] for i in range(MAX_THREADS_NUM)]
accel_same = [data_same[0] / data_same[i] for i in range(MAX_THREADS_NUM)]
make_plots(x, accel, accel_same, ["diff_accel", "same_accel"], "comp_accel", y_max=max(max(accel), max(accel_same)))

eff = [accel[i] / (i + 1) for i in range (MAX_THREADS_NUM)]
eff_same = [accel_same[i] / (i + 1) for i in range (MAX_THREADS_NUM)]
make_plots(x, eff, eff_same, ["diff_eff", "same_eff"], "comp_eff", y_max=max(max(eff), max(eff_same)))