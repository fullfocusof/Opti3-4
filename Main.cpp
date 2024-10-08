#include "SymplexInterraction.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void GoToXY(short x, short y)
{
	SetConsoleCursorPosition(hStdOut, { x, y });
}

void ConsoleCursorVisible(bool show, short size)
{
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hStdOut, &structCursorInfo);
	structCursorInfo.bVisible = show;
	structCursorInfo.dwSize = size;
	SetConsoleCursorInfo(hStdOut, &structCursorInfo);
}

int main()
{
	setlocale(LC_ALL, "ru");
	SetConsoleTitle(L"Симплекс-метод");
	ConsoleCursorVisible(false, 100);

	SymplexInterraction test;

	int active_menu = 0;
	int keyInput;
	bool exitProg = false;

	while (!exitProg)
	{
		int x = 40, y = 12;
		GoToXY(x, y);

		vector<string> menu =
		{
			"Импорт данных из файла",
			"Просмотр данных",
			"Итерация симплекс-метода",
			"Выход"
		};

		for (int i = 0; i < menu.size(); i++)
		{
			if (i == active_menu)
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else
			{
				SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
			}

			GoToXY(x, y++);
			cout << menu[i] << endl;
		}

		keyInput = _getch();
		switch (keyInput)
		{
			case KEY_ESC:
				exit(0);

			case KEY_UP:
			{
				if (active_menu > 0)
				{
					active_menu--;
				}
			}
			break;

			case KEY_DOWN:
			{
				if (active_menu < menu.size() - 1)
				{
					active_menu++;
				}
			}
			break;

			case KEY_ENTER:
			{
				switch (active_menu)
				{
					case 0:
					{
						system("cls");

						try
						{
							test.readFromFile("dataInput.txt");
							cout << "Успешный импорт данных";
						}
						catch (const exception& ex)
						{
							cout << ex.what();
						}
					
						printQuit();
					}
					break;

					case 1:
					{
						system("cls");

						try
						{
							test.printData();
						}
						catch (const exception& ex)
						{
							cout << ex.what();
						}
					
						printQuit();
					}
					break;

					case 2:
					{
						system("cls");

						try
						{
							test.symplexIteration("dataOutput.txt");
							cout << "Итерация симплекс-метода проведена успешно";
						}
						catch (const exception& ex)
						{
							cout << ex.what();
						}

						printQuit();
					}
					break;

					case 3:
					{
						system("cls");
						exitProg = true;
					}
					break;
				}
			}
			break;
		}
	}
}