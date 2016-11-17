extern "C" {
#include "H264SwDecApi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Debug prints */
#define DEBUG(argv) printf argv

#include "opttarget.h"

#include "extraFlags.h"

}

// This is just H264SwDecRet with clearer names
enum StreamStatus {
    HEADERS_READY              = 0, // Stream headers were successfully decoded, thus stream information is available for query now
    PIC_READY                  = 1, // Picture is ready and no more data is on buffer
    PIC_READY_BUFFER_NOT_EMPTY = 2, // Picture is ready and more data remains in the input buffer
    STREAM_ENDED               = 3, // Input stream was decoded but no picture is ready, thus get more data
    STREAM_ERROR               = 4  // Internal stream error
};

class Stream {
public:
    Stream();
    ~Stream();

    u8* GetFrame(u32* outImageWidth, u32* outImageHeight);
    void SetStream(u8* strmBuffer, u32 strmLength);
    void UpdateStream(u8* strmBuffer, u32 strmLength);
    StreamStatus BroadwayDecode();

private:
    u8 *streamBuffer = NULL;

    H264SwDecInst decInst;
    H264SwDecInput decInput;
    H264SwDecOutput decOutput;
    H264SwDecPicture decPicture;
    H264SwDecInfo decInfo;

    u32 picDecodeNumber;
    u32 picSize;
    u32 currPackagePos;
    u32 bufferSize;
};


u32 broadwayGetMajorVersion();

u32 broadwayGetMinorVersion();