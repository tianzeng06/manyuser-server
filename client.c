#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "unpipc.h"
#include "my_err.h"

#define SERV_FIFO "/work/test/test.serv"

int main()
{
	char fifoname[MAXLINE],buff[MAXLINE];
	
	//read pathname
	fgets(fifoname,MAXLINE,stdin);
	size_t len=strlen(fifoname);
	fifoname[len-1]='\0';
	
	//create FIFO whith pathname
	pid_t pid=getpid();
	sprintf(fifoname,"%s:%ld",fifoname,pid);
	if((mkfifo(fifoname,FILE_MODE)<0)&&(errno!=EEXIST))
	  err_sys("can't create %s",fifoname);
	
	//open FIFO to server and write pathname to FIFO
	int writefifo=open(SERV_FIFO,O_WRONLY,0);
	write(writefifo,fifoname,len);

	//now open our FIFO;blocks until server opens for writing
	int readfifo=open(fifoname,O_RDONLY,0);

	//read from IPC,write to standard output
	ssize_t n;
	while((n=read(readfifo,buff,MAXLINE))>0)
	  write(STDOUT_FILENO,buff,n);

	close(readfifo);
	unlink(fifoname);
	exit(0);
}
