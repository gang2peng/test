#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	int fd;
	char *hello_node = "/dev/hello_ctl123";
	
	if((fd=open(hello_node,O_RDWR|O_NDELAY)) < 0 ){
		printf("\nopen fail!\n");
	}
	else{
		printf("\nopen sucess!\n");
		ioctl(fd,1,6);
	}
	close(fd);
}