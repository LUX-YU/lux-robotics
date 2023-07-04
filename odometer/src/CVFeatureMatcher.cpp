#include "lux/robotics/odometer/CVFeatureMatcher.hpp"

CVFeatureMatcher::CVFeatureMatcher(cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::DescriptorMatcher> matcher)
	: _detector(std::move(detector)), _matcher(std::move(matcher))
{

}

void CVFeatureMatcher::match(const cv::Mat& img1, const cv::Mat img2, KeyPointList& key_list1, KeyPointList& key_list2, DMatchList& match_list)
{
	cv::Mat descriptor_1, descriptor_2;

	// detect oriented FAST corner position
	_detector->detect(img1, key_list1);
	_detector->detect(img2, key_list2);

	// compute BRIEF descriptor according to the corner position
	_detector->compute(img1, key_list1, descriptor_1);
	_detector->compute(img2, key_list2, descriptor_2);

	_matcher->match(descriptor_1, descriptor_2, match_list);

	// auto min_max = cv::minmax
}

void setMinimumLimit(double)
{

}
