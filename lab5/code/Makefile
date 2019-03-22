CC  = gcc
LIB = openssl/libcrypto.so.3
CFLAGS = -Iopenssl/includes -Lopenssl/
LFLAGS = -lcrypto
samples = 1000000
threshold = 175

te0 = 1d2fa0
te1 = 1d2ba0
te2 = 1d27a0
te3 = 1d23a0

.PHONY: all run binaries analyze_data plot_data

all: binaries setup collect_data analyze_data recover_key #plot_data

run: collect_data 

profile: system_profile plot_profile
 

binaries: victim attacker analysis system_profile key_recover

victim: victim.c
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS)

attacker: attacker.c
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS)

analysis: analysis.c
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS)

system_profile: system_profile.c 
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS)

key_recover: key_recover.c 
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS)

setup:
	$(shell export LD_LIBRARY_PATH=/home/arjun/Documents/eece5698_hss/lab5/code/openssl:$LD_LIBRARY_PATH)


collect_data:
	./victim & 
	./attacker -t data/timing_te0.txt -c data/cipher_te0.txt -p data/plain_te0.txt -n $(samples) -o $(te0) -v $(LIB)
	./attacker -t data/timing_te1.txt -c data/cipher_te1.txt -p data/plain_te1.txt -n $(samples) -o $(te1) -v $(LIB)
	./attacker -t data/timing_te2.txt -c data/cipher_te2.txt -p data/plain_te2.txt -n $(samples) -o $(te2) -v $(LIB)
	./attacker -t data/timing_te3.txt -c data/cipher_te3.txt -p data/plain_te3.txt -n $(samples) -o $(te3) -v $(LIB)

analyze_data:
	./analysis -t data/timing_te0.txt -c data/cipher_te0.txt -o data/result_te0.txt -s $(threshold) -r data/cipher_threshold_te0.txt
	./analysis -t data/timing_te1.txt -c data/cipher_te1.txt -o data/result_te1.txt -s $(threshold) -r data/cipher_threshold_te1.txt
	./analysis -t data/timing_te2.txt -c data/cipher_te2.txt -o data/result_te2.txt -s $(threshold) -r data/cipher_threshold_te2.txt
	./analysis -t data/timing_te3.txt -c data/cipher_te3.txt -o data/result_te3.txt -s $(threshold) -r data/cipher_threshold_te3.txt

recover_key:
	./key_recover -c data/cipher_threshold_te0.txt -n 0
	./key_recover -c data/cipher_threshold_te1.txt -n 1
	./key_recover -c data/cipher_threshold_te2.txt -n 2
	./key_recover -c data/cipher_threshold_te3.txt -n 3



plot_data:
	python plot.py data/result_te0.txt &
	python plot.py data/result_te1.txt &
	python plot.py data/result_te2.txt &
	python plot.py data/result_te3.txt

plot_profile:
	./system_profile 
	python plot_profile.py timing_data_cache_hit timing_data_cache_miss

.PHONY: clean
clean:	
	rm -f victim attacker analysis system_profile

clean_data:
	rm -f data/* timing_data_cache_hit timing_data_cache_miss