#include <iostream>
#include <string>
#include <iostream>
#include <Windows.h> 
#include <vector> 
#include <fstream>

using namespace std;

vector<string> separateWords(string text);
string getText(string name_file);
void writeToResult(string name_file, vector<string> array_of_words);

int main()
{
    setlocale(0, ""); // поддержка кириллицы в консоли (вывод)

    string name_file; //название исходного файла

    cout << "Введите номер файла: ";
    cin >> name_file;

    //функция получения текста
    string text = getText(name_file);

    //функция разбивает текст на слова 
    vector <string> array_of_words = separateWords(text);

    return 0;
}

string getText(string name_file)
{
    string text = ""; //исходная строка
    ifstream file_original; //создаем переменную файла

    file_original.open("original_" + name_file + ".txt"); // открываем файл

    if (file_original.is_open()) //если файл открылся 
    {
        char ch;
        while (file_original.get(ch)) //пока можем считать символ из файла
        {
            text += ch;; //прибавляем символ к строке 
        }
        file_original.close(); // Закрытие файла
    }
    else
    {
        cout << "Файл original не открылся";
        exit(0);
    }
    return text;
}

vector<string> separateWords(string text)
{
    vector<string> array_of_words; //массив слов
    string s = "";

    text = '"' + text + '"'; //добавляем кавычки
    text = '"' + text + '"'; //добавляем кавычки

    string high_letters = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; //алфавит русский
    string low_letters = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    string numbers = "0123456789";

    for (int i = 2; i < text.length() - 2; i++)
    {
        if (high_letters.find(text[i]) != -1 || low_letters.find(text[i]) != -1) //если символ строки буква из кириллицы

        {
            s += text[i]; //собираем слово 
            if (high_letters.find(text[i + 1]) == -1 && low_letters.find(text[i + 1]) == -1 && text[i + 1] != '-')
            {
                array_of_words.push_back(s); //добавляем в массив строку
                s = ""; //обнуляем строку
            }
        }
        else
        {
            if (numbers.find(text[i]) != -1 || text[i] == ',' && numbers.find(text[i + 1]) != -1) //если символ строки цифра или запятая 
            {
                s += text[i]; //собираем слово 
                if (numbers.find(text[i + 1]) == -1 && text[i + 1] != ',' || numbers.find(text[i + 2]) == -1 && text[i + 1] == ',') //следующий символ не цифра и не запятая или след символ запятая а после не цифра
                {
                    array_of_words.push_back(s); //добавляем в массив строку
                    s = ""; //обнуляем строку
                }
            }
            else
            {

                if (text[i] == '-' && (numbers.find(text[i + 1]) != -1 || ((high_letters.find(text[i + 1]) != -1 || low_letters.find(text[i + 1]) != -1) && s != ""))) //если символ строки минус и следующий символ цифра
                {
                    s += text[i]; //собираем слово 
                }
            }
        }
    }
    return array_of_words;
}

void writeToResult(string name_file, vector <string> array_of_words)
{
    ofstream file_result;
    file_result.open("result_" + name_file + ".txt"); // открываем файл на запись в него

    //вывод слов
    for (int i = 0; i < array_of_words.size(); i++)
    {
        file_result << array_of_words[i] << endl;
    }
    file_result.close();
}
