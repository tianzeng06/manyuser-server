#include <string.h>
#include "unpipc.h"
//#include "my_err.h"
#define MAXLINE 4096

void client(int readfd,int writefd)
{
	char buff[MAXLINE];
	fgets(buff,MAXLINE,stdin);

	size_t len=strlen(buff);
	if(buff[len-1]=='\n')
	  len--;

	write(writefd,buff,len);

	ssize_t n;
	while((n=read(readfd,buff,MAXLINE))>0)
	  write(STDOUT_FILENO,buff,n);
}
