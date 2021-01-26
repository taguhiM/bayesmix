#include <gtest/gtest.h>

#include <Eigen/Dense>

#include "../src/hierarchies/lin_reg_uni_hierarchy.hpp"
#include "../src/hierarchies/nnig_hierarchy.hpp"
#include "../src/hierarchies/nnw_hierarchy.hpp"
#include "../src/utils/proto_utils.hpp"
#include "ls_state.pb.h"
#include "marginal_state.pb.h"

TEST(nnighierarchy, draw) {
  auto hier = std::make_shared<NNIGHierarchy>();
  bayesmix::NNIGPrior prior;
  double mu0 = 5.0;
  double lambda0 = 0.1;
  double alpha0 = 2.0;
  double beta0 = 2.0;
  prior.mutable_fixed_values()->set_mean(mu0);
  prior.mutable_fixed_values()->set_var_scaling(lambda0);
  prior.mutable_fixed_values()->set_shape(alpha0);
  prior.mutable_fixed_values()->set_scale(beta0);
  hier->set_prior(prior);
  hier->initialize();

  auto hier2 = hier->clone();
  hier2->draw();

  bayesmix::MarginalState out;
  bayesmix::MarginalState::ClusterState* clusval = out.add_cluster_states();
  bayesmix::MarginalState::ClusterState* clusval2 = out.add_cluster_states();
  hier->write_state_to_proto(clusval);
  hier2->write_state_to_proto(clusval2);

  ASSERT_TRUE(clusval->DebugString() != clusval2->DebugString());
}

TEST(nnighierarchy, sample_given_data) {
  auto hier = std::make_shared<NNIGHierarchy>();
  bayesmix::NNIGPrior prior;
  double mu0 = 5.0;
  double lambda0 = 0.1;
  double alpha0 = 2.0;
  double beta0 = 2.0;
  prior.mutable_fixed_values()->set_mean(mu0);
  prior.mutable_fixed_values()->set_var_scaling(lambda0);
  prior.mutable_fixed_values()->set_shape(alpha0);
  prior.mutable_fixed_values()->set_scale(beta0);
  hier->set_prior(prior);
  hier->initialize();

  Eigen::VectorXd datum(1);
  datum << 4.5;

  auto hier2 = hier->clone();
  hier2->add_datum(0, datum);
  hier2->sample_given_data();

  bayesmix::MarginalState out;
  bayesmix::MarginalState::ClusterState* clusval = out.add_cluster_states();
  bayesmix::MarginalState::ClusterState* clusval2 = out.add_cluster_states();
  hier->write_state_to_proto(clusval);
  hier2->write_state_to_proto(clusval2);

  ASSERT_TRUE(clusval->DebugString() != clusval2->DebugString());
}

TEST(nnwhierarchy, draw) {
  auto hier = std::make_shared<NNWHierarchy>();
  bayesmix::NNWPrior prior;
  Eigen::Vector2d mu0;
  mu0 << 5.5, 5.5;
  bayesmix::Vector mu0_proto;
  bayesmix::to_proto(mu0, &mu0_proto);
  double lambda0 = 0.2;
  double nu0 = 5.0;
  Eigen::Matrix2d tau0 = Eigen::Matrix2d::Identity() / nu0;
  bayesmix::Matrix tau0_proto;
  bayesmix::to_proto(tau0, &tau0_proto);
  *prior.mutable_fixed_values()->mutable_mean() = mu0_proto;
  prior.mutable_fixed_values()->set_var_scaling(lambda0);
  prior.mutable_fixed_values()->set_deg_free(nu0);
  *prior.mutable_fixed_values()->mutable_scale() = tau0_proto;
  hier->set_prior(prior);
  hier->initialize();

  auto hier2 = hier->clone();
  hier2->draw();

  bayesmix::MarginalState out;
  bayesmix::MarginalState::ClusterState* clusval = out.add_cluster_states();
  bayesmix::MarginalState::ClusterState* clusval2 = out.add_cluster_states();
  hier->write_state_to_proto(clusval);
  hier2->write_state_to_proto(clusval2);

  ASSERT_TRUE(clusval->DebugString() != clusval2->DebugString());
}

