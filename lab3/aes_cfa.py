#!/usr/bin/python
import matplotlib.pyplot as plt
import numpy as np 
import pandas as pd 
import os
import sys
from scipy.stats import pearsonr

TRACES = 5000

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

#Represents a lookup table of the hamming weight values for all byte values (0 - 255)
#Was created by running the "hamming_weight" function on all values from 0 - 255
#Having a lookup table significantly decreases the execution time for the CPA attack
hamming_weight_vals = [ 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
                        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
                        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
                        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
                        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
                        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
                        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 ]

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

#Gets data from a certain file and creates a .npy binary to 
def get_data(file_selection, create_npy, num_traces):
    if create_npy:
        file_read = file_selection.split(".")[0] + '.txt'
        with open(file_read) as f:
            data_arr = np.mat([map(int, line.split()) for line in f])
        np.save(file_selection, data_arr)
    else:
        data_arr = np.load(file_selection)
    return data_arr[0:num_traces]


def plot_one_row(data):
    print("Plotting one power trace...")
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
    regular_cipher_byte = key
    hamming_weights = []    
    #HW_n = S^-1(C_shiftcipherbyte XOR Kn) XOR C_regularcipherbyte for n = 0..255
    for sample in cipher_data:
        shift_cipher_byte_val = sample[shift_cipher_byte]
        regular_cipher_byte_val = sample[regular_cipher_byte]
        hw_sample_data = []
        # print(shift_cipher_byte_val)
        for key in range(0, 256):
            sbox_inv_val = sbox_inv[shift_cipher_byte_val ^ key]
            hw_data = sbox_inv_val ^ regular_cipher_byte_val
            hw_val = hamming_weight_vals[hw_data]
            hw_sample_data.append(hw_val)
        hamming_weights.append(hw_sample_data)
    return hamming_weights

#Find pearson correlation between samples @ t=2669 
def get_pearson_data(power_trace, hamming_weight_vals, num_traces=TRACES):
    #For each possible key byte get an array of (Power_trace[i][2668]) and (Hamming_weight_vals[i][j])
    power_trace_leaky = power_trace[0:num_traces,2668]
    pearson_correlation = []
    for key in range(0, 256):
        hw_vals = [hamming_weight_vals[i][key] for i in range(0, num_traces)]
        pearson_val, _ = pearsonr(power_trace_leaky, hw_vals)
        pearson_correlation.append(pearson_val)
    key_byte_val = pearson_correlation.index(min(pearson_correlation))
    return key_byte_val, pearson_correlation        


def get_trace_data(expression, num_traces=TRACES):
    subdir = "data"
    try:
        #data/ added to the file structure in order to properly parse in the get_data() method
        files = [subdir + "/" + f for f in os.listdir(subdir)]
    except OSError:
        #error raises on either data folder being empty or folder not existing.
        print("No Data to Read From. Please create \"data\" folder with files *trace_int.txt and *cipher.txt")
        sys.exit(0)
    for f in files:
        if expression in f:
            found_file = f
            break
    potential_npy = found_file.split('.')[0] + '.npy'
    create_npy = False
    print("Getting Trace Data for %s ..." % found_file.split('.')[0])
    if potential_npy not in files:
        print("Caching %s data in .npy file for easier future use." % found_file)
        create_npy = True
    return get_data(potential_npy, create_npy, num_traces)

#Retreives the key byte by getting the pearson correlation data for a specific cipherbyte and power trace column
def retreive_key_bytes(power_trace_data, cipher_data, num_traces=TRACES):
    found_key_bytes = np.zeros(shape=16, dtype=np.int16)
    correlation_data = np.zeros(shape=(16, 256), dtype=np.float64)
    for key in range(0, len(found_key_bytes)):
        key_byte, pearson_data = get_pearson_data(power_trace_data, get_hamming_weight_data(cipher_data, key))
        index_to_insert = byte_index_map[key]
        found_key_bytes[index_to_insert] = key_byte
        correlation_data[index_to_insert] = pearson_data
    print("Found {} keys from analyzing correlation...".format(len(found_key_bytes)))
    print("Retreived key is: {}".format(found_key_bytes))
    return found_key_bytes, correlation_data


#Plots the Pearsons correlation data 
def plot_correlation_data(key_bytes, correlation_data):
    print("Plotting all correlation data...")
    fig,axes = plt.subplots(4, 4, sharex=True, sharey=True)
    fig.text(0.5, 0.04, 'All Possible Key Values', ha='center', fontsize=26)
    fig.text(0.04, 0.5, 'Pearson Correlation Coefficient', va='center', rotation='vertical', fontsize=26)
    plot_num = 0

    x_vals = np.arange(start=0, stop=256, step=1, dtype=int)
    for i in range(0, len(axes)):
        for j in range(0, len(axes[0])):
            title_string = "Key Byte #{} with value {}".format(plot_num+1, key_bytes[plot_num])
            axes[i,j].set_title(title_string)
            axes[i,j].set_xlim((0, 256))
            axes[i,j].plot(x_vals, correlation_data[plot_num])
            plot_num += 1


if __name__ == '__main__':
    plt.rcParams.update({'font.size': 18})

    power_trace_data = get_trace_data(expression="trace_int.txt")
    cipher_data = get_trace_data(expression="cipher.txt")

    plot_one_row(power_trace_data[0])
    
    key_bytes, correlation_data = retreive_key_bytes(power_trace_data, cipher_data)
    plot_correlation_data(key_bytes, correlation_data)
    plt.show()