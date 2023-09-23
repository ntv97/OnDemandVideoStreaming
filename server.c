#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>

#define SIZE 1024

char* send_intial_prompt(int sockfd) {
	int i = 0;
	char *msg1 = "These are the movies available: ";
	char* msgreq = "\nWhich movie would you like to watch?";
	char *filename = calloc(SIZE, sizeof(char));
	while (1) {
		if (!i) {
			if (send(sockfd, msg1, SIZE, 0) == -1) {
				perror("[-]Error in sending file.");
				exit(1);
			}	  
			DIR *d;
			struct dirent *dir;
			d = opendir("/home/nhivo/OnDemandVideoStreaming/ServerVideos");
			if (d) {
				while ((dir = readdir(d)) != NULL) {
					if (dir->d_type == DT_REG) {
						if (send(sockfd, dir->d_name, SIZE, 0) == -1) {
							perror("[-]Error in sending file.");
							exit(1);
						}
					}
				}
				closedir(d);
			}
			if (send(sockfd, msgreq, SIZE, 0) == -1) {
				perror("[-]Error in sending file.");
				exit(1);
			}
		} else if (i == 1) {
			int n = recv(sockfd, filename, SIZE, 0);
			if (n<=0) break;
			printf("\nMovie Requested: %s\n", filename);
		} else {
			break;
		}
		i++;
	}
	return filename;
}

void send_file(FILE *fp, int sockfd){
	printf("Start sending file\n\n");
	int n;
	char data[SIZE] = {0};

	while (1) {
		char buffer[SIZE];
		size_t bytesread = fread(buffer, 1, sizeof(buffer), fp);
		if (bytesread == 0) break;
		if (send(sockfd, buffer, sizeof(buffer), 0) == -1) {
			perror("[-]Error in sending file.");
			exit(1);
		}
		bzero(buffer, SIZE);
	}
}


int main(){
	char *ip = "127.0.0.1";
	int port = 8080;

	int sockfd, new_sock;
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[SIZE];

	FILE *fp;
//char *filename = "ServerVideos/dogs.mp4";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("[-]Error in socket");
		exit(1);
	}
	printf("> Server socket created successfully.\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("> Error in bind");
		exit(1);
	}
	printf("> Binding successful.\n");

	if(listen(sockfd, 10) == 0){
		  printf("> Listening....\n");
	} else {
		  perror("> Error in listening");
		exit(1);
	}

	addr_size = sizeof(new_addr);
	new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
	printf("> Connected with client.\n");

	char* filename = send_intial_prompt(new_sock);
	char path[SIZE];
	strcpy(path, "ServerVideos/");
	strcpy(path+strlen(path), filename);
//printf("Path: %s\n", path);
	fp = fopen(path, "rb");
	if (!fp) {
		perror("[-]Error in reading file.");
		exit(1);
	}

//send_intial_prompt(new_sock);
	send_file(fp, new_sock);
	printf("> Video File sent successfully.\n");
	printf("> ... Closing the connection ...\n");
	fclose(fp);
	return 0;
}

