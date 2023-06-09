#pragma once
#include <memory>
#include "CVFeatureMatcher.hpp"

class EpipolarGeometry
{
public:
	LUX_PUBLIC explicit EpipolarGeometry(
		std::unique_ptr<CVFeatureMatcher> matcher, 
		const cv::Mat& K, 
		cv::Point2d principal_point, 
		double focal_length
	);

	LUX_PUBLIC void estimate(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& R, cv::Mat& t);

private:
	cv::Mat								_K;
	cv::Point2d							_principal_point;
	double								_focal_length;
	std::unique_ptr<CVFeatureMatcher>	_matcher;
};
