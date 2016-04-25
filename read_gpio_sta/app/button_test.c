#include <stdio.h> /*��׼���������,��sscanf����,fprintf�����������������*/
#include <stdlib.h>
#include <unistd.h> /*һЩ��Ķ�����������,��stderr*/
#include <sys/ioctl.h> /*�ļ��������ƿ�,��ioctl������������*/

#define DEF_GPIO_DIR_OUT    0x01
#define DEF_GPIO_DIR_IN     0x06
#define DEF_GPIO_SET_DATA   0x03
#define DEF_GPIO_CLR_DATA   0x04
#define DEF_GPIO_GET_DATA   0x05

#define GPIO_BUTTON 26
//#define GPIO_BUTTON 58

int main(int argc, char **argv)
{
	int fd; /*�豸��,���Ӵ򿪵�leds�豸���*/ 

    char key;
	
	//fd = open("/dev/leo368_leds", 0);
	fd = open("/dev/button", 2);
	
	/*����������Բ�ʵ��open�������������������£��豸�Ĵ򿪲�����Զ�ɹ���*/
	
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

