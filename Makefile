all: a b

a: a.o
	nvcc -o a a.o -lcurand
b: b.o
	nvcc -o b b.o -lcurand

a.o: a.cu
	nvcc -c -O3 -arch=sm_30 a.cu

b.o: b.cu
	nvcc -c -O3 -std=c++11 -arch=sm_30 b.cu

clean:
	rm *.o a b

