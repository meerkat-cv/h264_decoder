#include "H264SwDecApi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Debug prints */
#define DEBUG(argv) printf argv

#include "opttarget.h"

#include "extraFlags.h"

typedef struct {
    u32 length;
    u8 *buffer;
    u8 *pos;
    u8 *end;
} Stream;

u8 *broadwayCreateStream(u32 length);

u8* getFrame(u32* outImageWidth, u32* outImageHeight);

void broadwayPlayStream(u32 length);


u32 broadwayInit();

void setStream(Stream stream);

extern void broadwayOnHeadersDecoded();

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height);

u32 broadwayDecode();

void broadwayExit();

u8 *broadwayCreateStreamBuffer(u32 size);

u32 broadwayGetMajorVersion();

u32 broadwayGetMinorVersion();

