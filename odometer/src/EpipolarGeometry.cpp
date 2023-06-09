#include "lux/odometer/EpipolarGeometry.hpp"
#include <opencv2/opencv.hpp>

EpipolarGeometry::EpipolarGeometry(
	std::unique_ptr<CVFeatureMatcher> matcher, 
	const cv::Mat& K, 
	cv::Point2d principal_point,
	double focal_length)
: _matcher(std::move(matcher)), _K(K), _principal_point(principal_point), _focal_length(focal_length)
{
	
}

void EpipolarGeometry::estimate(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& R, cv::Mat& t)
{
	KeyPointList list1, list2;
	DMatchList   match_list;
	_matcher->match(img1, img2, list1, list2, match_list);

	std::vector<cv::Point2f> points_1;
	std::vector<cv::Point2f> points_2;

	// find fundamental matrix
	// cv::Mat fundmental_matrix = cv::findFundamentalMat(points_1, points_2, cv::FM_8POINT);
	for (size_t i = 0; i < match_list.size(); i++)
	{
		points_1.push_back(list1[match_list[i].queryIdx].pt);
		points_1.push_back(list1[match_list[i].trainIdx].pt);
	}

	// find essential matrix
	cv::Mat essential_matrix = cv::findEssentialMat(points_1, points_2, _focal_length, _principal_point);

	// recover pose
	cv::recoverPose(essential_matrix, points_1, points_2, R, t, _focal_length, _principal_point);
}
