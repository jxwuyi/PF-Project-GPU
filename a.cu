#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sort.h>
#include <thrust/scan.h>

#include <iostream>
using namespace std;

#include <cuda.h>
#include <curand.h>

int main() {
  int N = 6;

  thrust::host_vector<int> A(N);
  for(int i=0;i<N;++i)A[i]=i*i;

  thrust::device_vector<int> B = A;
  thrust::inclusive_scan(B.begin(), B.end(), B.begin());

  thrust::host_vector<int> C = B;
  for(int i = 0; i<A.size();++i) cout << A[i] << " "; cout << endl;
  for(int i=0;i<C.size();++i) cout << C[i] << " "; cout << endl;

  thrust::device_vector<double> D(N);
  double* D_ptr = thrust::raw_pointer_cast(D.data());
  curandGenerator_t gen;
  curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
  curandGenerateUniformDouble(gen, D_ptr, N);

  thrust::host_vector<double> E(N);
  thrust::copy(D.begin(), D.end(), E.begin());

  cout << "Random Gaussian : " << endl;
  for(int i = 0; i < E.size(); i++) cout << "  >> " << E[i] << endl;  

}
