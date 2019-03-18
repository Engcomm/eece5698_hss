import sys
import matplotlib.pyplot as plt
import numpy as np
import math

plt.rcParams.update({'font.size':22})
def plot_histo(data_hit, data_miss, figure):
    ax = figure.add_subplot(111)
    ax.set_title('Timing distribution for system profile')
    ax.set_xlabel('Time (cycles)')
    ax.set_ylabel('Frequency')
    mean_hit = np.mean(data_hit)
    std_hit = np.std(data_hit)
    data_hit = [d for d in data_hit if d < (mean_hit + 3*std_hit)]

    mean_miss = np.mean(data_miss)
    std_miss = np.std(data_miss)
    data_miss = [d for d in data_miss if d < (mean_miss + 3*std_miss)]

    print(min(data_hit))
    print(max(data_hit))
    ax.hist(data_hit)

    ax.hist(data_miss)
    ax.text(np.mean(data_miss), plt.ylim()[1]-70000, "Mean for missed data is is %d cycles" % np.mean(data_miss), color='g', fontsize=40)
    ax.text(np.mean(data_miss), plt.ylim()[1]-30000, "Mean for hit data is %d cycles" % np.mean(data_hit), color='b', fontsize=40)
    ax.legend(['Cache Hit Timing', 'Cache Miss Timing'])



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
