#!/bin/sh

gen_arg_parse_h \
	log_level,string,trace \
	r_trace,int,man \
	g_trace,int,man \
	b_trace,int,man \
	a_trace,int,255 \
	display_ip_address,string, \
	display_port,int,0 \
	input_image,string, \
	> src/arg_parse.h
