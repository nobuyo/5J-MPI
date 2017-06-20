CC := mpicc

%: %.c
	$(CC) $< -o $@