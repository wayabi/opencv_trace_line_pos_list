#ifndef __U_ARG_PARSE__
#define __U_ARG_PARSE__

#include <string>
#include <ostream>

#include <boost/program_options.hpp>

class arg_parse {

public:
	std::string log_level;
	std::string input_image;
	int start_pixel_value;
public:
	std::string parse(int argc, const char** argv)
	{
		using namespace boost::program_options;
		options_description opt("opt");
		opt.add_options()
			("log_level", value<std::string>()->default_value("trace"), "log_level")
			("input_image", value<std::string>()->default_value(""), "input_image")
			("start_pixel_value", value<int>()->default_value(128), "start_pixel_value")
		;
		variables_map vm;
		try{
			store(parse_command_line(argc, argv, opt), vm);
			notify(vm);
			log_level = vm["log_level"].as<std::string>();
			input_image = vm["input_image"].as<std::string>();
			start_pixel_value = vm["start_pixel_value"].as<int>();
		}catch(std::exception& e){
			return std::string(e.what());
		}
		return std::string();
	}

	void print(std::ostream& o)
	{
		o << "log_level[std::string]: " << log_level << std::endl;
		o << "input_image[std::string]: " << input_image << std::endl;
		o << "start_pixel_value[int]: " << start_pixel_value << std::endl;
	}
};
#endif
