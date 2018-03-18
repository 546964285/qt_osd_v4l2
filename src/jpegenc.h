#ifndef JPEGENC_H
#define JPEGENC_H
#include <stdio.h>
#include <stdlib.h>         // exit
#include <fcntl.h>		// for open()
#include <unistd.h>     // for close()
#include <malloc.h>         // clloc
#include <getopt.h>		// getopt_long()
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <media/davinci/dm365_ccdc.h>
#include <media/davinci/vpfe_capture.h>
#include <media/davinci/imp_previewer.h>
#include <media/davinci/imp_resizer.h>
#include <media/davinci/dm365_ipipe.h>

#include <video/davincifb_ioctl.h>
#include <video/davinci_osd.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/dmai/ColorSpace.h>
#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Time.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/ce/Ienc1.h>

int   readFrame(Buffer_Handle hBuf,void * ptr);
void  csayhello(char *str);

#endif
