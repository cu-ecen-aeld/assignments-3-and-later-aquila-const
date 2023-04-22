#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9000"

#define BUFFER_SIZE 1024

#define TARGET_FILE "/var/tmp/aesdsocketdata"

struct addrinfo *serverinfo = NULL;

FILE *outfile = NULL;
FILE *clientdata = NULL;

int sck_fd = 0;
int s_fd = 0;

volatile sig_atomic_t sig_state = 0;

/**
 * Helper funcs
*/

void destroy_socket()
{

    if(clientdata) fclose(clientdata);
    if(outfile) fclose(outfile);

    if(serverinfo) freeaddrinfo(serverinfo);
    
    syslog(LOG_INFO, "Cleanup complete.");
    
}

void sig_handler_(int signal)
{
    if(signal == SIGINT || signal == SIGTERM || signal == SIGKILL){ 
      syslog(LOG_INFO, "Caught signal.");
      sig_state = 1;
    }
    destroy_socket();

    remove(TARGET_FILE);
    exit(0);
}


void init_sig_handler_(struct sigaction sigact){
    sigact.sa_handler = sig_handler_;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGKILL, &sigact, NULL);

}

void init_sock_struct(struct addrinfo hints)
{
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

void cleanup(){
    fclose(outfile);
    outfile = NULL;
    fclose(clientdata);
    clientdata = NULL;
    close(s_fd);
    s_fd = 0;
    syslog(LOG_INFO, "Shutting down server.");
}
/**
 * End helper funcs
*/
int main(int argc, char *argv[]){

    char * input = NULL;
    char packet_buff[BUFFER_SIZE];

    struct addrinfo hints;
    struct sockaddr their_addr;
    
    int s_fd = 0;
    int lis_status = 0;
    struct sigaction old_act = {0};
    
    
    socklen_t their_addr_size;
    size_t len = 0;
    size_t size = 0;
    const int sock_opt = 1;

    init_sig_handler_(old_act);

    memset(&hints, 0, sizeof(hints));

    init_sock_struct(hints);
    
    if (getaddrinfo("localhost", PORT, &hints, &serverinfo)){
        return -1;
    }

    sck_fd = socket(PF_INET, SOCK_STREAM, 0);

    if(sck_fd == -1)
    {
        close(sck_fd);
        return (-1);
    }

    if(setsockopt(sck_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt))){
        return -1;
    }

    if((bind(sck_fd, serverinfo->ai_addr, serverinfo->ai_addrlen)) != 0)
    {
        close(sck_fd);
        return -1;
    }
    pid_t pid = fork();
    
    if(argc == 2){
        if(strcmp(argv[1], "-d") == 0){
            if(pid == -1)
            {
                return -1;
            }
            if(pid){
                printf("Waiting for connection request...\n");

                exit(EXIT_SUCCESS);
            }
        }
    }

    
    if((lis_status = listen(sck_fd, 10)) == -1)
    {
        perror("listen");
        exit(1);
    } 

    while(1)
    {
        
        their_addr_size = sizeof their_addr;

        s_fd = accept(sck_fd, (struct sockaddr *)&their_addr, &their_addr_size);

        if(s_fd == -1)
        {
            perror("accept");
            continue;
        }

        clientdata = fdopen(s_fd, "rb");
    
        if(clientdata){
            input = NULL;
            len = 0;
            if(getline(&input, &len, clientdata))
            {
                outfile = fopen(TARGET_FILE, "a+");
            
                fputs(input, outfile);
                fflush(outfile);
                free(input);

                size = 0;

                fseek(outfile, 0, SEEK_SET);

                while((size = fread(packet_buff, sizeof(char), BUFFER_SIZE, outfile)) > 0)
                send(s_fd, packet_buff, size, 0);
                cleanup();
            }
        }
    }
    
    exit(EXIT_SUCCESS);
}