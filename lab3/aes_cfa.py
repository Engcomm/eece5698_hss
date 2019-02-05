import matplotlib.pyplot as plt
import numpy as np 
import os
import sys


def find_data_file():
    subdir = "data"
    path = os.path.normpath(os.path.join(os.getcwd(), subdir))
    os.chdir(path)
    files = [f for f in os.listdir(os.getcwd()) if os.path.isfile(f)]
    print('Current files:')
    print("-------------------------------------------------------------")
    [print("{}. {}".format(f+1, files[f])) for f in enumerate(files)]
    # for i in range(0, len(files)):
    #     print("{}. {}".format(i + 1, files[i]))
    print("-------------------------------------------------------------")


    return 0
    




if __name__ == '__main__':
    selection = find_data_file()
