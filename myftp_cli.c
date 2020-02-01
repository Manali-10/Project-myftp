#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
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

int main(int argc, char *argv[]){
	int sockfd,fd;
	struct stat buf1;
	char *runbuf;

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	char *serv_ip="127.0.0.1";
	
	struct sockaddr_in servaddr;
	
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8000);
	inet_pton(AF_INET,serv_ip,&servaddr.sin_addr);

	int ret_val;	
	
	ret_val=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	printf("myftp>\n");

	printf("pwd  - current directory of the server\nlpwd - current directory of client\ndir  - file names under the current directory of the server \nldir - file names under the current directory of the client\ncd   - change directory of the server\nlcd  - change directory of the client\nget  - to download file from server\nput  - to upload file on server\nquit - to terminate the myftp session\n");
		

	while(1){

	printf("\nmyftp>\n");

	memset(&arr,0,sizeof(arr));

	printf("\nEnter the command:");
	scanf("%s",arr.cmd);

	if(strcmp(arr.cmd,"lpwd")==0){
		system("pwd");
	}
	else if(strcmp(arr.cmd,"ldir")==0){
		system("dir");
	}
	else if(strcmp(arr.cmd,"quit")==0){
		exit(1);
	}
	else if(strcmp(arr.cmd,"pwd")==0){
		write(sockfd,&arr,sizeof(arr));

		read(sockfd,&arr,sizeof(arr));

		printf("%s\n",arr.buf);
	}	
	else if(strcmp(arr.cmd,"dir")==0){
		write(sockfd,&arr,sizeof(arr));

		read(sockfd,&arr,sizeof(arr));

		printf("%s\n",arr.buf);
	}
	else if(strcmp(arr.cmd,"lcd")==0){
		printf("Enter the path :");
		scanf("%s",arr.path);
		chdir(arr.path);
		printf("lcd command sucessfully done\n");	
	}	
	else if(strcmp(arr.cmd,"cd")==0){
		printf("Enter the path :");
		scanf("%s",arr.path);
		write(sockfd,&arr,sizeof(arr));
		printf("cd command sucessfully done\n");	
	}
	else if(strcmp(arr.cmd,"get")==0){

		printf("Enter the filename :");
		scanf("%s",arr.filename);
		
		write(sockfd,&arr,sizeof(arr));

		read(sockfd,&arr,sizeof(arr));

		runbuf=(char *)malloc(arr.size);
	
		arr.ack=1;

		write(sockfd,&arr,sizeof(arr));

		read(sockfd,runbuf,arr.size);

		fd=open(arr.filename,O_CREAT | O_EXCL | O_RDWR,0777);

		write(fd,runbuf,arr.size);

		printf("get command sucessfully done\n");
	}
	else if(strcmp(arr.cmd,"put")==0){
		
		printf("Enter the filename :");
		scanf("%s",arr.filename);

		int info=stat(arr.filename,&buf1);

		arr.size=buf1.st_size;

		runbuf=(char *)malloc(arr.size);

		write(sockfd,&arr,sizeof(arr));

		read(sockfd,&arr,sizeof(arr));

		fd=open(arr.filename,O_RDONLY);

		read(fd,runbuf,arr.size);

		write(sockfd,runbuf,arr.size);
		
		printf("put command sucessfully done\n");
	}

	}	
}
