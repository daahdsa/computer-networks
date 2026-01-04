#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <locale>

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
    SOCKET sock;
    sockaddr_in servAddr, cliAddr;
    int cliSize = sizeof(cliAddr);

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(SRV_PORT);

    bind(sock, (sockaddr*)&servAddr, sizeof(servAddr));

    cout << "UDP сервер запущен..." << endl;

    while (true) {
        int received = recvfrom(sock, (char*)&S, sizeof(S), 0,
            (sockaddr*)&cliAddr, &cliSize);

        if (received > 0) {
            if (string(S.name) == "exit") {
                cout << "Клиент вышел" << endl;
                continue;
            }

            char ip[INET_ADDRSTRLEN];
            InetNtopA(AF_INET, &(cliAddr.sin_addr), ip, INET_ADDRSTRLEN);

            cout << "От " << ip << ": " << S.name << endl;
            cout << "Введите оценки за экзамены: ";
            for (int i = 0; i < 4; i++) {
                cout << S.grades[i]<<"  ";
            }

            string bonus = calcScholarship();
            cout << "Стипендия: " << bonus << endl;

            sendto(sock, bonus.c_str(), bonus.size() + 1, 0,
                (sockaddr*)&cliAddr, cliSize);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
