all: a

a: a.o
	nvcc -o a a.o

a.o: a.cu
	nvcc -c -O3 -arch=sm_30 a.cu

clean:
	rm *.o a

