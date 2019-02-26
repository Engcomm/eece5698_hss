function [push_peak] = get_key_strokes_threshold(file, theoretical_threshold)

threshold_found = false;
max_clicks = 100;

while ~threshold_found
    [push_peak, clicksRecognized, ~] = extractKeyStroke(file, max_clicks, theoretical_threshold, (0));
    if (clicksRecognized == max_clicks)
       threshold_found = true;
    end
    theoretical_threshold = theoretical_threshold - 1;
end


end
