#include "unpipc.h"
#include "my_err.h"
#include <string.h>

void server(int readfd,int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE+1];

	if((n=read(readfd,buff,MAXLINE))==0)
	  err_quit("end-of-file while reading pathname");
	buff[n]='\0';

	if((fd=open(buff,O_RDONLY))<0)
	{
		 snprintf(buff+n,sizeof(buff)-n,":can't open, %s\n",strerror(errno));
		n=strlen(buff);
		write(writefd,buff,n);
	}
	else
	{
		while((n=read(fd,buff,MAXLINE))>0)
		  write(writefd,buff,n);

		close(fd);
	}
}
