#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<strings.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/stat.h>
struct info{
	char cmd[10];
	char filename[20];
	char buf[100];
	char path[50];
	long int size;
	int ack;
};

struct info arr;

int main(){
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	int ret_val,connfd,sockfd,fd,i,j,length;
	struct stat buf1;
	char *runbuf,shellbuf[50];
	char temp[]=" >txt.txt";

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(8000);

	ret_val=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	listen(sockfd,5);
	
	clilen=sizeof(cliaddr);
		

	connfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
	
	while(1){

	printf("myftp>\n");
	
	read(connfd,&arr,sizeof(arr));

	printf("cmd : %s\n",arr.cmd);
	
	if(strcmp(arr.cmd,"get")==0){
	
		int info=stat(arr.filename,&buf1);

		arr.size=buf1.st_size;

		runbuf=(char *)malloc(arr.size);

		write(connfd,&arr,sizeof(arr));

		read(connfd,&arr,sizeof(arr));
	
		fd=open(arr.filename,O_RDONLY);

		read(fd,runbuf,arr.size);

		write(connfd,runbuf,arr.size);
		
		printf("get command sucessfully done\n");

	}
	else if(strcmp(arr.cmd,"put")==0){
	
		runbuf=(char *)malloc(arr.size);

		arr.ack=1;

		write(connfd,&arr,sizeof(arr));

		read(connfd,runbuf,arr.size);

		fd=open(arr.filename,O_CREAT | O_EXCL | O_RDWR,0777);

		write(fd,runbuf,arr.size);

		printf("put pucommand sucessfully done\n");
	
	}
	else if(strcmp(arr.cmd,"pwd")==0){
		length=strlen(arr.cmd);
		for(i=0;i<length;i++){
			shellbuf[i]=arr.cmd[i];
		}
		for(i=length,j=0;i<12;j++,i++){
			shellbuf[i]=temp[j];
		}
		system(shellbuf);

		fd=open("./txt.txt",O_RDONLY);

		read(fd,&arr.buf,100);

		write(connfd,&arr,sizeof(arr));

		printf("pwd command sucessfully done\n");
	}
	else if(strcmp(arr.cmd,"dir")==0){
		length=strlen(arr.cmd);
		for(i=0;i<length;i++){
			shellbuf[i]=arr.cmd[i];
		}
		for(i=length,j=0;i<12;j++,i++){
			shellbuf[i]=temp[j];
		}
		system(shellbuf);

		fd=open("./txt.txt",O_RDONLY);

		read(fd,&arr.buf,100);

		write(connfd,&arr,sizeof(arr));

		printf("dir command sucessfully done\n");
		
	}
	else if(strcmp(arr.cmd,"cd")==0){
		chdir(arr.path);

		printf("cd command sucessfully done\n");
	}
	else
		exit(1);

	}	

	return 0;
}
