#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[1024] = {0};

    // Создаю сокета
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    // Подключаюсь к сокету
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Запускаю прослушку сокета
    if (listen(server_socket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    int client_socket;
    int client_address_length = sizeof(client_address);

    // Сервер принимает подключение (ACK)
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t*)&client_address_length)) < 0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    // Цикл обработки сообщений на сервере от клиента
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        read(client_socket, buffer, 1024);
        std::cout << "Received message from client: " << buffer << std::endl;

        // Отправка подтверждения о получении сообщения
        const char* confirmation = "Message received";
        write(client_socket, confirmation, strlen(confirmation));
    }

    close(server_socket);
    return 0;
}
