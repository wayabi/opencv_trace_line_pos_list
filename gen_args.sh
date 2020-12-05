#!/bin/sh

gen_arg_parse_h \
	log_level,string,trace \
	input_image,string, \
	> src/arg_parse.h
