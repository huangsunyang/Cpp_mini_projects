#include "utility.h" 


int main() {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("sock error");
        exit(-1); 
    }
    
    if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("pipe error");
        exit(-1);
    }

    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }

    static struct epoll_event events[2];

    addfd(epfd, sock, true);
    addfd(epfd, pipe_fd[0], true);

    bool isClientwork = true;

    char message[BUF_SIZE];

    int pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) {
        close(pipe_fd[0]);
        std::cout << "Please input 'exit' to exit the chat room" << std::endl;

        while (isClientwork) {
            bzero(&message, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            if(strncasecmp(message, EXIT, strlen(EXIT)) == 0) {
                isClientwork = false;
            } else if (write(pipe_fd[1], message, strlen(message) - 1) < 0) {
                perror("fork error");
                exit(-1);
            }
        }
    } else {
        close(pipe_fd[1]);

        while (isClientwork) {
            int epoll_events_count = epoll_wait(epfd, events, 2, -1);
            for (int i = 0; i < epoll_events_count; i++) {
                bzero(&message, BUF_SIZE);
                if (events[i].data.fd == sock) {
                    int ret = recv(sock, message, BUF_SIZE, 0);

                    if (ret == 0) {
                        std::cout << "Server closed connection " << sock << std::endl;
                        close(sock);
                        isClientwork = false;
                    } else {
                        std::cout << message << std::endl;
                    }
                } else {
                    int ret = read(events[i].data.fd, message, BUF_SIZE);
                    if (ret == 0) {
                        isClientwork = 0;
                    } else {
                        send(sock, message, BUF_SIZE, 0);
                    }
                }
            }
        }
    }

    if (pid) {
        close(pipe_fd[0]);
        close(sock);
    } else {
        close(pipe_fd[1]);
    }
    return 0;
}
