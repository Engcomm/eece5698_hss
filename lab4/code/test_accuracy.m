function test_accuracy(files, net)

first_is_highest = 0;
second_is_highest = 0;
third_is_highest = 0;
for i=1:length(files)
    file_found = strsplit(files(i), "/");   
    feature = accuracy_test(files(i), 15);
    feature = net(feature');
%     feature = net(accuracy_test(files(i), 15)');
    for j=1:8
        col = feature(:,j);
        
        [~, index] = sort(col, 'descend');

        if index(1) == i
            first_is_highest = first_is_highest + 1;
        elseif index(2) == i
            second_is_highest = second_is_highest + 1;
            
        elseif index(3) == i
            third_is_highest = third_is_highest + 1;
        end
    end
    fprintf("For %s, first = %d\tsecond = %d\tthird=%d\n", file_found(length(file_found)), first_is_highest, second_is_highest, third_is_highest);
    first_is_highest = 0;
    second_is_highest = 0;
    third_is_highest = 0;
    
end


end