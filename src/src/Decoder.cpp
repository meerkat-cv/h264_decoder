#include "Decoder.h"

Stream::Stream() {

}

Stream::~Stream() {
    if (streamBuffer) {
        free(streamBuffer);
    }
}

void Stream::SetStream(u8* strmBuffer, u32 strmLength) {
    decInput.pStream = strmBuffer;
    decInput.dataLen = strmLength;
}

// void Stream::UpdateStream(u32 strmLen, u8* strmBuffer, u8* strmPos, u8* strmEnd) {
    // u32 new_buffer_size = broadwayStream.length - broadwayStream.mid_stream + strmLen;
    // u8* new_buffer = new u8[new_buffer_size];
    // if (new_buffer == NULL) {
    //     DEBUG(("UNABLE TO ALLOCATE MEMORY\n"));
    //     exit(1);
    // }
    // // using mid_stream to discard the past stream package. Otherwise we would just keep on growing
    // // the buffer
    // u32 last_package_size = broadwayStream.length-broadwayStream.mid_stream;
    // memcpy(new_buffer, broadwayStream.buffer+broadwayStream.mid_stream, last_package_size);
    // memcpy(new_buffer+last_package_size, strmBuffer, strmLen);
    // free(broadwayStream.buffer);
    // broadwayStream.buffer = new_buffer;

    // broadwayStream.pos -= broadwayStream.mid_stream;
    // broadwayStream.length = last_package_size + strmLen;
    // broadwayStream.end = broadwayStream.buffer+broadwayStream.length;

    // decInput.pStream = broadwayStream.buffer;
    // decInput.dataLen = broadwayStream.length;   
 // }


u8* Stream::GetFrame(u32* outImageWidth, u32* outImageHeight) {
    *outImageWidth = decInfo.picWidth;
    *outImageHeight = decInfo.picHeight;

    return (u8*)decPicture.pOutputPicture;
}


u32 Stream::Init() {
  H264SwDecRet ret;
#ifdef DISABLE_OUTPUT_REORDERING
  u32 disableOutputReordering = 1;
#else
  u32 disableOutputReordering = 0;
#endif

  /* Initialize decoder instance. */
  ret = H264SwDecInit(&decInst, disableOutputReordering);
  if (ret != H264SWDEC_OK) {
    DEBUG(("DECODER INITIALIZATION FAILED\n"));
    return -1;
  }

  picDecodeNumber = 1;
  return 0;
}


StreamStatus Stream::BroadwayDecode() {
    decInput.picId = picDecodeNumber;

    H264SwDecRet ret = H264SwDecDecode(decInst, &decInput, &decOutput);
    StreamStatus status;

    switch (ret) {
        case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
            // Stream headers were successfully decoded, thus stream information is available for query now. 
            ret = H264SwDecGetInfo(decInst, &decInfo);
            if (ret != H264SWDEC_OK) {
                return STREAM_ERROR;
            }

            picSize = decInfo.picWidth * decInfo.picHeight;
            picSize = (3 * picSize) / 2;

            decInput.dataLen -= decOutput.pStrmCurrPos - decInput.pStream;
            decInput.pStream = decOutput.pStrmCurrPos;

            break;

        case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
            // Picture is ready and more data remains in the input buffer,
            // update input structure.
            decInput.dataLen -= decOutput.pStrmCurrPos - decInput.pStream;
            decInput.pStream = decOutput.pStrmCurrPos;

            // fall through 

        case H264SWDEC_PIC_RDY:
            picDecodeNumber++;
      
            while (H264SwDecNextPicture(decInst, &decPicture, 0) == H264SWDEC_PIC_RDY) { }

            break;

        case H264SWDEC_STRM_PROCESSED:
        case H264SWDEC_STRM_ERR:
            // Input stream was decoded but no picture is ready, thus get more data. 
            decInput.dataLen = 0;
            break;
              
        default:
            break;
    }

    switch (ret) {
        case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
        case H264SWDEC_OK:
            status = HEADERS_READY;
            break;

        case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
            status = PIC_READY_BUFFER_NOT_EMPTY;

        case H264SWDEC_PIC_RDY:
            status = PIC_READY;
            break;

        case H264SWDEC_STRM_PROCESSED:
            status = STREAM_ENDED;
            break;

        case H264SWDEC_STRM_ERR:
        default:
            status = STREAM_ERROR;
    }


  return status;
}


u32 broadwayGetMajorVersion() {
    return H264SwDecGetAPIVersion().major;
}

u32 broadwayGetMinorVersion() {
    return H264SwDecGetAPIVersion().minor;
}

