#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>


using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SRV_PORT 1234




struct Student {
    char name[50];
    int grades[4];
} S;

string calcScholarship() {
    bool hasThree = false;
    bool allFives = true;

    for (int i = 0; i < 4; i++) {
        if (S.grades[i] == 3) {
            hasThree = true;
        }
        if (S.grades[i] != 5) {
            allFives = false;
        }
    }

    if (hasThree) {
        return "NO_BONUS";
    }
    else if (allFives) {
        return "HIGH_BONUS";
    }
    else {
        return "STANDARD_BONUS";
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    WSADATA wsa;
    SOCKET servSock, cliSock;
    sockaddr_in servAddr, cliAddr;
    int cliSize = sizeof(cliAddr);

    WSAStartup(MAKEWORD(2, 2), &wsa);

    servSock = socket(AF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(SRV_PORT);

    bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    listen(servSock, SOMAXCONN);

    cout << "TCP сервер запущен... Ожидание данных" << endl;

    while (true) {
        cliSock = accept(servSock, (sockaddr*)&cliAddr, &cliSize);

        char ip[INET_ADDRSTRLEN];
        InetNtopA(AF_INET, &(cliAddr.sin_addr), ip, INET_ADDRSTRLEN);
        cout << "Клиент подключен: " << ip << endl;

        while (true) {
            int received = recv(cliSock, (char*)&S, sizeof(S), 0);

            if (received <= 0) {
                cout << "Клиент отключился" << endl;
                break;
            }

            if (string(S.name) == "exit") {
                cout << "Клиент вышел" << endl;
                break;
            }

            cout << "От " << ip << ": " << S.name << endl;
            cout << "Введите оценки за экзамены: ";
            for (int i = 0; i < 4; i++) {
                cout << S.grades[i] << "  ";
            }

            string bonus = calcScholarship();
            cout << "Стипендия: " << bonus << endl;

            send(cliSock, bonus.c_str(), bonus.size() + 1, 0);
        }

        closesocket(cliSock);
        cout << "Соединение закрыто" << endl;
    }

    closesocket(servSock);
    WSACleanup();
    return 0;
}
