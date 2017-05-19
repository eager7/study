#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

#define BUF_SIZE 10000

int update_ota_http_connect_server(int server_socket, char *host, int port){
	struct sockaddr_in server_addr;
    struct hostent *server;
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket < 0){
		printf("\n\r[%s] Create socket failed", __FUNCTION__);
		return -1;
	}

    server = gethostbyname(host);
    if(server == NULL) 
        printf("[ERROR] Get host ip failed\n");
    
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr,server->h_addr,server->h_length);
    
    if (connect(server_socket,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		printf("\n\r[%s] Socket connect failed", __FUNCTION__);
		return -1;
	}

	return server_socket;
}

int http_update_ota(char *host, int port, char *resource)
{
	unsigned char buf[BUF_SIZE], *alloc, *request;

	// Connect server
	int server_socket = update_ota_http_connect_server(server_socket, host, port);
	if(server_socket == -1){
		printf("update_ota_http_connect_server error\n");
		return -1;
	}
	request = (unsigned char *) malloc(strlen("GET /") + strlen(resource) + strlen(" HTTP/1.1\r\nHost: ") + strlen(host) + strlen("\r\n\r\n") + 1);
	sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", resource, host);

	int ret = write(server_socket, request, strlen(request));
	if(ret < 0){
		printf("\n\r[%s] Send HTTP request failed", __FUNCTION__);
		return -1;
	}
	while(1){
		memset(buf, 0, BUF_SIZE);
		int read_bytes = read(server_socket, buf, BUF_SIZE);

		if(read_bytes == 0){
			printf("read finished\n");
			break;
		} 
		if(read_bytes < 0){
			printf("\n\r[%s] Read socket failed", __FUNCTION__);
			return -1;
		}
		
		for(int i = 0; i < read_bytes; i ++){
			printf("%c", buf[i]);
		}		
	}
	
}
int main()
{
	printf("http client program...\n");
	http_update_ota("192.168.0.103", 8082, "test.bin");
	return 0;
}