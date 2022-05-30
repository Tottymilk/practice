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
bool isNumber(string s);
void sort(vector <string>& array_of_words);
int findInAlphabet(char a);
vector<int> getArrayCountWords(vector<string> array_of_words);
void writeToAnalysis(string name_file, string text, int word_count, vector<int> number_words_array, int time);

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

    //сортировка Шелла
    int start_time = clock();
    sort(array_of_words);
    int end_time = clock();

    //запись в файл result
    writeToResult(name_file, array_of_words);

    //подсчет количества слов на каждую букву
    vector<int> number_words_array = getArrayCountWords(array_of_words);

    int words_count = array_of_words.size(); //количество слов

    //запись в файл analysis 
    writeToAnalysis(name_file, text, words_count, number_words_array, end_time - start_time);

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

void sort(vector <string>& array_of_words)
{
    string numbers = "0123456789-"; //цифры и минус, то с чего может начинаться число

    int i, j, step, number_tmp;
    string tmp;

    vector <string> array_of_numbers;
    vector <string> subarray_of_words;

    //получаем массив чисел и массив слов
    for (i = 0; i < array_of_words.size(); i++)
    {
        if (numbers.find(array_of_words[i][0]) != -1)
            array_of_numbers.push_back(array_of_words[i]);
        else
            subarray_of_words.push_back(array_of_words[i]);
    }

    //сортируем массив слов
    for (step = subarray_of_words.size() / 2; step > 0; step /= 2)
    {
        for (i = step; i < subarray_of_words.size(); i++)
        {
            tmp = subarray_of_words[i];
            number_tmp = findInAlphabet(subarray_of_words[i][0]); //номер в алфавите

            for (j = i; j >= step; j -= step)
            {
                if (number_tmp < findInAlphabet(subarray_of_words[j - step][0]))
                    subarray_of_words[j] = subarray_of_words[j - step];
                else
                    break;
            }
            subarray_of_words[j] = tmp;
        }
    }

    //сортируем массив чисел
    double double_number_tmp, double_number;
    for (step = array_of_numbers.size() / 2; step > 0; step /= 2)
    {
        for (i = step; i < array_of_numbers.size(); i++)
        {
            tmp = array_of_numbers[i];
            if (isNumber(array_of_numbers[i])) //если строка - дробное число 
            {
                double_number_tmp = stod(array_of_numbers[i]); //временной переменной присваиваем число

                for (j = i; j >= step; j -= step)
                {
                    if (isNumber(array_of_numbers[j])) //если строка - дробное число
                    {
                        if (double_number_tmp < stod(array_of_numbers[j - step])) //сравниваем число временной переменной и число array_of_numbers[j - step]
                            array_of_numbers[j] = array_of_numbers[j - step];
                        else
                            break;
                    }
                }
                array_of_numbers[j] = tmp;
            }
        }
    }
    array_of_words = {};

    //сливаем массивы обратно в один массив
    for (i = 0; i < subarray_of_words.size(); i++)
    {
        array_of_words.push_back(subarray_of_words[i]);
    }

    for (i = 0; i < array_of_numbers.size(); i++)
    {
        array_of_words.push_back(array_of_numbers[i]);
    }
}

bool isNumber(string s)
{
    try
    {
        double a = stod(s);
        return 1;
    }
    catch (invalid_argument e)
    {
        return 0;
    }
}

int findInAlphabet(char a)
{
    //функция возвращает индекс вхождения символа в алфавит 
    string low_letters = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    string high_letters = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

    if (high_letters.find(a) != -1)
        return high_letters.find(a);
    else
        return low_letters.find(a);
}

vector<int> getArrayCountWords(vector<string> array_of_words)
{
    vector<int> count_words_array(33);
    //Каждый элемент массив - это количество слов на букву

    string high_letters = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; //алфавит русский
    string low_letters = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    for (int i = 0; i < array_of_words.size(); i++) //идем по всему массиву строк
    {
        if (!isNumber(array_of_words[i]))
        {
            count_words_array[findInAlphabet(array_of_words[i][0])]++;// увеличиваем элемент с индексом равным номеру маленькой буквы в нижнем регистре
        }
    }
    return count_words_array;
}

void writeToAnalysis(string name_file, string text, int word_count, vector<int> number_words_array, int time)
{
    ofstream file_analysis;

    file_analysis.open("analysis_" + name_file + ".txt"); // открываем файл на запись в него

    file_analysis
        << "Исходный текст: " << endl
        << "<<" << text << ">>" << endl
        << "Параметры выбранного варианта (22): кириллица, по алфавиту, по возрастанию, учитывать числа, сортировка шелла" << endl
        << "Количество слов: " << word_count << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " с" << endl
        << "Статистика (количество слов на каждую букву алфавита): " << endl;

    cout
        << "Исходный текст: " << endl
        << "<<" << text << ">>" << endl
        << "Параметры выбранного варианта (22): кириллица, по алфавиту, по возрастанию, учитывать числа, сортировка шелла" << endl
        << "Количество слов: " << word_count << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " с" << endl
        << "Статистика (количество слов на каждую букву алфавита): " << endl;

    string lo_reg = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    //вывод количества слов на каждую букву
    for (int i = 0; i < number_words_array.size(); i++)
    {
        file_analysis << lo_reg[i] << ": " << number_words_array[i] << endl;
        cout << lo_reg[i] << ": " << number_words_array[i] << endl;
    }
    file_analysis.close();
}
