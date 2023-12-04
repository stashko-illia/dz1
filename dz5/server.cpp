#include <sys/socket.h>
#include <netinet/in.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main(){
    const int PORT = 5555;
    //Налаштування адреси
    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(PORT);
    server_adress.sin_addr.s_addr = INADDR_ANY;

    //Створення сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Прив'язування сокета до адреси
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

    listen(server_socket, 5);    
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(server_socket, (sockaddr *)&newSockAddr, &newSockAddrSize);
    std::cout << "Connected with client!" << std::endl;

    char msgSend[2048];
    while(1){


        char length[4]; // 4 bytes for size of the message
        recv(newSd, (char *)&length, 4, 0); // receiving size

        int sizeOfReceive = atoi(length) + 1; // converting to integer
        char* msgReceived = new char[sizeOfReceive]; //allocating memore for message

        int receiveRes = recv(newSd, msgReceived, sizeOfReceive, 0); // receiving the message

        if(receiveRes == 0){
            std::cout << "Connection has been lost :(.\n";
            break;
        }
        std::cout << "Client: " << msgReceived << '\n';

        std::cout << "Reply: "; // write a reply
        std::string data;
        getline(std::cin, data);
        strcpy(msgSend, data.c_str());

        send(newSd, std::to_string(strlen(msgSend)).c_str(), 4, 0);//sending size
        send(newSd, (char *)msgSend, strlen(msgSend), 0); //sending message
        
        memset(&msgSend, 0, sizeof(msgSend)); // memore clean up
        delete msgReceived;
    }
    close(newSd);
    close(server_socket);
}
