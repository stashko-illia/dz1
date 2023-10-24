#include <sys/socket.h>
#include <netinet/in.h>
#include <cstddef>
#include <unistd.h>
#include <iostream>

int main(){
    const int PORT = 3456;

    //Створення сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //Налаштування адреси
    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(PORT);
    server_adress.sin_addr.s_addr = INADDR_ANY;
    // Прив'язування сокета до адреси
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));
    
    listen(server_socket, 5);
    char response[1024];
    int client_socket;
    while(true){
        //Очікуємо на запит
        client_socket = accept(server_socket, NULL, NULL);
        recv(client_socket, response, sizeof(response), 0);
        std::cout<<response<<std::endl;
    }
    //Закриваємо сокет
    close(client_socket);

}
