function [files, test_files, secret_files] = setup_files()

files_list = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
secret_files_list = {'secret0', 'secret1', 'secret2'};

files = strings([1, 26]);
test_files = strings([1, 26]);
secret_files = strings([1, 3]);

subdir = 'data/';
test_file = '-test';
file_type = '.wav';


for n = 1 : length(files)
    files(n) = fullfile(pwd, subdir, strcat(files_list(n), file_type));
    test_files(n) = fullfile(pwd, subdir, strcat(files_list(n), test_file, file_type));
end


for n = 1 : length(secret_files)
   secret_files(n) = fullfile(pwd, subdir, strcat(secret_files_list(n), file_type)); 
end

end

