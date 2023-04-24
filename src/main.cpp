//
// Created by lux on 2023/4/23.
//
#include "KalmanFilter.hpp"
#include <matplot/matplot.h>

int main()
{
    using namespace matplot;

    // real value sequence
    std::vector<double> x_ = linspace(0, 1.0);
    std::vector<double> x(50, -0.37727);

    // create measurement value
    std::vector<double> z;
    std::random_device random_device{};
    std::mt19937 gen{random_device()};
    std::normal_distribution<double> normal_distribution(-0.37727, 0.1);

    // initialize kalman filter
    using Filter = KalmanFilter<double, 1, 1, 1>;
    Filter::Description filter_description;
    filter_description.process_noise        = Filter::MatrixNxN{1e-5};
    filter_description.measurement_noise    = Filter::MatrixMxM{0.01};
    filter_description.state_transform_mat  = Filter::MatrixNxN{1};
    filter_description.measurement_function = Filter::MatrixMxN{1};
    filter_description.control_input_mat    = Filter::MatrixNxC{1};

    // do estimate
    std::vector<double> estimate_value(50);
    std::vector<double> predict_value(50);
    Filter filter{filter_description};
    for(size_t i = 0; i < 50; i++){
        x_.push_back((double)i);
        z.push_back(normal_distribution(random_device));

        filter.predict();
        predict_value[i] = filter.predict_value()[0];
        filter.update(Filter::MeasureType{z.back()});
        estimate_value[i] = filter.estimate_value()[0];
    }

    plot(x_, x)->display_name("real state");
    hold(on);
    plot(x_, z)->display_name("measurement value");
    plot(x_, predict_value)->display_name("predict value");
    plot(x_, estimate_value)->display_name("estimate value");
    hold(off);
    matplot::legend();

    show();

    return 0;
}