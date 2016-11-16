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

class Stream {
public:
    Stream();
    ~Stream();

    u8* GetFrame(u32* outImageWidth, u32* outImageHeight);
    u32 Init();
    void SetStream(u8* strmBuffer, u32 strmLength);
    u32 BroadwayDecode();

private:
    u8 *streamBuffer = NULL;

    H264SwDecInst decInst;
    H264SwDecInput decInput;
    H264SwDecOutput decOutput;
    H264SwDecPicture decPicture;
    H264SwDecInfo decInfo;

    u32 picDecodeNumber;
    u32 picDisplayNumber;
    u32 picSize;
};


u32 broadwayGetMajorVersion();

u32 broadwayGetMinorVersion();