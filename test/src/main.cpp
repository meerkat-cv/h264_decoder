#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <Decoder.h>

int main(int argc, char* argv[]) {
	std::cout << std::endl << std::endl;
    std::cout << "ooo        ooooo                              oooo                      .   "     << std::endl;
    std::cout << "`88.       .888'                              `888                    .o8   "     << std::endl;
    std::cout << " 888b     d'888   .ooooo.   .ooooo.  oooo d8b  888  oooo   .oooo.   .o888oo "     << std::endl;
    std::cout << " 8 Y88. .P  888  d88' `88b d88' `88b `888\"\"8P  888 .8P'   `P  )88b    888   "   << std::endl;
    std::cout << " 8  `888'   888  888ooo888 888ooo888  888      888888.     .oP\"888    888   "    << std::endl;
    std::cout << " 8    Y     888  888    .o 888    .o  888      888 `88b.  d8(  888    888 . "     << std::endl;
    std::cout << "o8o        o888o `Y8bod8P' `Y8bod8P' d888b    o888o o888o `Y888\"\"8o   \"888\" " << std::endl;
    std::cout << std::endl << "\tSimple H264 Decoder" << std::endl << std::endl;

    if(argc < 2) {
        std::cout << "Usage: h264_test raw_video_part1.bin raw_video_part1.bin ..." << std::endl;
        std::cout << "Example: ./h264_test ../../data/video_websocket_*" << std::endl;
        return 1;
    }

    Stream s;

    for(int part=0; part<argc-1; part++) {
        FILE *finput;
        u32 strmLen;
        u8 *byteStrmStart;

        // open input file for reading, file name given by user. If file open
        // fails -> exit 
        finput = fopen(argv[part+1],"rb");
        if (finput == NULL) {
            DEBUG(("UNABLE TO OPEN INPUT FILE\n"));
            return -1;
        }

         // check size of the input file -> length of the stream in bytes 
        fseek(finput,0L,SEEK_END);
        strmLen = (u32)ftell(finput);
        rewind(finput);

         // allocate memory for stream buffer. if unsuccessful -> exit 
        byteStrmStart = new u8[strmLen];
        if (byteStrmStart == NULL) {
            DEBUG(("UNABLE TO ALLOCATE MEMORY\n"));
            return -1;
        }

         // read input stream from file to buffer and close input file 
        u32 ignored = fread(byteStrmStart, sizeof(u8), strmLen, finput);
        fclose(finput);

         
        // Sets the stream to be decoded
        if(part == 0) {
            s.SetStream(byteStrmStart, strmLen);
        } else {
            s.UpdateStream(byteStrmStart, strmLen);
        }

        std::cout << "Opening: " << argv[part+1] << std::endl;
        bool streaming = true;
        u32 height, width;
        u8* img_data;
        // consuming all the data from argv[part+1]
        do {
            StreamStatus ret = s.BroadwayDecode();
            switch (ret) {
                case PIC_READY_BUFFER_NOT_EMPTY:
                    img_data = s.GetFrame(&width, &height);
                    break;

                case PIC_READY:
                    img_data = s.GetFrame(&width, &height);
                    streaming = false;
                    break;

                case STREAM_ERROR:
                case STREAM_ENDED:
                    streaming = false;
                    break;
                  
                default:
                    break;
            }

            // Is there any new frame?
            if (ret == PIC_READY || ret == PIC_READY_BUFFER_NOT_EMPTY) {
                if(img_data == NULL) {
                    std::cout << "Image data is NULL" << std::endl;
                    break;
                }
                cv::Mat img_buffer(height+height/2, width, CV_8UC1, (uchar *)img_data);
                // if you want only grayscale you can directly get only Y. And if you want the
                // cr and cb channels separated, here you go
                // cv::Mat Y(height, width, CV_8UC1, (uchar *)img_data);
                // cv::Mat cb(height/2, width/2, CV_8UC1, (uchar *)(img_data+height*width));
                // cv::Mat cr(height/2, width/2, CV_8UC1, (uchar *)(img_data+height*width+(height/2)*(width/2)));
                cv::Mat original;
                cv::cvtColor(img_buffer, original, cv::COLOR_YUV2RGB_YV12);
                cv::imshow("Original", original);
                cv::waitKey(64);
            }

        } while (streaming);

        // free this stream buffer package
        free(byteStrmStart);
    }
    
    return 0;
}
