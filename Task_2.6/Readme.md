## Задание 2.6.	Реализовать простой синтезатор частот.

## Requirements
Программа гарантированно работает на следующих системах:
#### Windows 10x64
- IDE **CLion 2021.3.4**
- Компилятор **bundled MinGW**
- Система сборки **bundled CMake**

#### Windows Subsystem for Linux
- Образ **Ubuntu 18.04.5 LTS**
- IDE **CLion 2021.3.4**
- Компилятор **gcc/g++**
- Система сборки **Cmake 3.10.2**

Для анализа спектра звука использовалась Adobe Audition 2022.

## Запуск и работа
Запускается из main.cpp, исполняемый файл - Task_2.6.exe/Task_2.6.
#### Входные данные
1) Ввести частоту дискретизации файла (в Гц)
2) Ввести путь к файлу (*\*/\*.wav*), чтобы сохранить получившийся трек
3) Последовательно (по строкам) вводить звуки в формате **<частота звучания> <длительность>**. Частота указывается в Гц, длительность в с.
4) Для завершения и сохранения файла, напишите "exit" без кавычек.

#### Выходные данные
Моно-аудиофайл \*.wav с набором мелодий, введенном в п. 3. Продолжительность аудиофайла (в секундах) - сумма продолжительностей мелодий из п. 3.

## Примеры
#### Одна нота
Консольные данные:

![image](https://user-images.githubusercontent.com/99473127/179425011-62fdecc2-27cd-4365-9dfe-322a939112f2.png)

Частотный спектр:

![image](https://user-images.githubusercontent.com/99473127/179425026-6f67f066-485f-4222-b0c9-b10ea1322c01.png)

Тональный спектр:

![image](https://user-images.githubusercontent.com/99473127/179425034-2f4c8a9f-5ecf-45ac-ad49-ac9c6e088248.png)

Форма звуковой волны:

![image](https://user-images.githubusercontent.com/99473127/179425129-7b162e4f-ea96-472c-97aa-2c5a7a675dd1.png)

#### Несколько нот
Консольные данные:

![image](https://user-images.githubusercontent.com/99473127/179424826-ed7e86a0-33b9-4e43-a965-31e6baa859ac.png)

Частотный спектр:

![image](https://user-images.githubusercontent.com/99473127/179424845-2fe364cb-1cfb-4b80-a7c1-9c11eff4a586.png)

Тональный спектр:

![image](https://user-images.githubusercontent.com/99473127/179424944-f5d2bbcf-d4a4-49a6-b4d9-ace98227398e.png)

Форма звуковой волны на стыке нот:

![image](https://user-images.githubusercontent.com/99473127/179425096-e71e934f-93b7-4131-9a4a-294722f734a0.png)
