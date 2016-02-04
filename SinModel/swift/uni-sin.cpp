#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <numeric>
#include <string>
#include <memory>
#include <functional>
#include <utility>
#include "random/Bernoulli.cpp"
#include "random/Beta.cpp"
#include "random/Binomial.cpp"
#include "random/BooleanDistrib.cpp"
#include "random/Categorical.cpp"
#include "random/Gaussian.cpp"
#include "random/Geometric.cpp"
#include "random/Poisson.cpp"
#include "random/UniformChoice.cpp"
#include "random/UniformInt.cpp"
#include "random/UniformReal.cpp"
#include "util/Hist.h"
#include "util/util.h"
#include "util/DynamicTable.h"

using namespace std;
using namespace swift::random;


int main();

namespace swift {

  class BLOG_Static_State;
  class BLOG_Temporal_State;

  const int _ParticleN_ = 5000;
  const int _TimeLim_ = 5000;
  const int _DependN_ = 2;

  class BLOG_Static_State {
  public:
    void init();
    double __value_theta;
    int __mark_theta;
  };
  class BLOG_Temporal_State {
  public:
    void init();
    void _clear_marks();
    double __value_X;
    int __mark_X;
    double __value_Y;
    int __mark_Y;
  };
  BLOG_Static_State _stat_memo[_ParticleN_];
  BLOG_Static_State _ptr_stat_memo[_ParticleN_];
  BLOG_Temporal_State _temp_memo[_ParticleN_][_DependN_];
  BLOG_Temporal_State* _ptr_temp_memo[_ParticleN_][_DependN_];
  BLOG_Temporal_State* _ptr_backup[_ParticleN_][_DependN_];
  void init();
  void debug();
  int _cur_loop;
  unsigned _cur_time;
  default_random_engine __random_device;
  const double __fixed_sigma2 = 1;
  double __get_theta();
  UniformReal UniformReal6476568;
  double __likeli_theta();
  void __set_theta(double);
  double __get_X(unsigned);
  Gaussian Gaussian6477560;
  Gaussian Gaussian6479144;
  double __likeli_X(unsigned);
  void __set_X(unsigned, double);
  double __get_Y(unsigned);
  Gaussian Gaussian6476920;
  double __likeli_Y(unsigned);
  void __set_Y(unsigned, double);
  bool _set_evidence(double&);
  std::function<bool(double&)> _evidenceTable[_TimeLim_ + 1];
  void setup_temporal_evidences();
  void sample();
  double _weight[_ParticleN_];
  inline void _evaluate_query(double);
  inline void print();
  std::function<void(double)> _queryTable[_TimeLim_ + 1];
  std::function<void(void)> _printTable[_TimeLim_ + 1];
  void setup_temporal_queries();
  void setup_temporal_prints();
  Hist<double> _answer_0 = Hist<double>(true);

  void BLOG_Static_State::init() {
  }
  void BLOG_Temporal_State::init()                                                                                                                                                                                                         {
}
void BLOG_Temporal_State::_clear_marks()
{}
void init()
{
  UniformReal6476568.init(-1,1);
  Gaussian6477560.init(0,__fixed_sigma2);
  setup_temporal_evidences();
  setup_temporal_queries();
  setup_temporal_prints();
}
void debug()
{}
double __get_theta()
{
  BLOG_Static_State& _cur_state = _stat_memo[_cur_loop];
  double& __retvalue = _cur_state.__value_theta;
  int& __mark = _cur_state.__mark_theta;
  if (__mark>0)
    return __retvalue;
  __retvalue=UniformReal6476568.gen();
  __mark=1;
  return __retvalue;
}
double __likeli_theta()
{
  BLOG_Static_State& _cur_state = _stat_memo[_cur_loop];
  double& __retvalue = _cur_state.__value_theta;
  double __local_weight = 0.000000;
  __local_weight=UniformReal6476568.loglikeli(__retvalue);
  return __local_weight;
}
void __set_theta(double __retvalue_arg)
{
  BLOG_Static_State& _cur_state = _stat_memo[_cur_loop];
  _cur_state.__value_theta=__retvalue_arg;
  int& __mark = _cur_state.__mark_theta;
  __mark=1;
}
double __get_X(unsigned t)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  double& __retvalue = _cur_state.__value_X;
  int& __mark = _cur_state.__mark_X;
  if (__mark==t+1)
    return __retvalue;
  if (t==0)
    __retvalue=Gaussian6477560.gen();
  else
    __retvalue = (Gaussian6479144.init(sin(__get_theta()*__get_X(prev(t, 1))), 4), Gaussian6479144.gen());
  __mark=t+1;
  return __retvalue;
}
double __likeli_X(unsigned t)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  double& __retvalue = _cur_state.__value_X;
  double __local_weight = 0.000000;
  if (t==0)
    __local_weight=Gaussian6477560.loglikeli(__retvalue);
  else
    __local_weight = (Gaussian6479144.init(sin(__get_theta()*__get_X(prev(t, 1))), 4), Gaussian6479144.loglikeli(__retvalue));
  return __local_weight;
}
void __set_X(unsigned t, double __retvalue_arg)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  _cur_state.__value_X=__retvalue_arg;
  int& __mark = _cur_state.__mark_X;
  __mark=t+1;
}
double __get_Y(unsigned t)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  double& __retvalue = _cur_state.__value_Y;
  int& __mark = _cur_state.__mark_Y;
  if (__mark==t+1)
    return __retvalue;
  __retvalue=(Gaussian6476920.init(__get_X(t),0.5*0.5),Gaussian6476920.gen());
  __mark=t+1;
  return __retvalue;
}
double __likeli_Y(unsigned t)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  double& __retvalue = _cur_state.__value_Y;
  double __local_weight = 0.000000;
  __local_weight = (Gaussian6476920.init(__get_X(t), 0.5*0.5), Gaussian6476920.loglikeli(__retvalue));
  return __local_weight;
}
void __set_Y(unsigned t, double __retvalue_arg)
{
  BLOG_Temporal_State& _cur_state = *_ptr_temp_memo[_cur_loop][t%_DependN_];
  _cur_state.__value_Y=__retvalue_arg;
  int& __mark = _cur_state.__mark_Y;
  __mark=t+1;
}

