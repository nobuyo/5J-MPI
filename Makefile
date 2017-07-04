CC := mpicc

%: %.c
	$(CC) $< -o $@ -lm