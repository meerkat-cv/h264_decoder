import numpy as np 
import cv2
import os
import sys
sys.path.append('../' )
from lib import h264_decoder

PIC_READY                  = 0 # Picture is ready and no more data is on buffer
STREAM_ENDED               = 1 # Input stream was decoded but no picture is ready, thus get more data
STREAM_ERROR               = 2 # Internal stream error

# open a video stream
newFile = open('../../data/video_full.avi', 'rb')
buff_size = os.fstat(newFile.fileno()).st_size
buff = newFile.read(buff_size)
buff = np.fromstring(buff, np.uint8);
newFile.close()


# get a Stream object
s = h264_decoder.Stream()
s.SetStream(buff)

streaming = True
while streaming:
    ret = s.BroadwayDecode()
    if ret == PIC_READY:
        img_data = s.GetFrame()
        
    else: # ret == STREAM_ERROR or ret == STREAM_ENDED:
        streaming = False

    # Is there any new frame?
    if ret == PIC_READY:
        if img_data is None:
            print("Image data is None")
            break
        
        cv2.imshow("Video", img_data)
        cv2.waitKey(64)

