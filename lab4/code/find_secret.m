function find_secret(secret_files, net)

chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

out_1 = "";
out_2 = "";
for i=1:length(secret_files)
    file_found = strsplit(secret_files(i), "/");   

    fprintf("Examining file %s...", file_found(length(file_found))) 
    feature = find_secret_feature(secret_files(i), 15);
    feature_net = net(feature');
    
    for j=1:size(feature_net, 2)
        col = feature_net(:,j);
        [~, index_order] = sort(col, 'descend');
        idx1 = index_order(1);
        idx2 = index_order(2);
        char1 = chars(idx1);
        char2 = chars(idx2);
        out_1 = strcat(out_1, char1, " ");
        out_2 = strcat(out_2, char2, " ");
%         [~, idx] = max(col);
%         col(idx) = NaN;
%         
    end
    fprintf('\n\n')
    disp(out_1)
    fprintf('\n')
    disp(out_2)
    fprintf('\n')
    out_1 = "";
    out_2 = "";
end

end
