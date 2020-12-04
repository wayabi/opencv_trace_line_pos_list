#!/bin/sh

./a.out \
	--log_level trace \
	--r_trace 255 \
	--g_trace 255 \
	--b_trace 255 \
	--a_trace 255 \
	--display_ip_address 192.168.11.5 \
	--display_port 12345 \
	--input_image a.png
