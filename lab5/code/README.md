# Setup
If on a POSIX environment, make sure to run the following commmands.

`cd code/openssl`
`export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`

These commands add the openssl library to the shared library path so that both the victim and attacker will have access to the same library in memory. 

# Compiling the Code
Run `make binaries` to compile all the code...
All binaries are compiled using 

# System profiling
Run `make plot_profile` to run the system_profile.c file and plot the results in Python.
The timing data is stored in the files `timing_data_cache_hit` and `timing_data_cache_miss`.

Run `make plot_cache_hits` to plot the cache hits using the threshold of 20000, allowing for analysis of the leaky ciphertext data.

# Preparing the Flush + Reload Attack -- Online portion
Run `make collect_data` to collect the timing and ciphertext data from the victim.
The data for this and all following processes will be stored in the data/ subfolder.

# Offline portion
Run `make analyze_data` and `make recover_key` to come up with the result.txt and final key bytes.
Run `make plot_data` to plot the four cache line results.
