project(h264_decoder)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=c++1y -fPIC -O3 -mavx -fpermissive")

include_directories("src/src/"
					"src/inc/")

file(GLOB all_h264_decoder_src
	"src/src/h264bsd_transform.c"
	"src/src/h264bsd_util.c"
	"src/src/h264bsd_byte_stream.c"
	"src/src/h264bsd_seq_param_set.c"
	"src/src/h264bsd_pic_param_set.c"
	"src/src/h264bsd_slice_header.c"
	"src/src/h264bsd_slice_data.c"
	"src/src/h264bsd_macroblock_layer.c"
	"src/src/h264bsd_stream.c"
	"src/src/h264bsd_vlc.c"
	"src/src/h264bsd_cavlc.c"
	"src/src/h264bsd_nal_unit.c"
	"src/src/h264bsd_neighbour.c"
	"src/src/h264bsd_storage.c"
	"src/src/h264bsd_slice_group_map.c"
	"src/src/h264bsd_intra_prediction.c"
	"src/src/h264bsd_inter_prediction.c"
	"src/src/h264bsd_reconstruct.c"
	"src/src/h264bsd_dpb.c"
	"src/src/h264bsd_image.c"
	"src/src/h264bsd_deblocking.c"
	"src/src/h264bsd_conceal.c"
	"src/src/h264bsd_vui.c"
	"src/src/h264bsd_pic_order_cnt.c"
	"src/src/h264bsd_decoder.c"
	"src/src/H264SwDecApi.c"
	"src/src/extraFlags.c"
	"src/src/Decoder.cpp"
)

add_library( meerkat_h264_decoder SHARED
		${all_h264_decoder_src})


