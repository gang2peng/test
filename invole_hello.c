#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

main()
{
	int fd;
	int ioctl_state1;
	int ioctl_state2;
	char *hello_node0 = "/dev/JTSB0";
	char *hello_node1 = "/dev/JTSB1";
	if((fd=open(hello_node0,O_RDWR|O_NDELAY))<0){
		printf("OPEN APP fail!\n");
	}
	else{
		printf("sucess APP!\n");
		ioctl_state1=ioctl(fd,1,6);
		printf("ioctl_state1 is %d\n",ioctl_state1);
	}
	
	close(fd);
	
	if((fd=open(hello_node1,O_RDWR|O_NDELAY))<0){
		printf("OPEN APP fail!\n");
	}
	else{
		printf("sucess APP!\n");
		ioctl_state2=ioctl(fd,1,7);
		printf("ioctl_state2 is %d\n",ioctl_state2);
	}
	
	close(fd);
	
}
