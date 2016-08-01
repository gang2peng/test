#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void main(int argc,char *argv[])
{	
		int fd;
		int i;
		char *buzzer = "/dev/buzzer_ctl";

		if( atoi(argv[1]) >1 )
			printf("error ! argv[1] must be less than 2!!\n");

		if((fd = open(buzzer, O_RDWR|O_NOCTTY|O_NDELAY))<0)
		{
			printf("open buzzer fail!\n");
			exit(1);
		}
		else
		{	
				ioctl(fd,atoi(argv[1]),atoi(argv[2]));
				printf("open sucess!!\n");
		}

		close(fd);
	
}
