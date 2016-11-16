#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// The lib was generated with a C compiler
extern "C" {
	#include <Decoder.h>
}

int main(int argc, char* argv[]) {
	FILE *finput;
	u32 strmLen;
	u8 *byteStrmStart;
    u8 *streamStop = NULL;
    H264SwDecInput decInput;

    if(argc != 2) {
        std::cout << "Usage: h264_test raw_video.bin" << std::endl;
        return 1;
    }

    /* open input file for reading, file name given by user. If file open
     * fails -> exit */
    finput = fopen(argv[argc-1],"rb");
    if (finput == NULL) {
        DEBUG(("UNABLE TO OPEN INPUT FILE\n"));
        return -1;
    }

    /* check size of the input file -> length of the stream in bytes */
    fseek(finput,0L,SEEK_END);
    strmLen = (u32)ftell(finput);
    rewind(finput);

    /* allocate memory for stream buffer. if unsuccessful -> exit */
    byteStrmStart = (u8 *)malloc(sizeof(u8)*strmLen);
    if (byteStrmStart == NULL) {
        DEBUG(("UNABLE TO ALLOCATE MEMORY\n"));
        return -1;
    }

    /* read input stream from file to buffer and close input file */
    fread(byteStrmStart, sizeof(u8), strmLen, finput);
    fclose(finput);


    /* initialize decoder. If unsuccessful -> exit */
    broadwayInit();
	
    // Sets the stream to be decoded
    Stream s;
    s.length = strmLen;
    s.buffer = byteStrmStart;
    s.pos = byteStrmStart;
    s.end = byteStrmStart+strmLen;
    setStream(s);

    std::cout << "About to play: " << strmLen << std::endl;
    bool streaming = true;
    do {
        u32 ret = broadwayDecode();
        switch (ret) {
            case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
                /* Stream headers were successfully decoded, thus stream information is available for query now. */
                break;

            case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
                /* Picture is ready and more data remains in the input buffer,
                 * update input structure.
                 */
                /* fall through */

            case H264SWDEC_PIC_RDY:
                {
                    u32 height, width;
                    u8* img_data;
                    img_data = getFrame(&width, &height);
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

                    break;
                }

            case H264SWDEC_STRM_PROCESSED:
                // fall through
            case H264SWDEC_STRM_ERR:
                /* Input stream was decoded but no picture is ready, thus get more data. */
                streaming = false;
                break;
              
            default:
                break;
        }
    } while (streaming);
    
    return 0;
}
