CC := mpicc

%: %.c
	$(CC) $< -o $@ -lm -O3