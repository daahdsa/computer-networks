//программа-сервер

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

    // Используем абсолютные пути
    string nameR = "C:\\server\\REQUEST.txt";
    string nameA = "C:\\server\\ANSWER.txt";

    Person B;
    int answer;

    cout << "Сервер работает и ждет запросов..." << endl;
    cout << "файл запроса: " << nameR << endl;
    cout << "файл ответа: " << nameA << endl;

    while (true) {
        // Пытаемся прочитать запрос
        ifstream fR(nameR);
        if (fR.is_open()) {
            fR.read((char*)&B, sizeof(B));
            if (fR.gcount() == sizeof(B)) {
                cout << "**********НОВЫЙ ЗАПРОС***********" << endl;
                cout << "Имя: " << B.name << endl;
                cout << "Рост: " << B.height << "cм, Вес: " << B.weight << "кг" << endl;

                // Вычисляем ИМТ
                double height_m = B.height * 0.01;
                double IMT = B.weight / (height_m * height_m);

                cout << "Индекс массы тела: " << IMT << endl;

                if (IMT < 18.5) {
                    answer = 0;
                    cout << "Результат: недобор веса" << endl;
                }
                else if (IMT < 25) {
                    answer = 1;
                    cout << "Результат: норма" << endl;
                }
                else {
                    answer = 2;
                    cout << "Результат: масса тела выше нормы " << endl;
                }

                // Записываем ответ
                ofstream fA(nameA);
                if (fA.is_open()) {
                    fA.write((char*)&answer, sizeof(answer));
                    fA.close();
                    cout << "Данные успешно отправлены!" << endl;
                }
                else {
                    cout << "Ошибка: не удалось создать файл ответа" << endl;
                }

                // Удаляем обработанный запрос
                fR.close();
                if (remove(nameR.c_str()) != 0) {
                    cout << "Не удалось удалить запрос" << endl;
                }

                cout << "Следующий запрос?" << endl << endl;
            }
            else {
                fR.close();
            }
        }
        Sleep(1000); // Проверяем каждую секунду
    }
    return 0;
}