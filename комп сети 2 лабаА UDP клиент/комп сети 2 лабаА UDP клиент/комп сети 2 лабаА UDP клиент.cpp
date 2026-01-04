#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <locale>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define SRV_HOST "127.0.0.1"
#define SRV_PORT 1234
#define BUF_SIZE 64

struct Student {
    char name[50];
    int grades[4];
} S;

string getBonusText(const string& code) {
    if (code == "NO_BONUS") return "Без стипендии((";
    else if (code == "STANDARD_BONUS") return "Стандартная стипендия";
    else if (code == "HIGH_BONUS") return "Повышенная стипендия (молодец!!)";
    else return "Неизвестный код";
}

int main() {
    setlocale(LC_ALL, "Russian");

    WSADATA wsa;
    SOCKET sock;
    sockaddr_in servAddr;
    int addrSize = sizeof(servAddr);

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SRV_PORT);
    InetPtonA(AF_INET, SRV_HOST, &servAddr.sin_addr);

    cout << "UDP сервер подключен!" << endl;
    cout << "Вводите данные или 'exit' для выхода" << endl;

    while (true) {
        cout << "\n---  Студентик ---" << endl;
        cout << "Имя (или 'exit'): ";
        cin.getline(S.name, 50);

        if (string(S.name) == "exit") {
            sendto(sock, (char*)&S, sizeof(S), 0,
                (sockaddr*)&servAddr, addrSize);
            cout << "Выход..." << endl;
            break;
        }

        cout << "Введите оценки за экзамены: ";
        for (int i = 0; i < 4; i++) {
            cin >> S.grades[i];
        }

        cin.ignore();

        sendto(sock, (char*)&S, sizeof(S), 0,
            (sockaddr*)&servAddr, addrSize);

        cout << "Данные отправлены" << endl;

        char bonusCode[50];
        int received = recvfrom(sock, bonusCode, sizeof(bonusCode), 0,
            (sockaddr*)&servAddr, &addrSize);

        if (received > 0) {
            string code(bonusCode);
            string bonusText = getBonusText(code);

            cout << "\n>>> РЕЗУЛЬТАТ <<<" << endl;
            cout << "Студент: " << S.name << endl;
            cout << "Стипендия: " << bonusText << endl;
            cout << "----------------" << endl;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
