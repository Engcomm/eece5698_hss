#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np 
import pandas as pd 
import os
import sys
from scipy.stats import pearsonr
#k0 = 20 / 19 
#k15 = 198 / 197

byte_index_map = {
    0:0, 
    1:13, 
    2:10, 
    3:7, 
    4:4, 
    5:1, 
    6:14, 
    7:11, 
    8:8, 
    9:5, 
    10:2, 
    11:15, 
    12:12, 
    13:9, 
    14:6, 
    15:3
}


sbox_inv = [82, 9, 106, 213, 48, 54, 165, 56, 191, 64, 163, 158, 129, 243, 215, 251,
            124, 227, 57, 130, 155, 47, 255, 135, 52, 142, 67, 68, 196, 222, 233, 203,
            84, 123, 148, 50, 166, 194, 35, 61, 238, 76, 149, 11, 66, 250, 195, 78,
            8, 46, 161, 102, 40, 217, 36, 178, 118, 91, 162, 73, 109, 139, 209, 37,
            114, 248, 246, 100, 134, 104, 152, 22, 212, 164, 92, 204, 93, 101, 182, 146,
            108, 112, 72, 80, 253, 237, 185, 218, 94, 21, 70, 87, 167, 141, 157, 132,
            144, 216, 171, 0, 140, 188, 211, 10, 247, 228, 88, 5, 184, 179, 69, 6,
            208, 44, 30, 143, 202, 63, 15, 2, 193, 175, 189, 3, 1, 19, 138, 107,
            58, 145, 17, 65, 79, 103, 220, 234, 151, 242, 207, 206, 240, 180, 230, 115,
            150, 172, 116, 34, 231, 173, 53, 133, 226, 249, 55, 232, 28, 117, 223, 110,
            71, 241, 26, 113, 29, 41, 197, 137, 111, 183, 98, 14, 170, 24, 190, 27,
            252, 86, 62, 75, 198, 210, 121, 32, 154, 219, 192, 254, 120, 205, 90, 244,
            31, 221, 168, 51, 136, 7, 199, 49, 177, 18, 16, 89, 39, 128, 236, 95,
            96, 81, 127, 169, 25, 181, 74, 13, 45, 229, 122, 159, 147, 201, 156, 239,
            160, 224, 59, 77, 174, 42, 245, 176, 200, 235, 187, 60, 131, 83, 153, 97,
            23, 43, 4, 126, 186, 119, 214, 38, 225, 105, 20, 99, 85, 33, 12, 125]


def hamming_weight(num):
    num_ones = 0
    while num != 0:
        num_ones += num & 1
        num >>= 1
    return num_ones

def get_data(file_selection):
    # if file_out=None:
    #     file_out = ''.join([file_selection.split('.')[0], '_cache.h5']
    data_arr = pd.read_csv(file_selection, sep=' ', header=None).values
    return data_arr

def find_data_file():
    files = [f for f in os.listdir(os.getcwd()) if os.path.isfile(f)]
    print('Current files in %s:' % subdir)
    print("-------------------------------------------------------------")
    for i in range(0, len(files)):
        print("{}. {}".format(i + 1, files[i]))
    print("-------------------------------------------------------------")
    try:
        file_num = int(input("Enter the number representing the file you want to read from: "))
        if file_num > len(files) or file_num < 0:
            file_num = -1
            raise ValueError
    except ValueError:
        print("Entered number is invalid.")
        while 0 < file_num <= len(files):
            file_num = int(input("Enter the number representing the file you want to read from: "))
    file = files[file_num - 1]
    path_to_file = os.path.normpath(os.path.join(path, file))
    return path_to_file

def normalize_data(data):
    min_data = min(data)
    data = [d - min_data for d in data]
    return data

def plot_one_row(data):
    data = normalize_data(data)
    fig = plt.figure(1)
    ax = fig.add_subplot(111)
    ax.set_title("Normalized Power Trace Data for one row.")
    ax.set_xlabel("Sample Number")
    ax.set_ylabel("Power Trace output")
    sample_numbers = np.linspace(1, len(data), len(data), dtype=np.int16)
    ax.plot(sample_numbers, data)

#find sample @ t=2669 for x number of samples
#compare this against hamming weight

#Get inv sbox data for cipher byte val and guessed key val for each run
#Given a key index, check the cipher byte at the shifted row and find the hamming distance
#return a list of hamming weight data for each key byte in [0, 255]
def get_hamming_weight_data(cipher_data, key):
    #Test the first byte for now
    shift_cipher_byte = byte_index_map[key]
    regular_cipher_byte =key
    hamming_weight_vals = []
    #HW_n = S^-1(C_shiftcipherbyte XOR Kn) XOR C_regularcipherbyte for n = 0..255
    for sample in cipher_data:
        shift_cipher_byte_val = sample[shift_cipher_byte]
        regular_cipher_byte_val = sample[regular_cipher_byte]
        hw_sample_data = []
        # print(shift_cipher_byte_val)
        for key in range(0, 256):
            sbox_inv_val = sbox_inv[int(shift_cipher_byte_val ^ key)]
            hw_data = sbox_inv_val ^ regular_cipher_byte_val
            hw_val = hamming_weight(hw_data)
            hw_sample_data.append(hw_val)
        hamming_weight_vals.append(hw_sample_data)
    return hamming_weight_vals


#Find pearson correlation between samples @ t=2669 
def get_pearson_data(power_trace, hamming_weight_vals, num_traces=5000):
    #For each possible key byte get an array of (Power_trace[i][2668]) and (Hamming_weight_vals[i][j])
    power_trace_leaky = power_trace[0:num_traces,2668]
    # print("Leaky power trace is: ")
    # print(power_trace_leaky)
    # print("Leaky Power trace /length is %s " % len(power_trace_leaky))
    pearson_correlation = []
    for key in range(0, 256):
        # print(key)
        hw_vals = [hamming_weight_vals[i][key] for i in range(0, num_traces)]
        pearson_val, _ = pearsonr(power_trace_leaky, hw_vals)
        pearson_correlation.append(pearson_val)
    print("Min is {} with index {}".format(min(pearson_correlation),pearson_correlation.index(min(pearson_correlation))))
    return pearson_correlation        


if __name__ == '__main__':
    #TODO: Figure out if caching data is necessary 
        #Speeds up execution time
    print(byte_index_map)
    subdir = "data"
    path = os.path.normpath(os.path.join(os.getcwd(), subdir))
    os.chdir(path)

    print("Select power trace data to read from: ")

    file_selection = find_data_file()
    # print(file_selection)
    with open(file_selection) as f:
        data_output = get_data(f)
    plot_one_row(data_output[0])

    cipher_data_file = find_data_file()
    with open(cipher_data_file) as f:
        cipher_data = get_data(cipher_data_file)
    print(len(cipher_data[0]))
    pearson_data = []
    for key in range(0, 16):
        weights = get_hamming_weight_data(cipher_data, key)

        pearson_key = get_pearson_data(data_output, weights)
        pearson_data.append(pearson_key)
    plt.show()