#include <stdio.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <string.h>  
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#define I2C_DEV "/dev/i2c-1"//i2c_dev为i2c　adapter创建的别名  


//读操作先发Slaveaddr_W+Regaddr_H+Regaddr_L 3个字节来告诉设备操作器件及两个byte参数  
//然后发送Slaveaddr_R读数据  
static int iic_read(int fd, char buff[], int addr, int count)  
{  
    int res;  
    char sendbuffer1[2]; 

    sendbuffer1[0]=0x20;  
    res = write(fd,sendbuffer1,1);  
	printf("write %d byte at 0x20\n", res);	
	usleep(500);
	
    sendbuffer1[0]=addr;  
    write(fd,sendbuffer1,1);      
	usleep(500);

    sendbuffer1[0]=0x21;  
    write(fd,sendbuffer1,1);        
	usleep(500);

	res=read(fd,buff,count);  
	printf("read %d byte at 0x%x\n", res, addr);  
	return res;  
}  
//在写之前，在数据前加两个byte的参数，根据需要解析  
static int iic_write(int fd, char buff[], int addr, int count)  
{  
	int res;  
	int i,n;  
	static char sendbuffer[100];  
	memcpy(sendbuffer+2, buff, count);  
	sendbuffer[0]=addr>>8;  
	sendbuffer[1]=addr;  
	res=write(fd,sendbuffer,count+2);  
	printf("write %d byte at 0x%x\n", res, addr);  
} 

int main( int argc, char *argv[] )
{
	printf("Hello World!\n");
	int fd;  
    int res;  
    char ch;  
    char buf[50];  
    int regaddr = 0x00, i = 0, slaveaddr = 0x21;  //broadcast addr
    fd = open(I2C_DEV, O_RDWR);// I2C_DEV /dev/i2c-0  
	if(fd < 0){  
			printf("####i2c test device open failed####/n");  
			return (-1);  
	}
	printf("open i2c-1 success\n");
	
	res = ioctl(fd,I2C_TENBIT,0);   //not 10bit  
	printf("I2C_TENBIT ioctl return:%d\n", res);
	res = ioctl(fd,I2C_SLAVE,slaveaddr);    //设置I2C从设备地址[6:0] 
	printf("I2C_SLAVE ioctl return:%d\n", res);
	
	regaddr = 0x00;
	buf[0]=0xAA;  
	for(int i = 0; i < 1; i ++){
		res=iic_read(fd,buf,regaddr,1);
		printf("read reg 0x%02x\n", buf[0]);	
		sleep(1);
	}

	
	return 0;
}