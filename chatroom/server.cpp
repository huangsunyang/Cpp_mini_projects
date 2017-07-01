#include "utility.h"


int main() {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    int listener = socket(PF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("Listener");
        exit(-1);
    }
    
    std::cout << "Listen socket created" << std::endl;
    
    if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }
    
    int ret = listen(listener, 5);
    if (ret < 0) {
        perror("listen error");
        exit(-1);
    }
    
    std::cout << "Start to listen" << std::endl;

    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }
    std::cout << "epoll created. epollfd = " << epfd;
    static struct epoll_event events[EPOLL_SIZE];

    addfd(epfd, listener, true);

    while (true) {
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (epoll_events_count < 0) {
            perror("epoll failure");
            break;
        }

        std::cout <<"epoll_enents_count = " << epoll_events_count << std::endl;
        for (int i = 0; i < epoll_events_count; i++) {
            int sockfd = events[i].data.fd;

            if (sockfd == listener) {
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof(struct sockaddr_in);
                int clientfd = accept(listener, (struct sockaddr*) &client_address, &client_addrLength);
            

                std::cout << "client connection from" << inet_ntoa(client_address.sin_addr) << " :"
                    << ntohs(client_address.sin_port) << "(IP:PORT), clientfd = " << clientfd << std::endl;

                addfd(epfd, clientfd, true); 

                clients_list.push_back(clientfd);

                std::cout << "add new clientfd = " << clientfd << " to epoll" << std::endl;
                std::cout << "Now there are " << clients_list.size() << "clients in the chat room" << std::endl;

                std::cout << "weoclome message" << std::endl;
                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientfd);

                int ret = send(clientfd, message, BUF_SIZE, 0);
                if (ret < 0) {
                    perror("send error");
                    exit (-1);
                }
            } else {
                int ret = sendBroadcastmessage(sockfd);
                if (ret < 0) {
                    perror("send error");
                    exit (-1);
                }
            }
        }
    }
    close(listener);
    close(epfd);
    return 0;
}
                
