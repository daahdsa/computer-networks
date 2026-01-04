#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")   // библиотека Winsock
#pragma warning(disable: 4996)

using namespace std;

#define PORT 80              // стандартный HTTP-порт
#define BUFFER_SIZE 1024

int main() {
    setlocale(0, "");
    // Инициализация Winsock 
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Создание TCP-сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Настройка адреса сервера
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // любой IP
    serverAddr.sin_port = htons(PORT);

    // Привязка сокета к порту
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // Ожидание подключений клиентов
    listen(serverSocket, SOMAXCONN);

    cout << "HTTP сервер работает на порте 80\n";

    while (true) {
        // Принятие клиента 
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);

        char buffer[BUFFER_SIZE] = { 0 };

        // Получение HTTP-запроса
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        cout << "***********************Запрос от клиента**********************\n";
        cout << buffer << endl;

        // Подготовка HTTP-ответа
        string body = "Working???!!!!\n";
        string body2 = "gvhytghjgftgjfvghhiuy";
        string body0 = body + body2;
        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + to_string(body0.size()) + "\r\n"
            "\r\n" + body0;

        // Отправка ответа клиенту 
        send(clientSocket, response.c_str(), response.size(), 0);

        // Закрытие соединения
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


//telnet localhost 80