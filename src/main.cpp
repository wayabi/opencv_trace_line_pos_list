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

std::shared_ptr<Mat> process(Mat& m)
{
	std::shared_ptr<Mat> ret = make_shared<Mat>(m);
	int w = m.cols;
	int h = m.rows;
	int channels = m.channels();
	int size = w*h;
	unsigned char* b = m.data;
	int x_start = -1;
	int y_start = -1;
	for(int y=0;y<h;++y){
		for(int x=0;x<w;++x){
			if(*(b+(y*w+x)*channels+0) == 255){
				x_start = x;
				y_start = y;
				break;
			}
		}
		if(x_start >= 0) break;
	}

	if(x_start < 0){
		_li << "no start point.";
		return ret;
	}

	vector<unsigned char> history;
	history.resize(size);
	memset(&history[0], 0, size);
	history[y_start*w+x_start] = 255;
	cout << x_start << ", " << y_start << endl;

	while(sub_process(m.data, w, h, channels, x_start, y_start, &history[0])){
		cout << x_start << ", " << y_start << endl;
	}

	return ret;
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
	process(m_input);
	return 0;
}
