# no one EVER look at this okay PLEASE you're not allowed
import numpy as np 
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import math

aes_file = "aes.txt"
rsa_file = "rsa.txt"

with open(aes_file, 'r') as aes:
    aes_lines = aes.readlines()
    aes_data = [int(aes_line.split()[0]) for aes_line in aes_lines]

with open(rsa_file, 'r') as rsa:
    rsa_lines = rsa.readlines()
    rsa_data = [int(rsa_line.split()[0]) for rsa_line in rsa_lines]

aes_len_before = len(aes_data)
aes_data = [x for x in aes_data if x < 1000]
num_removed = len(aes_data) - aes_len_before

aes_mean = np.mean(aes_data, dtype = np.float64)
print("AES mean is: {}\n".format(aes_mean))

aes_std = np.std(aes_data, dtype=np.float64)
print("AES \mu is: {}\n".format(aes_std))

num_outside_four_sigma = len([x for x in aes_data if x > (aes_mean + 4*aes_std)])
num_outside_four_sigma += num_removed
percent_outside_four_sigma = 100 * (num_outside_four_sigma / float(aes_len_before))
print(percent_outside_four_sigma)

custom_legend = [Line2D([0], [0], color='blue', lw=10),
                Line2D([0], [0], color='magenta', lw=10),
                Line2D([0], [0], color='green', lw=10),
                Line2D([0], [0], color='cyan', lw=10),
                Line2D([0], [0], color='white', lw=0),
                Line2D([0], [0], color='white', lw=0)]

plt.rcParams.update({'font.size': 22})
plt.xlim(math.ceil(aes_mean - 4 * aes_std), math.ceil(aes_mean + 4*aes_std))

plt.grid(axis='y', color='black', alpha=0.5)
plt.title('Distribution of AES Encryption time in number of clock cycles')
n, bins, patches = plt.hist(aes_data, color='red', edgecolor='black', bins=int(round(len(aes_data) ** (1 /3))))
plt.xlabel('AES encryption time (Clock cycles)')
plt.ylabel('Counts')
plt.ylim(0, n.max() + 500)
plt.fill_between([int(aes_mean - aes_std), int(aes_mean + aes_std)], 0, plt.ylim()[1], color = 'blue', alpha=0.5)

plt.fill_between([int(aes_mean - 2*aes_std), int(aes_mean - aes_std)], 0, plt.ylim()[1], color='magenta', alpha=0.5)
plt.fill_between([int(aes_mean + aes_std), int(aes_mean + 2*aes_std)], 0, plt.ylim()[1], color='magenta', alpha=0.5)

plt.fill_between([int(aes_mean - 3*aes_std), int(aes_mean - 2*aes_std)], 0, plt.ylim()[1], color='green', alpha=0.5)
plt.fill_between([int(aes_mean + 2*aes_std), int(aes_mean + 3*aes_std)], 0, plt.ylim()[1], color='green', alpha=0.5)

plt.fill_between([math.ceil(aes_mean - 4*aes_std), int(aes_mean - 3*aes_std)], 0, plt.ylim()[1], color='cyan', alpha=0.5)
plt.fill_between([int(aes_mean + 3*aes_std), math.ceil(aes_mean + 4*aes_std)], 0, plt.ylim()[1], color='cyan', alpha=0.5)

plt.legend(custom_legend, ['1 \u03C3', '2 \u03C3', '3 \u03C3', '4 \u03C3', ''.join(['\u03C3=', "{:.4f}".format(aes_std)]), ''.join(['\u03BC=', "{:.4f}".format(aes_mean)])])

plt.show()