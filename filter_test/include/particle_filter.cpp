#pragma once
// std
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <vector>
// OpenMP
#include <omp.h>
// Eigen
#include <Eigen/Dense>
// utility
#include "math_util.hpp"

namespace filter
{
  class ParticleFilter
  {
  public:
    ParticleFilter() = default;
    ~ParticleFilter() = default;

    void init(const Eigen::VectorXd &x, const Eigen::MatrixXd &P, const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R, const int N)
    {
      x_ = x;
      P_ = P;
      Q_ = Q;
      R_ = R;
      N_ = N;
      particles_.resize(N);
      weights_.resize(N);
      cumsum_.resize(N);
      engine_.seed(seed_gen_());
      uniform_dist_ = std::uniform_real_distribution<double>(0.0, 1.0);
    }
    void predict(const Eigen::VectorXd &u, const double dt)
    {
      std::normal_distribution<double> normal_dist(0.0, 1.0);
      for (int i = 0; i < N_; i++)
      {
        Eigen::VectorXd x = particles_[i];
        x(0) += u(0) * dt * std::cos(x(2)) + normal_dist(engine_) * Q_(0, 0);
        x(1) += u(0) * dt * std::sin(x(2)) + normal_dist(engine_) * Q_(1, 1);
        x(2) += u(1) * dt + normal_dist(engine_) * Q_(2, 2);
        particles_[i] = x;
      }
    }
    void update(const Eigen::VectorXd &z)
    {
      std::vector<double> likelihood(N_);
      double sum = 0.0;
      for (int i = 0; i < N_; i++)
      {
        Eigen::VectorXd x = particles_[i];
        double dx = x(0) - z(0);
        double dy = x(1) - z(1);
        double d = std::sqrt(dx * dx + dy * dy);
        likelihood[i] = std::exp(-0.5 * d * d / R_(0, 0));
        sum += likelihood[i];
      }
      for (int i = 0; i < N_; i++)
      {
        weights_[i] = likelihood[i] / sum;
      }
    }
    void resampling()
    {
      std::vector<Eigen::VectorXd> resampled_particles(N_);
      std::vector<double> resampled_weights(N_);
      std::vector<double> cumsum(N_);
      cumsum[0] = weights_[0];
      for (int i = 1; i < N_; i++)
      {
        cumsum[i] = cumsum[i - 1] + weights_[i];
      }
      for (int i = 0; i < N_; i++)
      {
        double r = uniform_dist_(engine_);
        auto it = std::lower_bound(cumsum.begin(), cumsum.end(), r);
        int index = std::distance(cumsum.begin(), it);
        resampled_particles[i] = particles_[index];
        resampled_weights[i] = 1.0 / N_;
      }
      particles_ = resampled_particles;
      weights_ = resampled_weights;
    }

    Eigen::VectorXd getMean() const;
    Eigen::MatrixXd getCovariance() const;

  private:
    Eigen::VectorXd x_;
    Eigen::MatrixXd P_;
    Eigen::MatrixXd Q_;
    Eigen::MatrixXd R_;
    int N_;
    std::vector<Eigen::VectorXd> particles_;
    std::vector<double> weights_;
    std::vector<double> cumsum_;
    std::random_device seed_gen_;
    std::default_random_engine engine_;
    std::uniform_real_distribution<double> uniform_dist_;
  };
} // namespace filter
