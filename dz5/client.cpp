#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

int main(){
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5555); // Server port number
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    std::cout<<"Sucessfully connected to the server.\n";
    
    char msgSend[2048];
    while(1){
    
        std::cout<<"Enter your message: ";
        std::string data;
        getline(std::cin, data);
        strcpy(msgSend, data.c_str());

        send(client_socket, std::to_string(strlen(msgSend)).c_str(), 4, 0); //sending size
        send(client_socket, (char*)&msgSend, strlen(msgSend), 0); // sengding message

        char temp[4];// 4 bytes for size of the message
        recv(client_socket, (char *)&temp, 4, 0);// receiving size

        int sizeOfReceive = atoi(temp) + 1;// converting to integer
        char* msgReceived = new char[sizeOfReceive];//allocating memore for message

        int receiveRes = recv(client_socket, msgReceived, sizeOfReceive, 0);// receiving the message
        
        std::cout<<"Server replies: "<<msgReceived << '\n';

        std::memset(&msgSend, 0, sizeof(msgSend)); // memore clean up
        delete msgReceived;

    }
    close(client_socket);
}