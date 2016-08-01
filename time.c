#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#define __USE_GNU
#define LENTH 255

int main(int argc,char *argv[])
{
	int ret;
	DIR *dir;
	struct dirent *catlog;

	if(argc<2){
		perror("enter file path\n");
		return 1;
	}

	dir = opendir(argv[1]);
	if(NULL==dir){
		printf("opendir fail!!\n");
		return 1; 
	}

	catlog = readdir(dir);
	if(NULL==catlog){
		printf("readdir fail!!\n");
		return 1;
	}
	printf("%s is %ld\n",argv[1],catlog->d_ino);
	return 0;
}