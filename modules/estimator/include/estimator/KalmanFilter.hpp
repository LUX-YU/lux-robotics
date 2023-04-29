//
// Created by lux on 2023/4/23.
//
#pragma once
#include <Eigen/Core>
#include <Eigen/LU>
#include <random>

namespace lux::robotics
{
// SDIM: state dimension
// MDIM: measurement dimension
// CDIM: control dimension
template<typename Type, size_t SDIM, size_t MDIM, size_t CDIM>
class KalmanFilter
{
public:
    using VectorN   = Eigen::Vector<Type, SDIM>;
    using VectorM   = Eigen::Vector<Type, MDIM>;
    using VectorC   = Eigen::Vector<Type, CDIM>;
    using MatrixNxC = Eigen::Matrix<Type, SDIM, CDIM>;
    using MatrixNxN = Eigen::Matrix<Type, SDIM, SDIM>;
    using MatrixMxN = Eigen::Matrix<Type, MDIM, SDIM>;
    using MatrixNxM = Eigen::Matrix<Type, SDIM, MDIM>;
    using MatrixMxM = Eigen::Matrix<Type, MDIM, MDIM>;

    using MeasureType    = VectorM;
    using ControlType    = VectorC;

    struct Description
    {
        MatrixNxN    state_transform_mat;   // A
        MatrixNxC    control_input_mat;     // B
        MatrixMxN    measurement_function;  // H
        MatrixNxN    process_noise;         // Q noise, Process excitation noise covariance
        MatrixMxM    measurement_noise;     // R noise
    };

    explicit KalmanFilter(const Description& description,
                          const VectorN& value = VectorN::Zero(),
                          const MatrixNxN& error = MatrixNxN::Identity())
            :   _description(description),
                _last_estimate_state(value),
                _last_estimate_covariance_matrix(error){}

    void predict(const ControlType& control_value)
    {
        // 1 predict process
        // calculate    the prior state estimate vector
        // A            the state transform matrix
        // B            the control input matrix
        const auto& A = _description.state_transform_mat;
        const auto& B = _description.control_input_mat;
        _predict_value = A * _last_estimate_state + B * control_value;

        // calculate    the prior covariance matrix
        // Q            the state transform covariance mat
        const auto& Q = _description.process_noise;
        _predict_covariance_matrix = A * _last_estimate_covariance_matrix * A.transpose() + Q;
    }

    // the predict process without control input
    void predict()
    {
        const auto& A = _description.state_transform_mat;
        _predict_value = A * _last_estimate_state;

        const auto& Q = _description.process_noise;
        _predict_covariance_matrix = A * _last_estimate_covariance_matrix * A.transpose() + Q;
    }

    void update(const MeasureType& measure_value)
    {
        // 2 update process
        // calculate Kalman gain
        // K            the kalman gain
        // H            the measurement matrix
        // R            the measurement noise covariance matrix
        // x_h_p        the prior state estimate
        // p_h_p        the prior covariance matrix
        const auto& x_h_p   = _predict_value;
        const auto& p_h_p   = _predict_covariance_matrix;
        const auto& H       = _description.measurement_function;
        const auto& R       = _description.measurement_noise;
        auto& K = _current_gain;
        K = (p_h_p * H.transpose()) * (H * p_h_p * H.transpose() + R).inverse();

        // x_h          the estimate state vector
        // p            the estimate covariance matrix
        _last_estimate_state = x_h_p + K * (measure_value - H * x_h_p);
        _last_estimate_covariance_matrix = (MatrixNxN::Identity() - K * H) * p_h_p;
    }

    [[nodiscard]] const MatrixNxM& kalman_gain() const
    {
        return _current_gain;
    }

    [[nodiscard]] const VectorN& predict_value() const
    {
        return _predict_value;
    }

    [[nodiscard]] const MatrixNxN& predict_covariance_matrix() const
    {
        return _predict_covariance_matrix;
    }

    [[nodiscard]] const VectorN& estimate_value() const
    {
        return _last_estimate_state;
    }

    [[nodiscard]] const MatrixNxN& estimate_covariance_matrix() const
    {
        return _last_estimate_covariance_matrix;
    }

private:
    VectorN         _predict_value;
    MatrixNxN       _predict_covariance_matrix;
    MatrixNxM       _current_gain;

    VectorN         _last_estimate_state;
    MatrixNxN       _last_estimate_covariance_matrix;

    Description     _description;
};

class ExtendedKalmanFilter
{
public:

};

}