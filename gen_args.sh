#!/bin/sh

gen_arg_parse_h \
	log_level,string,trace \
	input_image,string, \
	start_pixel_value,int,128 \
	> src/arg_parse.h
