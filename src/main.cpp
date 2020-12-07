#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
#include <list>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "boost_util.h"
#include "arg_parse.h"

using namespace std;
using namespace cv;

arg_parse args;


bool sub_process(const unsigned char* src, int w, int h, int channels, int& x, int& y, unsigned char* history)
{
	int xx, yy;
	{
		xx = x-1;
		yy = y;
		if(x > 0){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x;
		yy = y-1;
		if(y > 0){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x+1;
		yy = y;
		if(x < w-1){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x;
		yy = y+1;
		if(y < h-1){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x-1;
		yy = y-1;
		if(x > 0 && y > 0){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x+1;
		yy = y-1;
		if(x < w-1 && y > 0){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x+1;
		yy = y+1;
		if(x < w-1 && y < h-1){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	{
		xx = x-1;
		yy = y+1;
		if(x > 0 && y < h-1){
			if(*(src+(yy*w+xx)*channels+0) == 255 && *(history+yy*w+xx) == 0){
				*(history+yy*w+xx) = 255;
				x = xx;
				y = yy;
				return true;
			}
		}
	}
	return false;
}

vector<int> xx_;
vector<int> yy_;

bool process(Mat& m, int start_pixel_value)
{
	int w = m.cols;
	int h = m.rows;
	int channels = m.channels();
	int size = w*h;
	unsigned char* b = m.data;
	int x_start = -1;
	int y_start = -1;
	for(int y=0;y<h;++y){
		for(int x=0;x<w;++x){
			if(*(b+(y*w+x)*channels+0) == start_pixel_value){
				x_start = x;
				y_start = y;
				break;
			}
		}
		if(x_start >= 0) break;
	}

	if(x_start < 0){
		_li << "no start point.";
		return false;
	}

	vector<unsigned char> history;
	history.resize(size);
	memset(&history[0], 0, size);
	history[y_start*w+x_start] = 255;
//	cout << x_start << ", " << y_start << endl;
	xx_.push_back(x_start);
	yy_.push_back(y_start);
	int x = x_start;
	int y = y_start;
	while(sub_process(m.data, w, h, channels, x, y, &history[0])){
//		cout << x << ", " << y << endl;
		xx_.push_back(x);
		yy_.push_back(y);
	}

	if(xx_.size() >= 4 && abs(x-x_start) <= 1 && abs(y-y_start)){
		return true;
	}
	return false;
}

int main(int argc, const char* argv[])
{
	string arg_error = args.parse(argc, argv);
	if(arg_error.length() > 0){
		cout << "arg parse error:" << arg_error << endl;
		return 1;
	}
	boost_util::set_log_level(args.log_level, 0);
	stringstream ss_args;
	args.print(ss_args);
	_li << ss_args.str();

	Mat m_input = imread(args.input_image.c_str());
	if(m_input.cols == 0){
		_le << "invalid input_image path:" << args.input_image;
		return 1;
	}
	bool is_loop = process(m_input, args.start_pixel_value);

	int num = xx_.size();
	for(int i=0;i<num;++i){
		cout << xx_[i] << ",";
	}
	cout << endl;
	for(int i=0;i<num;++i){
		cout << yy_[i] << ",";
	}
	cout << endl;

	cout << "is_loop:" << is_loop << endl;
	return 0;
}