void sample()
{
  double __local_weight;
  init();
  for (_cur_loop=0;_cur_loop<_ParticleN_;_cur_loop++)
  _stat_memo[_cur_loop].init();

  for (_cur_time=0;_cur_time<=_TimeLim_;_cur_time++)
  {
    int _tmp_idx = _cur_time%_DependN_;
    pointer_copy<_ParticleN_,_DependN_>(_tmp_idx,_ptr_temp_memo,_temp_memo);
    for (_cur_loop=0;_cur_loop<_ParticleN_;_cur_loop++)
    {
      if (_set_evidence(__local_weight))
        _evaluate_query(__local_weight);
      _weight[_cur_loop]=__local_weight;
    }

    print();
    normalizeLogWeights<_ParticleN_>(_weight);
    resample<_ParticleN_,_DependN_>(_weight,_stat_memo,_ptr_stat_memo,_ptr_temp_memo,_ptr_backup);
    //__perturb(theta);
  }

}
inline void _evaluate_query(double __local_weight)
{
  if (_queryTable[_cur_time])
    _queryTable[_cur_time](__local_weight);
}
inline void print()
{
  if (_printTable[_cur_time])
    {
      printf("++++++++++++++++ TimeStep @%d ++++++++++++++++\n",_cur_time);
      _printTable[_cur_time]();
    }
}
void setup_temporal_queries()
{
  _queryTable[_TimeLim_] = [&](double __local_weight) {
    _answer_0.add(__get_theta(),__local_weight);
  };
}
void setup_temporal_prints()
{
  _printTable[_TimeLim_] = [&]() {
    _answer_0.print("theta");
  };
}


vector<double> data_vec;

bool _set_evidence(double& __local_weight) {
  __local_weight = 0.000000;
  __set_Y(_cur_time, data_vec[_cur_time]);
  __local_weight += __likeli_Y(_cur_time);
  return true;
}

void setup_temporal_evidences() {
}

void generate_data() {
  double sigma = 2,
    sigma_obs = 0.5,
    sigma_x0 = 1,
    sigma_prior = 1;
  data_vec.clear();

  default_random_engine engine;
  normal_distribution<double> randn(0, 1);

  double prev_x = sigma_x0 * randn(engine);

  data_vec.push_back(prev_x + randn(engine) * sigma_obs);

  double a = -0.5;

  for (int i = 1; i <= _TimeLim_; ++i) {
    double x = sin(a*prev_x) + sigma*randn(engine);
    data_vec.push_back(x + randn(engine) * sigma_obs);
    prev_x = x;
  }
}

}
int main()
{
  cerr << "Generating Data ..." ;
  swift::generate_data();
  cerr << "  OK!" << endl;

  std::chrono::time_point<std::chrono::system_clock> __start_time = std::chrono::system_clock::now();
  swift::sample();
  std::chrono::duration<double> __elapsed_seconds = std::chrono::system_clock::now()-__start_time;
  printf("\nrunning time: %fs\n",__elapsed_seconds.count());

  //system("pause");
}

