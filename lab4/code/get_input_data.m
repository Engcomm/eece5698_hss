function [nn_input, nn_target] = get_input_data(num_keys_training, files)

threshold = 15;

input = [];
for i=1:length(files)
    key = files(i);
    file = strsplit(key, "/");
    fprintf("Extracting Key Stroke for %s\n", file(length(file)));
    push_peak = get_key_strokes_threshold(key, threshold);
    %{
        Push_peak represents a 100x441 length matrix, where each column 
        represents the sample from a specific keypress.
        If the NN wants to read all the samples for the letter 'a', it will
        check the block from [1:441, 1:100] with the rows being the
        specific sample data, and the columns being the index of the
        specific key press within the sequence of 100.
    %}
    features = abs(fft(push_peak'))';
    input = [input, features'];
end

nn_input = input;
nn_target = zeros(length(files), length(files) * num_keys_training);

for i=1:length(files)
    nn_target(i, (i-1)*num_keys_training+1:(i-1)*num_keys_training+num_keys_training) = ones(1, num_keys_training);
end

end