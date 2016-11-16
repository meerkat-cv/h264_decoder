#include "Decoder.h"

const size_t STREAM_BUFFER_SIZE = 1024 * 1024;
u8 *streamBuffer = NULL;

H264SwDecInst decInst;
H264SwDecInput decInput;
H264SwDecOutput decOutput;
H264SwDecPicture decPicture;
H264SwDecInfo decInfo;

u32 picDecodeNumber;
u32 picDisplayNumber;
u32 picSize;

void streamInit(Stream *stream, u32 length) {
    stream->buffer = stream->pos = (u8 *)malloc(sizeof(u8) * length);
    stream->length = length;
    stream->end = stream->buffer + length;
}

void playStream(Stream *stream) {
    decInput.pStream = stream->buffer;
    decInput.dataLen = stream->length;
    u32 i = 0;
    do {
        u8 *start = decInput.pStream;
        u32 ret = broadwayDecode();
        printf("Decoded Unit #%d, Size: %d, Result: %d\n", i++, (decInput.pStream - start), ret);
    } while (decInput.dataLen > 0);
    printf("Input len:%d stream_lengh:%d", decInput.dataLen, stream->length);
}


Stream broadwayStream;

void setStream(Stream stream) {
    broadwayStream.length = stream.length;
    broadwayStream.buffer = stream.buffer;
    broadwayStream.pos = stream.pos;
    broadwayStream.end = stream.end;

    decInput.pStream = stream.buffer;
    decInput.dataLen = stream.length;
}


u32 CropPicture(u8 *pOutImage, u8 *pInImage,
    u32 picWidth, u32 picHeight, CropParams *pCropParams)
{

    u32 i, j;
    u32 outWidth, outHeight;
    u8 *pOut, *pIn;

    if (pOutImage == NULL || pInImage == NULL || pCropParams == NULL ||
        !picWidth || !picHeight)
    {
        /* just to prevent lint warning, returning non-zero will result in
         * return without freeing the memory */
        free(pOutImage);
        return(1);
    }

    if ( ((pCropParams->cropLeftOffset + pCropParams->cropOutWidth) >
           picWidth ) ||
         ((pCropParams->cropTopOffset + pCropParams->cropOutHeight) >
           picHeight ) )
    {
        /* just to prevent lint warning, returning non-zero will result in
         * return without freeing the memory */
        free(pOutImage);
        return(1);
    }

    outWidth = pCropParams->cropOutWidth;
    outHeight = pCropParams->cropOutHeight;

    /* Calculate starting pointer for luma */
    pIn = pInImage + pCropParams->cropTopOffset*picWidth +
        pCropParams->cropLeftOffset;
    pOut = pOutImage;

    /* Copy luma pixel values */
    for (i = outHeight; i; i--)
    {
        for (j = outWidth; j; j--)
        {
            *pOut++ = *pIn++;
        }
        pIn += picWidth - outWidth;
    }

    outWidth >>= 1;
    outHeight >>= 1;

    /* Calculate starting pointer for cb */
    pIn = pInImage + picWidth*picHeight +
        pCropParams->cropTopOffset*picWidth/4 + pCropParams->cropLeftOffset/2;

    /* Copy cb pixel values */
    for (i = outHeight; i; i--)
    {
        for (j = outWidth; j; j--)
        {
            *pOut++ = *pIn++;
        }
        pIn += picWidth/2 - outWidth;
    }

    /* Calculate starting pointer for cr */
    pIn = pInImage + 5*picWidth*picHeight/4 +
        pCropParams->cropTopOffset*picWidth/4 + pCropParams->cropLeftOffset/2;

    /* Copy cr pixel values */
    for (i = outHeight; i; i--)
    {
        for (j = outWidth; j; j--)
        {
            *pOut++ = *pIn++;
        }
        pIn += picWidth/2 - outWidth;
    }

    return (0);
}



u8* getFrame(u32* outImageWidth, u32* outImageHeight) {
    *outImageWidth = decInfo.picWidth;
    *outImageHeight = decInfo.picHeight;

    u8* img_buffer = (u8 *)malloc(sizeof(u8) * decInfo.picWidth * (decInfo.picHeight+decInfo.picHeight/2));

    CropPicture(img_buffer, (u8*)decPicture.pOutputPicture, decInfo.picWidth, decInfo.picHeight, &decInfo.cropParams);

    // return img_buffer;
    return (u8*)decPicture.pOutputPicture;
}


u8 *broadwayCreateStream(u32 length) {
    streamInit(&broadwayStream, length);
    return broadwayStream.buffer;
}

#ifdef LATENCY_OPTIMIZATION
#endif


void broadwayPlayStream(u32 length) {
    broadwayStream.length = length;
    playStream(&broadwayStream);
}


u32 broadwayInit() {
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
    broadwayExit();
    return -1;
  }

  picDecodeNumber = picDisplayNumber = 1;
  return 0;
}


// extern void broadwayOnHeadersDecoded();

// extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height);

u32 broadwayDecode() {
    decInput.picId = picDecodeNumber;

    H264SwDecRet ret = H264SwDecDecode(decInst, &decInput, &decOutput);

    switch (ret) {
        case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
            /* Stream headers were successfully decoded, thus stream information is available for query now. */
            ret = H264SwDecGetInfo(decInst, &decInfo);
            if (ret != H264SWDEC_OK) {
                return -1;
            }

            picSize = decInfo.picWidth * decInfo.picHeight;
            picSize = (3 * picSize) / 2;

            // broadwayOnHeadersDecoded();

            decInput.dataLen -= decOutput.pStrmCurrPos - decInput.pStream;
            decInput.pStream = decOutput.pStrmCurrPos;
            break;

        case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
            /* Picture is ready and more data remains in the input buffer,
             * update input structure.
             */
            decInput.dataLen -= decOutput.pStrmCurrPos - decInput.pStream;
            decInput.pStream = decOutput.pStrmCurrPos;

            /* fall through */

        case H264SWDEC_PIC_RDY:
            picDecodeNumber++;
      
            while (H264SwDecNextPicture(decInst, &decPicture, 0) == H264SWDEC_PIC_RDY) {
                // picDisplayNumber++;
            }
            break;

      case H264SWDEC_STRM_PROCESSED:
      case H264SWDEC_STRM_ERR:
        /* Input stream was decoded but no picture is ready, thus get more data. */
        decInput.dataLen = 0;
        break;
      
      default:
        break;
    }
  return ret;
}

void broadwayExit() {
    if (streamBuffer) {
        free(streamBuffer);
    }
}

u8 *broadwayCreateStreamBuffer(u32 size) {
    u8 *buffer = (u8 *)malloc(sizeof(u8) * size);
    if (buffer == NULL) {
        DEBUG(("UNABLE TO ALLOCATE MEMORY\n"));
    }
    return buffer;
}

u32 broadwayGetMajorVersion() {
    return H264SwDecGetAPIVersion().major;
}

u32 broadwayGetMinorVersion() {
    return H264SwDecGetAPIVersion().minor;
}

