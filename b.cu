#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sort.h>
#include <thrust/scan.h>
#include <thrust/transform.h>
#include <thrust/functional.h>

#include <iostream>
#include <random>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <chrono>
using namespace std;

#include <cuda.h>
#include <curand.h>
#include <math.h>

const int MaxT = 500;
const int MaxPart = 500000;
const int N = MaxPart;
const double sig_x = 2;
const double sig_obs = 0.5;

double randn() {
  static default_random_engine engine;
  static normal_distribution<double> dist(0, 1);
  return dist(engine);
}

struct trans {
  double a;
  trans(double a):a(a){};
  __host__ __device__
  double operator()(double x, double y) {
    return sin(a * x) + y;
  }
};

struct likeli {
  double y, coef;
  likeli(double y, double sig_obs):y(y),coef(-1.0/(2*sig_obs*sig_obs)){};
  __host__ __device__
  double operator()(double x) {
    // cpd y ~ N(x, sig_obs)
    //    ignore coefficient 1/(sigma * sqrt(2 PI))
    return exp(coef * (x - y) * (x - y));
  }
};

struct resample {
  double sum;
  double* W;
  double* X;
  int N;
  resample(double sum, double *W, double* X, int N)
    :sum(sum),W(W),X(X),N(N){};
  __host__ __device__
  double operator()(double t) {
    t = t * sum;
    int lo = 0, hi = N, mid;
    while(lo + 1 < hi) {
      mid = lo + hi >> 1;
      if (W[mid - 1] >= t) hi = mid;
      else lo = mid;
    }
    return X[hi - 1];
  }
};

int main() {

  double x, y, cur_x, a = -0.5;
  thrust::device_vector<double> X(N), T(N), Z(N), Y(N), W(N); 
  double* X_ptr = thrust::raw_pointer_cast(X.data());
  double* T_ptr = thrust::raw_pointer_cast(T.data());
  double* Z_ptr = thrust::raw_pointer_cast(Z.data());
  double* W_ptr = thrust::raw_pointer_cast(W.data()); 
  
  curandGenerator_t gen;
  curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);

  int start, end;
  double duration;
  start = clock();
  // generate data
  thrust::host_vector<double> data(MaxT+1);
  for(int t=0; t<=MaxT; ++ t) {
    if (t == 0) cur_x = randn();
    else cur_x = sin(x * a) + randn() * sig_x;
    x = cur_x; y = cur_x + sig_obs * randn();
    data[t] = y;
  }
  end = clock();
  duration = (end - start) * 1.0 / CLOCKS_PER_SEC; 
  printf("Data Generation : %.6lfs\n", duration);


  start = clock();
  for(int t = 0; t <= MaxT; ++ t) {
    // fetch data
    y = data[t];
    // Particle Filter
    if (t == 0) {
      // initial value
      curandGenerateNormalDouble(gen, X_ptr, N, 0, 1);
    } else {
      // transition
      // x[t] = N(sin(a * x[t-1]), 2)
      curandGenerateNormalDouble(gen, Z_ptr, N, 0, 2);
      thrust::transform(X.begin(), X.end(), Z.begin(), X.begin(), trans(a));
    }
  
    // Calc Likelihood
    thrust::transform(X.begin(), X.end(), W.begin(), likeli(y, sig_obs));

    // resample
    thrust::inclusive_scan(W.begin(), W.end(), W.begin());

    double tot_sum = W[N - 1];
    curandGenerateUniformDouble(gen, Z_ptr, N);
    thrust::transform(Z.begin(), Z.end(), Y.begin(),
                      resample(tot_sum, W_ptr, X_ptr, N));
    thrust::copy(Y.begin(), Y.end(), X.begin());
  
  }

  end = clock();
  duration = 1.0 * (end - start) / CLOCKS_PER_SEC;
  printf("Time Elapsed: %.6lfds\n", duration);

}
