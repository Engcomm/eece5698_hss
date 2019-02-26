function [feature] = accuracy_test(file, theoretical_threshold)

threshold_found = false;
max_clicks = 8;
file_found = strsplit(file, "/");   

while ~threshold_found
    [feature, clicksRecognized, ~] = extractKeyStroke(file, max_clicks, theoretical_threshold, (0));
    if (clicksRecognized == max_clicks)
       threshold_found = true;
%        fprintf("Threshold found for %s with value %d\n", file_found(length(file_found)), theoretical_threshold);
    end
    theoretical_threshold = theoretical_threshold - 1;
end

feature = abs(fft(feature'))';

end
