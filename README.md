# Учебная практика
Программа предназначена для анализа текста на русском языке (символы кириллицы). На вход требуется файл с расширением .txt. Программа считывает текст из файла, разбивает его на слова, сортирует их по алфавиту в порядке возрастания и выводит в выходной файл. Также в файл анализа выводится небольшой анализ текста. Название исходного файла вводится с консоли, остальные файла создаются в процессе работы программы.
Погрешность разбивки слов +- 1 слово, при кол-ве слов 2000.

Для разбивки исходного текста на слова используется функция:
```
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
```                                    
Для сортировки используется функция:
```                                  
int sort(vector <string>& array_of_words)
{
    string numbers = "0123456789-"; //цифры и минус, то с чего может начинаться число

    int i, j, step, number_tmp, start_time, end_time;
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

    start_time = clock();
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
    end_time = clock();
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

    return end_time - start_time;
}
```
                                            
Программа работает только с файлами с расширением .txt. 
                                            
Скриношот файла с исходными данными:
![image](https://user-images.githubusercontent.com/104491550/171271753-db42153b-d73f-4bbc-b906-fc96f8868b51.png)
Скриношот файла с отсортированными словами:
![image](https://user-images.githubusercontent.com/104491550/171271824-ff0ead38-cc93-4db6-a693-c9046f25f9ab.png)
Скриношот файла с анализом:
![image](https://user-images.githubusercontent.com/104491550/171271928-2fece4d5-6c67-4df6-9314-4ad2a41e92d6.png)
Скриношот консоли:    
![image](https://user-images.githubusercontent.com/104491550/171272084-6f52908b-24d3-4c03-a275-ab1abcdc51bf.png)
![image](https://user-images.githubusercontent.com/104491550/171272150-83679bcf-022e-4a11-a25a-8255ed2fa160.png)

