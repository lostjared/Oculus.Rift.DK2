/* Acid Cam Functions for OpenCV
 * written by Jared Bruni
 * http://lostsidedead.com
 * GPL
 */

#ifndef __AC_H__
#define __AC_H__
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include "opencv2/opencv.hpp"


/*
 * to use set appropriate variables, call the function
 *
 * ac::SelfAlphaBlend(mat);
 *
 * or use draw_func
 *
 * ac::draw_func[function_index](mat);
 *
 * then just use function_index to point to whatever filter
 *
 * for text of the filter use the following array of strings
 *
 * ac::draw_strings[function_index];
 *
 */

extern int current_filterx, bytesPerSample, bytesPerRow, width, height, red, green, blue, offset, randomNumber, reverse;
extern bool negate;


namespace ac {
	static std::string version="2.0.17";
	extern float translation_variable, pass2_alpha;
	extern float alpha, tr;
	extern bool isNegative, noRecord,pass2_enabled,blendW,slide_Show,slide_Rand,strobe_It,switch_Back,blur_First,blur_Second,iRev;
    extern int color_order;
	extern double fps;
	extern int draw_offset;
	static std::string fileName ="VideoFile.avi";
	extern cv::Mat orig_frame;
	extern cv::Mat blendW_frame;
	extern cv::Mat image_files[4];
	extern bool images_Enabled,fps_force;
    extern int snapshot_Type;

	inline int GetFX(cv::Mat &frame, int x, int nw);
	inline int GetFY(cv::Mat &frame, int y, int nh);
	inline void invert(cv::Mat &frame, int x, int y);
	inline void randAlpha(float &value);
	void resetAll();
	void enablePass2(bool pass2_enabled, bool pass2_alpha);

	typedef void (*DrawFunction)(cv::Mat &frame);

	void SelfAlphaBlend(cv::Mat &frame);
	void StrobeEffect(cv::Mat &frame);
	void Blend3(cv::Mat &frame);
	void NegParadox(cv::Mat &frame);
	void ThoughtMode(cv::Mat &frame);
	void Pass2Blend(cv::Mat &frame);
	void RandTriBlend(cv::Mat &frame);
	void Blank(cv::Mat &frame);
	void Tri(cv::Mat &frame);
	void Distort(cv::Mat &frame);
	void CDraw(cv::Mat &frame);
	void Type(cv::Mat &frame);
    void NewOne(cv::Mat &frame);
	void blendWithImage(cv::Mat &frame);
	void triBlendWithImage(cv::Mat &frame);
	void imageStrobe(cv::Mat &frame);
	void imageDistraction(cv::Mat &frame);
	void blendFractal(cv::Mat &frame);
	void blendFractalMood(cv::Mat &frame);
    void cossinMultiply(cv::Mat &frame);
    void colorAccumulate1(cv::Mat &frame);
    void colorAccumulate2(cv::Mat &frame);
    void colorAccumulate3(cv::Mat &frame);
    void filter8(cv::Mat &frame);
    void filter3(cv::Mat &frame);
    void rainbowBlend(cv::Mat &frame);
    void randBlend(cv::Mat &frame);
    void newBlend(cv::Mat &frame);
    void alphaFlame(cv::Mat &frame);
    void custom(cv::Mat &frame);
	// draw functions / strings
	static std::string draw_strings[] = { "Self AlphaBlend", "StrobeEffect", "Blend #3", "Negative Paradox", "ThoughtMode", "RandTriBlend", "Blank", "Tri", "Distort", "CDraw", "Type", "NewOne", "Blend Fractal","Blend Fractal Mood", "CosSinMultiply", "Color Accumlate1", "Color Accumulate2", "Color Accumulate3", "filter8","filter3","Rainbow Blend","Rand Blend","New Blend", "Alpha Flame Filters", "Custom", "Blend With Image #1",  "TriBlend with Image", "Image Strobe", "Image distraction" };
    extern DrawFunction draw_func[30];
    extern int draw_max;
    extern bool snapShot;
}

extern void custom_filter(cv::Mat &frame);


#endif
