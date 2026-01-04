//программа-клиент

#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

struct Person {
    char name[25];
    int height;
    int weight;
};

int main() {
    setlocale(LC_ALL, "rus");

    string nameR = "C:\\server\\REQUEST.txt";
    string nameA = "C:\\server\\ANSWER.txt";

    Person B;
    int answer;

    cout << "********* КЛИЕНТ *********" << endl;

    cout << "Введите имя: ";
    cin >> B.name;
    cout << "Введите рост(cм): ";
    cin >> B.height;
    cout << "Введите вес(кг): ";
    cin >> B.weight;

    // Удаляем старый файл ответа если существует
    remove(nameA.c_str());

    // Записываем запрос
    ofstream fR(nameR);
    if (!fR.is_open()) {
        cout << "Ошибка создания ответа от файла" << endl;
        cout << "Проверьте папку С:/server" << endl;

        // Попробуем создать в текущей директории
        cout << "Пробуем создать файл в нужной директории" << endl;
        nameR = "REQUEST.bin";
        nameA = "ANSWER.bin";

        fR.open(nameR, ios::binary);
        if (!fR.is_open()) {
            cout << "Не удалось оздать файл" << endl;
            cout << "Нажмите Enter для выхода";
            cin.ignore();
            cin.get();
            return 1;
        }
    }

    fR.write((char*)&B, sizeof(B));
    fR.close();

    cout << "Запрос отправлен, ожидание ответа..." << endl;

    // Ждем ответ (максимум 10 секунд)
    bool gotResponse = false;
    for (int i = 0; i < 100; i++) {
        ifstream fA(nameA);
        if (fA.is_open()) {
            fA.read((char*)&answer, sizeof(answer));
            if (fA.gcount() == sizeof(answer)) {
                gotResponse = true;
                fA.close();
                break;
            }
            fA.close();
        }
        cout << "."; // Прогресс
        Sleep(100); // Ждем 100ms
    }
    cout << endl;

    if (gotResponse) {
        cout << "************** РЕЗУЛЬТАТ ***************" << endl;
        cout << "Имя: " << B.name << endl;
        cout << "Рост: " << B.height << "cм, Вес: " << B.weight << "кг" << endl;
        cout << "Результат по введенным данным: ";
        switch (answer) {
        case 0: cout << "недобор веса"; break;
        case 1: cout << "нормальный вес"; break;
        case 2: cout << "избыточный вес"; break;
        default: cout << "неизвестно"; break;
        }
        cout << endl;
    }
    else {
        cout << "Ошибка: не ответа от сервера" << endl;
        cout << "Проверьте работу сервера и нахождение файлов в указанной папке" << endl;
    }

    cout << "Нажмите Enter для выхода";
    cin.ignore();
    cin.get();

    return 0;
}