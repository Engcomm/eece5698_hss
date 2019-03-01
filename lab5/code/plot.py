import argparse
import matplotlib.pyplot as plt

argparser = argparse.ArgumentParser(description="Plot Analysis.c Result File")
argparser.add_argument('result_file', help="the result file")

args = argparser.parse_args()

result_file = args.result_file

counter = []

def loadResultFile():
    global counter
    fid = open(result_file, 'r')
    lines = fid.readlines()
    fid.close()
    lineHandler = lambda x: map(float, x.split(' '))
    counter = map(lineHandler, lines)

def plotCounter():
    fig, axes = plt.subplots(4, 4, sharex='all', sharey='all')
    y0 = 99999
    y2 = 0
    for i in range(16):
        col = i % 4
        row = int(i / 4)
        axes[row, col].plot(range(256), counter[i], 'bo', markersize=2)
        axes[row, col].set_title('C'+str(i))
        if y0 > min(counter[i]):
            y0 = min(counter[i])
        if y2 < max(counter[i]):
            y2 = max(counter[i])
    y1 = int((y2 - y0) / 2 + y0)
    plt.setp(axes, xticks=[0, 128, 255], yticks=[y0, y1, y2])
    plt.show()

loadResultFile()
plt.show()
plotCounter()
