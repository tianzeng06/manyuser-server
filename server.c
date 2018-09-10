#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include "my_err.h"
#include "unpipc.h"

#define SERV_FIFO "/work/test/test.serv"

int main()
{
	char *ptr=NULL,buff[MAXLINE+1],fifoname[MAXLINE],tmp[MAXLINE];

	if((mkfifo(SERV_FIFO,FILE_MODE)<0)&&(errno!=EEXIST))
	  err_sys("can't create %s",SERV_FIFO);

	//open sever's well-know FIFO reading and write
	int readfifo=open(SERV_FIFO,O_RDONLY,0);
	int dummyfd=open(SERV_FIFO,O_WRONLY,0);

	//read pathname from IPC(is from client pathname) to fifoname
	ssize_t n;
	if((n=read(readfifo,fifoname,MAXLINE))==0)
		err_quit("end of file while reading pathname");
	
	strcpy(tmp,fifoname);
	if((ptr=strchr(tmp,':'))==NULL)
		err_msg("bogus resquest: %s",fifoname);
	*ptr++='\0';
	
	//open write text to pathname(fifoname)
	int writefd=open(fifoname,O_WRONLY,0);
	
	//write text(from SERV_FIFO) to pathname(fifoname)
	int fd;
	if((fd=open(ptr,O_RDONLY))<0)
	{
		int len=strlen(fifoname);
		write(writefd,fifoname,len);
		close(writefd);
	}
	else
	{
		while((n=read(fd,buff,MAXLINE))>0)
			write(writefd,buff,n);

		close(fd);
		close(writefd);
	}
	exit(0);	
}
