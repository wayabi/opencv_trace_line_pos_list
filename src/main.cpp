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
#include "display_sender.h"

using namespace std;
using namespace cv;

arg_parse args;

vector<unsigned char> buf0_;

bool _sub_process0(const unsigned char* s, int w, int h, int channel, intx, inty, unsigned char* history)
{
	if(x < 0 || y < 0 || x >= w || y >= h || *(s+(y*w+x)*channel) != 255 || *(history+y*w+x) == 255){
		return false;
	}
	*(history+y*w+x) = 255;
	return true;
}

bool _sub_process1(const unsigned char* s, int w, int h, int channel, int x_start, int y_start, unsigned char* history, list<int>& stuck)
{
	int x = x_start;
	int y = y_start;
	while(true){
		int eight_flag = stuck.back;
		if(eight_flag == 8){
			stuck.pop_back();
			if(stuck.size() == 0) return;
		}
		++stuck.back;
		if(eight_flag == 0){
			if(_sub_process0(s, w, h, channel, x, y, history)){
				stuck.push_back(0);
			}else{
				stuck.pop_back();
			}
		}else if(eight_flag == 1){
			if(_sub_process0(s, w, h, channel, x-1, y, history, stuck)){
				stuck.back = stuck.back + 1;
			}
		}
	}
}

std::shared_ptr<Mat> process(Mat& m)
{
	std::shared_ptr<Mat> ret = make_shared<Mat>(m);
	int w = m.cols;
	int h = m.rows;
	int channel = m.channels();
	int size = w*h;
	buf0_.resize(size);
	memset(&buf0_[0], 0, size);
	unsigned char* b = m.data;
	int x_start = -1;
	int y_start = -1;
	for(int y=0;y<h;++y){
		for(int x=0;x<w;++x){
			if(*(b+(y*w+x)*channel+0) == 255){
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

	shared_ptr<display_sender> display_sender_;
	if(args.display_ip_address.size() > 0){
		display_sender_ = std::make_shared<display_sender>(0, args.display_ip_address, args.display_port);
	}

	Mat m_input = imread(args.input_image.c_str());
	if(m_input.cols == 0){
		_le << "invalid input_image path:" << args.input_image;
		return 1;
	}
	auto m_send = process(m_input);

	if(display_sender_ != nullptr && m_send != nullptr){
		display_sender_->send(*(m_send.get()));
		_li << "sended(" << m_send->cols << ", " << m_send->rows << ")";
	}

	getchar();

	return 0;
}
