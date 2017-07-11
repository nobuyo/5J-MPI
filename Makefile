CC := mpicc

%: %.c
	$(CC) $< -o $@ -lm

pi:
	$(CC) $< -o $@ -lm -O3