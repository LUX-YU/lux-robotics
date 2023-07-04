#include <opencv2/features2d.hpp>
#include <driver/camera/ImageSequenceCapture.hpp>

int main(int argc, char* argv)
{
    using namespace ::lux::robotics;

    auto virtual_camera = ImageSequence::make_shared(R"(F:\Resource\DataSet\SLAM\V1_03_difficult\mav0\cam0\data)");

    ImageSequenceCapture capture{ virtual_camera };
    cv::Mat mat, last_mat;

    std::vector<cv::KeyPoint> k1, k2;
    std::vector<cv::DMatch>   matches;

    if (!capture.grab()) return 0;
    capture.retrieve(last_mat);
    
    auto orb = cv::ORB::create(10);
    auto matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");

    while (capture.grab())
    {
        capture.retrieve(mat);
        if (mat.empty()) break;

        cv::Mat descriptors1, descriptors2;
        orb->detectAndCompute(mat,      cv::noArray(), k1, descriptors1);
        orb->detectAndCompute(last_mat, cv::noArray(), k2, descriptors2);

        matcher->match(descriptors1, descriptors2, matches);

        cv::Mat img_matches;
        cv::drawMatches(mat, k1, last_mat, k2, matches, img_matches);

        matches.resize(0);
        k1.resize(0);
        k2.resize(0);

        cv::imshow("test", img_matches);
        last_mat = mat;
        if (cv::waitKey(33) == 'q') break;
    }

	return 0;
}
