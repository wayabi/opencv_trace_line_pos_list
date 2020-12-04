#ifndef __U_ARG_PARSE__
#define __U_ARG_PARSE__

#include <string>
#include <ostream>

#include <boost/program_options.hpp>

class arg_parse {

public:
	std::string log_level;
	int r_trace;
	int g_trace;
	int b_trace;
	int a_trace;
	std::string display_ip_address;
	int display_port;
	std::string input_image;
public:
	std::string parse(int argc, const char** argv)
	{
		using namespace boost::program_options;
		options_description opt("opt");
		opt.add_options()
			("log_level", value<std::string>()->default_value("trace"), "log_level")
			("r_trace", value<int>()->required(), "r_trace")
			("g_trace", value<int>()->required(), "g_trace")
			("b_trace", value<int>()->required(), "b_trace")
			("a_trace", value<int>()->default_value(255), "a_trace")
			("display_ip_address", value<std::string>()->default_value(""), "display_ip_address")
			("display_port", value<int>()->default_value(0), "display_port")
			("input_image", value<std::string>()->default_value(""), "input_image")
		;
		variables_map vm;
		try{
			store(parse_command_line(argc, argv, opt), vm);
			notify(vm);
			log_level = vm["log_level"].as<std::string>();
			r_trace = vm["r_trace"].as<int>();
			g_trace = vm["g_trace"].as<int>();
			b_trace = vm["b_trace"].as<int>();
			a_trace = vm["a_trace"].as<int>();
			display_ip_address = vm["display_ip_address"].as<std::string>();
			display_port = vm["display_port"].as<int>();
			input_image = vm["input_image"].as<std::string>();
		}catch(std::exception& e){
			return std::string(e.what());
		}
		return std::string();
	}

	void print(std::ostream& o)
	{
		o << "log_level[std::string]: " << log_level << std::endl;
		o << "r_trace[int]: " << r_trace << std::endl;
		o << "g_trace[int]: " << g_trace << std::endl;
		o << "b_trace[int]: " << b_trace << std::endl;
		o << "a_trace[int]: " << a_trace << std::endl;
		o << "display_ip_address[std::string]: " << display_ip_address << std::endl;
		o << "display_port[int]: " << display_port << std::endl;
		o << "input_image[std::string]: " << input_image << std::endl;
	}
};
#endif
