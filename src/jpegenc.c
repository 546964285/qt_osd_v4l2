#include "jpegenc.h"


int readFrame(Buffer_Handle hBuf,void * ptr)
{
    Int8 * framePtr = Buffer_getUserPtr(hBuf);
    BufferGfx_Dimensions dim;
    BufferGfx_getDimensions(hBuf,&dim);
    unsigned int ySize,cbcrSize;
    char * colorFmt=NULL;
    ySize = dim.width*dim.height;

    switch(BufferGfx_getColorSpace(hBuf))
    {
        case ColorSpace_UYVY:
            cbcrSize = dim.width * dim.height;
            colorFmt = "UYVY";
            break;
        case ColorSpace_YUV444P:
            cbcrSize = dim.width * dim.height * 2;
            colorFmt = "444P";
            break;
        case ColorSpace_YUV422P:
            cbcrSize = dim.width * dim.height;
            colorFmt = "422P";
            break;
        case ColorSpace_YUV420P:
            cbcrSize = dim.width * dim.height / 2;
            colorFmt = "420P";
            break;
        case ColorSpace_GRAY:
            cbcrSize = 0;
            colorFmt = "GRAY";
            break;
        case ColorSpace_YUV420PSEMI:
            cbcrSize = dim.width * dim.height / 2;
            colorFmt = "420SP";
            break;
        default:
            printf("Unsupported input color space.\n");
            return -1;
    }

    memcpy(framePtr,ptr,221184);
    printf("Read %s frame size %d (%dx%d)\n", colorFmt,(int)(ySize+cbcrSize),(int)dim.width,(int)dim.height);
    Buffer_setNumBytesUsed(hBuf,Buffer_getSize(hBuf));
    return 0;
}


void csayhello(char *str)
{
    printf("%s\n",str);
}
