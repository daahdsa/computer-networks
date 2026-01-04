#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>


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

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SRV_PORT);
    InetPtonA(AF_INET, SRV_HOST, &servAddr.sin_addr);

    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == 0) {
        cout << "Подключение к серверу установлено!" << endl;
        cout << "Вводите данные или 'exit' для выхода" << endl;

        while (true) {
            cout << "\n--- Студентик ---" << endl;
            cout << "Имя (или 'exit'): ";
            cin.getline(S.name, 50);

            if (string(S.name) == "exit") {
                send(sock, (char*)&S, sizeof(S), 0);
                cout << "Выход..." << endl;
                break;
            }

            cout << "Введите оценки за экзамены: ";
            for (int i = 0; i < 4; i++) {
                cin >> S.grades[i];
            }

            cin.ignore();

            send(sock, (char*)&S, sizeof(S), 0);
            cout << "Данные отправлены" << endl;

            char bonusCode[BUF_SIZE];
            int received = recv(sock, bonusCode, sizeof(bonusCode), 0);

            if (received > 0) {
                string code(bonusCode);
                string bonusText = getBonusText(code);

                cout << "\n**********РЕЗУЛЬТАТ**********" << endl;
                cout << "Студент: " << S.name << endl;
                cout << "Стипендия: " << bonusText << endl;
                cout << "----------------" << endl;
            }
        }
    }
    else {
        cout << "Ошибка подключения!" << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
