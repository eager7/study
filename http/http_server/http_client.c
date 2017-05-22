#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

#define BUF_SIZE 10000
typedef struct {
	unsigned int	status_code;
	unsigned int	header_len;
	unsigned char	*body;
	unsigned int	body_len;
} http_response_result_t;

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

int parse_http_response(uint8_t *response, uint32_t response_len, http_response_result_t *result) {
    uint32_t i, p, q, m;
    uint8_t status[4] = {0};
    uint32_t content_length = 0;
    int ret = 0;
    const uint8_t *content_length_buf1 = "CONTENT-LENGTH";
    const uint8_t *content_length_buf2 = "Content-Length";
    const uint32_t content_length_buf_len = strlen(content_length_buf1);

    // status code
    i = p = q = m = 0;
    for (; i < response_len; ++i) {
        if (' ' == response[i]) {
            ++m;
            if (1 == m) {//after HTTP/1.1
                p = i;
            } 
			else if (2 == m) {//after status code
                q = i;
                break;
            }
        }
    }
    if (!p || !q || q-p != 4) {//HTTP/1.1 200 OK
        return 0;
    }

    memcpy(status, response+p+1, 3);//get the status code

    // Content-Length
    p = q = 0;
    for (i = 0; i < response_len; ++i) {
        if (response[i] == '\r' && response[i+1] == '\n') {
            q = i;//the end of the line
            if (!memcmp(response+p, content_length_buf1, content_length_buf_len) ||
                    !memcmp(response+p, content_length_buf2, content_length_buf_len)) {//the line of content length
                int j1 = p+content_length_buf_len, j2 = q-1;
                while ( j1 < q && (*(response+j1) == ':' || *(response+j1) == ' ') ) ++j1;
                while ( j2 > j1 && *(response+j2) == ' ') --j2;
                uint8_t len_buf[12] = {0};
                memcpy(len_buf, response+j1, j2-j1+1);
                content_length = atoi((char const *)len_buf);
                ret = 1;
            }
            p = i+2;
        }
        if (response[i] == '\r' && response[i+1] == '\n' &&
                response[i+2] == '\r' && response[i+3] == '\n') {//the end of header
            p = i+4;//p is the start of the body
            break;
        }
    }

    if (ret) {
        result->status_code = atoi((char const *)status);
		result->header_len = p;
        result->body = response + p;
        result->body_len = content_length;
    }

    return ret && (response_len >= p + content_length);
}

int http_update_ota(char *host, int port, char *resource)
{
	unsigned char buf[BUF_SIZE];
	unsigned char *alloc, *request;
	unsigned int idx = 0;
	http_response_result_t rsp_result = {0};
	
	// Connect server
	int server_socket = update_ota_http_connect_server(server_socket, host, port);
	if(server_socket == -1){
		printf("update_ota_http_connect_server error\n");
		return -1;
	}
	request = (unsigned char *) malloc(strlen("GET /") + strlen(resource) + strlen(" HTTP/1.1\r\nHost: ") + strlen(host) + strlen("\r\n\r\n") + 1);
	sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", resource, host);
	printf("Send Req to Server\n");
	int ret = write(server_socket, request, strlen(request));
	if(ret < 0){
		printf("\n\r[%s] Send HTTP request failed", __FUNCTION__);
		return -1;
	}
	memset(buf, 0, BUF_SIZE);
	//while(1)
	{
		int read_bytes = read(server_socket, buf, 1);
#if 0
		if(read_bytes == 0){
			printf("read finished\n");
			//continue;
			printf("---------------Http Read:\n");
			for(int i = 0; i < idx; i ++){
				//printf("%c", buf[i]);
			}		
			//break;
		} 
		if(read_bytes < 0){
			printf("\n\r[%s] Read socket failed", __FUNCTION__);
			return -1;
		}
		idx += read_bytes;
		#endif
	}
	memset(&rsp_result, 0, sizeof(rsp_result));
	parse_http_response(buf, idx, &rsp_result);
	printf("Read Image Finished[%d]\n", rsp_result.status_code);
	if(rsp_result.status_code != 200){
		printf("Failed Read Image\n");
		return -1;
	}
	if (0 == rsp_result.body_len){
		printf("\n\r[%s] New firmware size = 0 !\n", __FUNCTION__);
		return -1;
	}
	else{
		printf("\n\r[%s] Download new firmware begin, total size : %d\n\r", __FUNCTION__, rsp_result.body_len);
		for(int i = 0; i < rsp_result.body_len; i++){
			//printf("%c ", rsp_result.body[i]);
		}		
	}

}

int main()
{
	printf("http client program...\n");
	http_update_ota("192.168.0.103", 8082, "test.bin");
	return 0;
}