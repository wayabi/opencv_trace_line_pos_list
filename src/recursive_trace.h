#include <vector>
#include <list>

public class recursive_trace {
	public struct trace_info {
	public:
		int x_;
		int y_;
		int x_parent_;
		int y_parent_;
		int step_parent_;
	};

	public struct map_info {
	public:
		const unsigned char* src_;
		int w_;
		int h_;
		int channels_;
		unsigned char* history_;
	}

	std::list<trace_info> trace_;
	std::vector<unsigned char> history_;
};

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
