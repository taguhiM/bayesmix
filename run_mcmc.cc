#include <iostream>

#include "src/includes.h"

int main() {
  LapNIGHierarchy H{};
  H.hypers = std::make_shared<LapNIG::Hyperparams>();
  H.state.mean = 1;
  H.state.scale = 1;
  H.hypers->mean = 0;
  H.hypers->var = 1;
  H.hypers->shape = 2;
  H.hypers->scale = 1;
  H.hypers->mh_mean_var = 3;
  H.hypers->mh_log_scale_var = 1;

  auto &RNG = bayesmix::Rng::Instance().get();

  for (int Id = 0; Id < 100; ++Id) {
    Eigen::RowVectorXd v(1);
    v << stan::math::double_exponential_rng(0, 2, RNG);
    H.add_datum(Id, v);
  }

  std::ofstream trace_plot_file;
  trace_plot_file.open("./trace.csv");

  unsigned int ITER = 10000;
  for (unsigned int j = 0; j < ITER; ++j) {
    H.sample_full_cond();
    trace_plot_file << H.get_state().mean << "," << H.get_state().scale
                    << std::endl;
  }

  std::cout << (double)LapNIGHierarchy::accepted_ / LapNIGHierarchy::iter_
            << std::endl;
  trace_plot_file.close();

  return 0;
}
