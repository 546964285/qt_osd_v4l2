#include <stdio.h> /*标准输入输出库,像sscanf函数,fprintf函数都是在这个库里*/
#include <stdlib.h>
#include <unistd.h> /*一些宏的定义在这里面,像stderr*/
#include <sys/ioctl.h> /*文件操作控制库,像ioctl函数就在这里*/

#define DEF_GPIO_DIR_OUT    0x01
#define DEF_GPIO_DIR_IN     0x06
#define DEF_GPIO_SET_DATA   0x03
#define DEF_GPIO_CLR_DATA   0x04
#define DEF_GPIO_GET_DATA   0x05

#define GPIO_BUTTON 26
//#define GPIO_BUTTON 58

int main(int argc, char **argv)
{
	int fd; /*设备号,将从打开的leds设备获得*/ 

    char key;
	
	//fd = open("/dev/leo368_leds", 0);
	fd = open("/dev/button", 2);
	
	/*驱动程序可以不实现open这个函数，在这种情况下，设备的打开操作永远成功。*/
	
	if (fd < 0) {	
		perror("open device /dev/leo368_leds");	
		fprintf(stdout, "*****0*****:\n");
		exit(1);
	}
    ioctl(fd, DEF_GPIO_SET_DATA, GPIO_BUTTON);

	fprintf(stdout, "*****1*****:\n");
	ioctl(fd, DEF_GPIO_DIR_OUT, GPIO_BUTTON);

    sleep(1);
	fprintf(stdout, "*****2*****:\n");
	while (1)
	{
        //ioctl(fd, DEF_GPIO_SET_DATA, GPIO_BUTTON);

		//keyn = ioctl(fd, DEF_GPIO_GET_DATA, GPIO_BUTTON);

        

        //printf("button value is %x\n\n", ioctl(fd, DEF_GPIO_GET_DATA, GPIO_BUTTON));
		//sleep(1);

		read(fd, &key, sizeof(key));

		if(key!='0')
		{
		    putchar(key);
		    putchar('\n');
		}

        //ioctl(fd, DEF_GPIO_CLR_DATA, GPIO_BUTTON);
        //sleep(1);
	}
	close(fd);	
	return 0;
}

