import sys
import matplotlib.pyplot as plt
import numpy as np
import math

plt.rcParams.update({'font.size':22})
def plot_histo(data_hit, data_miss, figure):
    ax = figure.add_subplot(211)
    ax.set_title('Timing distribution for system profile, sample size = %d' % len(data_hit))
    ax.set_xlabel('Time (cycles)')
    ax.set_ylabel('Frequency, Bin Width = 5 Cycles')
    mean_hit = np.mean(data_hit)
    std_hit = np.std(data_hit)
    print("STD hit is %d" % std_hit)
    # data_hit = [d for d in data_hit if d < (mean_hit + std_hit)]

    mean_miss = np.mean(data_miss)
    std_miss = np.std(data_miss)
    print("STD Miss is %d" % std_miss)
    # data_miss = [d for d in data_miss if d < (mean_miss + std_miss)]

    bin_list = range(np.min(data_hit), np.max(data_hit) + 5, 5)
    ax.set_xlim(np.min(data_hit), mean_hit*2)
    ax.hist(data_hit, color='g', bins=bin_list)
    ax.legend(['Cache Hit Timing'])
    ax.text((ax.get_xlim()[0] + ax.get_xlim()[1]) / 2, ax.get_ylim()[1]/2, "Mean for hit data is %d cycles" % np.mean(data_hit), color='g', fontsize=40)

    ax = figure.add_subplot(212)
    ax.set_xlabel('Time (cycles)')
    ax.set_ylabel('Frequency, Bin Width = 20 Cycles')
    bin_list = range(np.min(data_miss), np.max(data_miss) + 20, 20)
    ax.set_xlim(np.min(data_miss) - (np.min(data_miss) % 20), np.max(data_miss))
    ax.hist(data_miss, bins=bin_list)
    ax.text((ax.get_xlim()[0] + ax.get_xlim()[1]) / 2, ax.get_ylim()[1]/2, "Mean for missed data is is %d cycles" % np.mean(data_miss), color='b', fontsize=40)
    ax.legend(['Cache Miss Timing'])

    print("Hit Mean before removal of excess %d" % mean_hit)
    print("Miss Mean before removal of excess %d" % mean_miss)
    print("Hit Mean before removal of excess %d" % np.mean(data_hit))
    print("Miss Mean before removal of excess %d" % np.mean(data_miss))

    



def read_data_to_numpy(file):
    x = np.fromfile(file, dtype=np.int32, sep='\n')
    print(np.mean(x))
    print(x)
    return x


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage %s file_hit file_miss' % sys.argv[0])

    file_hit = sys.argv[1]
    data_hit = read_data_to_numpy(file_hit)

    file_miss = sys.argv[2]
    data_miss = read_data_to_numpy(file_miss)
    
    plot_histo(data_hit, data_miss, plt.figure())

    plt.show()
