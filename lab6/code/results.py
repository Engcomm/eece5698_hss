#!/usr/env/bin python

import sys
import os
import matplotlib.pyplot as plt
import numpy as np

with open('results.txt', 'r') as f:
    data = np.loadtxt(f, delimiter='\n')
    print(data)

fig = plt.figure()
ax = fig.add_subplot(111)

y = data / 100000
ax.set_xlabel('Number of NOP Instructions') 
ax.set_xlabel('Accuracy')
ax.plot(range(len(data)), y)
