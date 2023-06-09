#pragma once
#include <opencv2/features2d/features2d.hpp>
#include <lux/cxx/visibility_control.h>

using KeyPointList	= std::vector<cv::KeyPoint>;
using DMatchList	= std::vector<cv::DMatch>;

class CVFeatureMatcher
{
public:
	LUX_PUBLIC CVFeatureMatcher(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorMatcher> matcher);

	LUX_PUBLIC void setMinimumLimit(double);

	LUX_PUBLIC void match(const cv::Mat& img1, const cv::Mat img2, KeyPointList& key_list1, KeyPointList& key_list2, DMatchList& match_list);

private:
	double minimum_limit{0};
	cv::Ptr<cv::FeatureDetector>	_detector;
	cv::Ptr<cv::DescriptorMatcher>	_matcher;
};
