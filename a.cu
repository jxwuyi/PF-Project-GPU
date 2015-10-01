#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/scan.h>

#include<iostream>
using namespace std;

int main() {
  int N=5;
  thrust::host_vector<int> A(N);
  for(int i=0;i<N;++i) A[i]=i*i;
  
  thrust::device_vector<int> B=A;
  thrust::inclusive_scan(B.begin(), B.end(), B.begin());
  
  thrust::host_vector<int> C=B;
  for(int i=0;i<A.size();++i)cout<<A[i]<<", ";cout<<endl;
  for(int i=0;i<C.size();++i)cout<<C[i]<<", ";cout<<endl;
}
