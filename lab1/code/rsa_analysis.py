# no one EVER look at this okay PLEASE you're not allowed
import numpy as np 
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import math

rsa_file = "rsa.txt"

with open(rsa_file, 'r') as rsa:
    rsa_lines = rsa.readlines()
    rsa_data = [int(rsa_line.split()[0]) for rsa_line in rsa_lines]

rsa_len_before = len(rsa_data)
rsa_data = [x for x in rsa_data if x < 500000]
num_removed = rsa_len_before - len(rsa_data)
print(num_removed)

rsa_mean = np.mean(rsa_data, dtype = np.float64)
print("rsa mean is: {}\n".format(rsa_mean))

rsa_std = np.std(rsa_data, dtype=np.float64)
print("rsa \mu is: {}\n".format(rsa_std))

num_outside_four_sigma = len([x for x in rsa_data if x > (rsa_mean + 4*rsa_std)])
num_outside_four_sigma += num_removed
percent_outside_four_sigma = 100 * (num_outside_four_sigma / float(rsa_len_before))

print(percent_outside_four_sigma)

custom_legend = [Line2D([0], [0], color='blue', lw=10),
                Line2D([0], [0], color='magenta', lw=10),
                Line2D([0], [0], color='green', lw=10),
                Line2D([0], [0], color='cyan', lw=10),
                Line2D([0], [0], color='white', lw=0),
                Line2D([0], [0], color='white', lw=0)]

plt.rcParams.update({'font.size': 22})
plt.xlim(math.ceil(rsa_mean - 4 * rsa_std), math.ceil(rsa_mean + 4*rsa_std))

plt.grid(axis='y', color='black', alpha=0.5)
plt.title('Distribution of RSA Encryption time in number of clock cycles')
n, bins, patches = plt.hist(rsa_data, color='red', edgecolor='black', bins=int(round(len(rsa_data) ** (1 /3))))
plt.xlabel('RSA encryption time (Clock cycles)')
plt.ylabel('Counts')
plt.ylim(0, n.max() + 500)
plt.fill_between([int(rsa_mean - rsa_std), int(rsa_mean + rsa_std)], 0, plt.ylim()[1], color = 'blue', alpha=0.5)

plt.fill_between([int(rsa_mean - 2*rsa_std), int(rsa_mean - rsa_std)], 0, plt.ylim()[1], color='magenta', alpha=0.5)
plt.fill_between([int(rsa_mean + rsa_std), int(rsa_mean + 2*rsa_std)], 0, plt.ylim()[1], color='magenta', alpha=0.5)

plt.fill_between([int(rsa_mean - 3*rsa_std), int(rsa_mean - 2*rsa_std)], 0, plt.ylim()[1], color='green', alpha=0.5)
plt.fill_between([int(rsa_mean + 2*rsa_std), int(rsa_mean + 3*rsa_std)], 0, plt.ylim()[1], color='green', alpha=0.5)

plt.fill_between([math.ceil(rsa_mean - 4*rsa_std), int(rsa_mean - 3*rsa_std)], 0, plt.ylim()[1], color='cyan', alpha=0.5)
plt.fill_between([int(rsa_mean + 3*rsa_std), math.ceil(rsa_mean + 4*rsa_std)], 0, plt.ylim()[1], color='cyan', alpha=0.5)

plt.legend(custom_legend, ['1 \u03C3', '2 \u03C3', '3 \u03C3', '4 \u03C3', ''.join(['\u03C3=', "{:.4f}".format(rsa_std)]), ''.join(['\u03BC=', "{:.4f}".format(rsa_mean)])])

plt.show()