#include "GripPipeline.h"

namespace grip {

GripPipeline::GripPipeline() {
}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
void GripPipeline::Process(cv::Mat& source0){
	//Step CV_flip0:
	//input
	cv::Mat cvFlipSrc = source0;
	FlipCode cvFlipFlipcode = FlipCode::Y_AXIS;
	cvFlip(cvFlipSrc, cvFlipFlipcode, this->cvFlipOutput);
	//Step CV_cvtColor0:
	//input
	cv::Mat cvCvtcolorSrc = cvFlipOutput;
    int cvCvtcolorCode = cv::COLOR_RGB2HSV;
	cvCvtcolor(cvCvtcolorSrc, cvCvtcolorCode, this->cvCvtcolorOutput);
	//Step HSV_Threshold0:
	//input
	cv::Mat hsvThresholdInput = cvCvtcolorOutput;
	double hsvThresholdHue[] = {0.1885141325667572, 0.1885141325667572};
	double hsvThresholdSaturation[] = {255.0, 255.0};
	double hsvThresholdValue[] = {255.0, 255.0};
	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);
	//Step Find_Contours0:
	//input
	cv::Mat findContoursInput = hsvThresholdOutput;
	bool findContoursExternalOnly = false;  // default Boolean
	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);
}

/**
 * This method is a generated getter for the output of a CV_flip.
 * @return Mat output from CV_flip.
 */
cv::Mat* GripPipeline::GetCvFlipOutput(){
	return &(this->cvFlipOutput);
}
/**
 * This method is a generated getter for the output of a CV_cvtColor.
 * @return Mat output from CV_cvtColor.
 */
cv::Mat* GripPipeline::GetCvCvtcolorOutput(){
	return &(this->cvCvtcolorOutput);
}
/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat* GripPipeline::GetHsvThresholdOutput(){
	return &(this->hsvThresholdOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> >* GripPipeline::GetFindContoursOutput(){
	return &(this->findContoursOutput);
}
	/**
	 * Flips an image along X, Y or both axes.
	 * @param src Image to flip.
	 * @param flipcode FlipCode of which direction to flip.
	 * @param dst flipped version of the Image.
	 */
	void GripPipeline::cvFlip(cv::Mat &src, FlipCode &flipcode, cv::Mat &dst) {
		cv::flip(src, dst, flipcode);
	}

	/**
	 * Converts an image from one color space to another.
	 * @param src Image to convert.
	 * @param code conversion code.
	 * @param dst converted Image.
	 */
	void GripPipeline::cvCvtcolor(cv::Mat &src, int code, cv::Mat &dst) {
		cv::cvtColor(src, dst, code);
	}

	/**
	 * Segment an image based on hue, saturation, and value ranges.
	 *
	 * @param input The image on which to perform the HSL threshold.
	 * @param hue The min and max hue.
	 * @param sat The min and max saturation.
	 * @param val The min and max value.
	 * @param output The image in which to store the output.
	 */
	void GripPipeline::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
		cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
	}

	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void GripPipeline::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}



} // end grip namespace

