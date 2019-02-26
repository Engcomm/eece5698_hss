close all;
clc;
clear;

num_keys_training = 100;
num_keys_secret = 8;
num_keys_test = 8;

sampling_freq = 44100;
file_type = '.wav';

%% First setup the files to read from.

[files, test_files, secret_files] = setup_files();

[nn_input, nn_target] = get_input_data(num_keys_training, files);



%% Train and use neural network

disp("Training Neural network...")
net = neural_net(nn_input, nn_target);

%% Find Accuracy using test files
test_accuracy(test_files, net);


%% Attack secrets
find_secret(secret_files, net);