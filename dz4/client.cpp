#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3456); // Server port number
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    char buffer[1024] = "Not Hello Server!";
    send(client_socket, buffer, sizeof(buffer), 0);
    close(client_socket);
}