TEST(nnwhierarchy, sample_given_data) {
  auto hier = std::make_shared<NNWHierarchy>();
  bayesmix::NNWPrior prior;
  Eigen::Vector2d mu0;
  mu0 << 5.5, 5.5;
  bayesmix::Vector mu0_proto;
  bayesmix::to_proto(mu0, &mu0_proto);
  double lambda0 = 0.2;
  double nu0 = 5.0;
  Eigen::Matrix2d tau0 = Eigen::Matrix2d::Identity() / nu0;
  bayesmix::Matrix tau0_proto;
  bayesmix::to_proto(tau0, &tau0_proto);
  *prior.mutable_fixed_values()->mutable_mean() = mu0_proto;
  prior.mutable_fixed_values()->set_var_scaling(lambda0);
  prior.mutable_fixed_values()->set_deg_free(nu0);
  *prior.mutable_fixed_values()->mutable_scale() = tau0_proto;
  hier->set_prior(prior);
  hier->initialize();

  Eigen::RowVectorXd datum(2);
  datum << 4.5, 4.5;

  auto hier2 = hier->clone();
  hier2->add_datum(0, datum);
  hier2->sample_given_data();

  bayesmix::MarginalState out;
  bayesmix::MarginalState::ClusterState* clusval = out.add_cluster_states();
  bayesmix::MarginalState::ClusterState* clusval2 = out.add_cluster_states();
  hier->write_state_to_proto(clusval);
  hier2->write_state_to_proto(clusval2);

  ASSERT_TRUE(clusval->DebugString() != clusval2->DebugString());
}

TEST(lin_reg_uni_hierarchy, read_and_write) {
  // TODO
}

TEST(lin_reg_uni_hierarchy, lpdf) {
  // TODO marginal_lpdf = posterior_lpdf - prior_lpdf
}

TEST(lin_reg_uni_hierarchy, misc) {
  int n = 5;
  int dim = 2;
  Eigen::Vector2d beta_true;
  beta_true << 10.0, 10.0;
  auto cov = Eigen::MatrixXd::Random(n, dim);  // random in [-1,1]
  auto data = cov * beta_true + Eigen::VectorXd::Random(n);  // same
  double unif_var = 1.0 / 3;                    // variance of a U[-1,1]
  double true_var = dim * unif_var + unif_var;  // variance of each datum
  LinRegUniHierarchy hier;
  bayesmix::LinRegUnivPrior prior;

  Eigen::Vector2d beta0 = 0 * beta_true;
  bayesmix::Vector beta0_proto;
  bayesmix::to_proto(beta0, &beta0_proto);
  auto Lambda0 = Eigen::Matrix2d::Identity();
  bayesmix::Matrix Lambda0_proto;
  bayesmix::to_proto(Lambda0, &Lambda0_proto);
  double a0 = 2.0;
  double b0 = 1.0;
  *prior.mutable_fixed_values()->mutable_mean() = beta0_proto;
  *prior.mutable_fixed_values()->mutable_var_scaling() = Lambda0_proto;
  prior.mutable_fixed_values()->set_shape(a0);
  prior.mutable_fixed_values()->set_scale(b0);
  hier.set_prior(prior);

  for (int i = 0; i < n; i++) {
    hier.add_datum(i, data.row(i), cov.row(i));
  }
  // TODO comment/delete
  ASSERT_EQ(hier.get_covar_sum_squares(), cov.transpose() * cov);
  ASSERT_EQ(hier.get_mixed_prod(), cov.transpose() * data);

  auto state = hier.get_state();

  hier.sample_given_data();
  for (int i = 0; i < dim; i++) {
    ASSERT_TRUE(state.regression_coeffs[i] > beta0[i]);
  }

  std::cout << "[          ] ----> var  = " << state.var << std::endl;
  std::cout << "[          ] approx. equal to" << std::endl;
  std::cout << "[          ] ----> true = " << true_var << std::endl;
}
