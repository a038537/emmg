// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include "parser.h" 
   

//     int BytesSent, nlen;
int i;
int timer;
uint8_t frame[255];
char tbuffer[33];
bool fileopen = 0;
bool fileerr = 0;
FILE *fp = NULL;
char temm[200] = {0};

int main(int argc, char const *argv[]) 
{ 
    unsigned int port;
    if(argc != 2){
        fprintf(stderr,"No Port defined! Exiting...\n");
        return -1;
    }

    port = atoi(argv[1]);
    readkeys();
    int sock = 0, BytesSent; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    char sendbuf[1024];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

     setup(0,sendbuf);
     send(sock, sendbuf, ((sendbuf[4]+5) &0xff ), 0);
     recv(sock , buffer, 1024,0);
     printf("\n");
     printf("Incoming Message\n");
	 for(i =0;i < buffer[4]+5;i++){printf("%02X ",buffer[i]&0xff);}
     printf("\n");

     setup(1,sendbuf);
     send(sock, sendbuf, ((sendbuf[4]+5) &0xff ), 0);
     recv(sock , buffer, 1024,0);
     printf("\n");
     printf("Incoming Message\n");
	 for(i =0;i < buffer[4]+5;i++){printf("%02X ",buffer[i]&0xff);}
     printf("\n");

     setup(2,sendbuf);
     send(sock, sendbuf, ((sendbuf[4]+5) &0xff ), 0);
     recv(sock, buffer, 1024,0);
     printf("\n");
     printf("Incoming Message\n");
	 for(i =0;i < buffer[4]+5;i++){printf("%02X ",buffer[i]&0xff);}
     printf("\n");


while(true){

    if(timer <= time(0)){

//    ifstream file("database.bin", ios::in|ios::binary|ios::ate);

	if((!fileopen) && (!fileerr)){
		fp = fopen("./emm.txt", "rt");
		if(fp == NULL){
			fprintf(stderr,"\n\nemm.txt not found!\n\n");
			fileerr = 1;
		} else {
			fprintf(stderr,"\n\nemm.txt opened\n\n ");
			//fprintf(stderr, "%s\n\n", ctime(&now));
			fileopen = 1;
		}
	}



    if (fileopen)
    {
        //file.seekg(0, ios::beg);
        while(!feof(fp)){
            fgets(temm,200,fp);
			//printf(temm);
			for(int idx = 0;idx < 33;idx++){
				sscanf(&temm[idx*3],"%hhx ",&tbuffer[idx]);
			}
					
			//file.read (tbuffer,33);
            parse(frame,tbuffer);
            printf("EMM to SEND: \n");
            for (int i=0;i <=(frame[2]+2) ;i++){printf("%02X ",frame[i]);}
            printf("\n");

            genframe(sendbuf);
            copyemm(sendbuf,frame);

            for(int i =0;i < 0xcf;i++){printf("%02X ",sendbuf[i]&0xff);}
            printf("\n");
            BytesSent = send(sock, sendbuf, ((sendbuf[4]+5) &0xff ), 0);

			if(BytesSent == SO_ERROR){
				printf("Client: send() error.\n");
			} else {
				printf("Client: send() is OK - bytes sent: %d\n", BytesSent);
			}
			usleep(50000);
        }
		fclose(fp);
		fileopen = 0;
		printf("Database processed... Continue sending NULL-Packets\n");
		timer = (time(0)+120);
    } else {
		printf("ERROR: database.bin not found! \n");
		timer = (time(0)+120);
	}
    }


	genframe(sendbuf);
	BytesSent = send(sock, sendbuf, ((sendbuf[4]+5) &0xff ), 0);

	if(BytesSent == SO_ERROR){
          printf("Client: send() error .\n");
	} else {
          //printf("Client: send() is OK - bytes sent: %d\n", BytesSent);
    }

usleep(50000);
}
    return 0; 
} 
