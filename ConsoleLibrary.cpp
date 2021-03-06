
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <limits>
#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <list>
#include <algorithm>
#include <iterator>
#include <locale>
#include <time.h>
#undef max
#define WIDTH 80
#define HEIGHT 22

using namespace std;

string new_file_coding(string user_key, string user_text);
string open_file_decode(string decode_key, string decode_text);
string login, password;
int counter = 0;


void AddNewFile() {
    wchar_t * in_directory = new wchar_t[MAX_PATH]; //директория текстового файла
	char in_directory_m[MAX_PATH];
	wchar_t * in_buff = new wchar_t[MAX_PATH]; //имя файла
	wchar_t * out_directory = new wchar_t[MAX_PATH];//корневая директория
	char name[64];
	string lib_buffer;
	int in_member = 0, out_member = 0, buff_member; 
	GetCurrentDirectory(MAX_PATH, out_directory); // получаем корневую директорию
	for (; out_directory[out_member] != '\0'; out_member++) {}; //пролистываем ее до конца и ставим обратный слэш
	out_directory[out_member] = '\\';
	out_member++;
	ifstream in_file, lib_file;
	ofstream lib_file_in;
	lib_file.open("library.txt");
	do { 
		cout << "Введите директорию файла(например C:\\user\\documents\\my_book.txt) или введите \'q\' для выхода в меню: " << endl;  
		cin.getline(in_directory_m, 260);
		OemToCharA(in_directory_m, in_directory_m);
		mbstowcs(in_directory, in_directory_m, MAX_PATH);
		cout << endl;
		if (in_directory_m[0] == 'q') {
			system("cls");
			return;
		}
		in_file.open(in_directory);
		if (!in_file.is_open()) {
			cout << "Произошла ошибка! Невозможно открыть этот файл!\n";
		}
	} while (!in_file.is_open());

	for (; in_directory[in_member] != '\0'; in_member++) {} //пролистываем до конца
	in_member--;
	for (buff_member = in_member; in_directory[buff_member] != '\\'; buff_member--) {} //и с конца до первого обратного слэша
	buff_member++;
	for (int i = 0; buff_member <= in_member; buff_member++, i++) { //скидываем в in_buff название файла
		in_buff[i] = in_directory[buff_member];
		in_buff[i + 1] = '\0';
	}
	for (int i = 0; in_buff[i] != '\0'; out_member++, i++) { //ставим в корневую директорию название файла
		out_directory[out_member] = in_buff[i];
	}
	out_directory[out_member] = '\0';
	wcstombs(name, in_buff, 64);
	CopyFile(in_directory, out_directory, true);
	for (;;) {
		getline(lib_file, lib_buffer);
		if (lib_buffer == name) {
			cout << "Этот файл уже существует!\n";
			cin.get();
			system("cls");
			return;
		}
		else if (lib_buffer.empty()) {
			break;
		}
	}

	lib_file.close();
	in_file.close();
	lib_file_in.open("library.txt", ios_base::app);
	cout << "Файл успешно добавлен в библиотеку!\n";
	lib_file_in << name << endl;
	lib_file_in.close();
	cin.get();
	system("cls");
}

void TextViewer(string bookname) {
	system("cls");
	ofstream lstbk;
	string lsbk_dir = login + "lstbk.txt", lstbk_buffer;
	if (counter > 10) {
		remove(lsbk_dir.c_str());
	}
	lstbk.open(lsbk_dir.c_str(), ios_base::app);
	lstbk_buffer = new_file_coding(password, bookname);
	lstbk << lstbk_buffer << endl;
	lstbk.close();
	string adress = bookname + ".txt", buffer;
	ifstream book;
	book.open(adress);
	if (!book.is_open()) {
		cout << "Такой книги не существует!";
	}
	else {
		counter++;
		for (;!book.eof();) {
			getline(book, buffer);
            cout << buffer << endl;
		}
		HANDLE hCon;
		COORD cPos;

		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		cPos.Y = 0;
		cPos.X = 0;
		SetConsoleCursorPosition(hCon, cPos);
	}
	book.close();
	cin.get();
	system("cls");
}

void SearchAndRedact() {
	system("cls");
	char ch_buff[MAX_PATH];
	string book_buffer, second_buffer, search_book, new_name_book, delete_book;
	list<string> books;
	int option_redact;
	while (true) {
		ifstream list;
		ifstream r_list;
		ofstream new_list;
		list.open("library.txt");
		if (list.is_open()) {
			r_list.open("library.txt");
			cout << "Подменю поиска книг по библиотеке и редактированию.\n1)Показать список всех книг.\n2)Найти книгу по названию.\n3)Переименновать книгу.\n4)Сортировать в алфавитном порядке.\n5)Удалить книгу.\n6)Назад.\n";
			cin >> option_redact;
			cin.get();
			switch (option_redact) {
			case 1:
				system("cls");
				while (!list.eof()) {
					getline(list, book_buffer);
					if (!book_buffer.empty()) {
						cout << book_buffer << endl;

					}
				}
				book_buffer.clear();
				cin.get();
				system("cls");
				break;
			case 2:
				system("cls");
				cout << "Введите название книги: ";
				getline(cin, search_book);
				OemToCharA(search_book.c_str(), ch_buff);
				search_book = ch_buff;
				search_book += ".txt";
				for (;;) {
					getline(list, book_buffer);
					if (book_buffer == search_book) {
						cout << book_buffer << endl;
						break;
					}
					else if (list.eof()) {
						cout << "Такой книги нет в библиотеке!\n";
						break;
					}
				}
				cin.get();
				system("cls");
				break;
			case 3:
				system("cls");
				cout << "Введите название книги, которую вы хотите переименновать: ";
				getline(cin, search_book);
				OemToCharA(search_book.c_str(), ch_buff);
				search_book = ch_buff;
				search_book += ".txt";
				new_list.open("newlist.txt");
				for (;;) {
					getline(list, book_buffer);
					if (book_buffer == search_book) {
						cout << "Введите новое название книги: ";
						getline(cin, new_name_book);
						OemToCharA(new_name_book.c_str(), ch_buff);
						new_name_book = ch_buff;
						new_name_book += ".txt";
						rename(search_book.c_str(), new_name_book.c_str());
						list.close();
						list.open("library.txt");
						while (!list.eof()) {
							getline(list, second_buffer);
							if (second_buffer == search_book) {

							}
							else if(!second_buffer.empty()){ 
								new_list << second_buffer << endl; 
							}
						}
						new_list << new_name_book << endl;
						new_list.close();
						list.close();
						r_list.close();
						remove("library.txt");
						rename("newlist.txt", "library.txt");
						system("cls");
						break;
					}
					else if(list.eof()){
						system("cls");
						cout << "Такой книги нет в библиотеке!\n";
						cin.get();
						system("cls");
						break;
					}
				}
				break;
			case 4:
				system("cls");
				new_list.open("newlist.txt");
				while (!list.eof()) {
					getline(list, book_buffer);
					books.push_back(book_buffer);
				}
				books.sort();
				copy(books.begin(), books.end(), ostream_iterator<string>(new_list, "\n"));
				while (!books.empty()) {
					books.pop_back();
				}
				list.close();
				r_list.close();
				new_list.close();
				remove("library.txt");
				rename("newlist.txt", "library.txt");
				system("cls");
				cout << "Успешно сортировано!\n";
				cin.get();
				system("cls");
				break;
			case 5:
				system("cls");
				cout << "Введите название книги, которую вы хотите удалить: ";
				getline(cin, delete_book);
				OemToCharA(delete_book.c_str(), ch_buff);
				delete_book = ch_buff;
				delete_book += ".txt";
				new_list.open("newlist.txt");
				for (;;) {
					getline(list, book_buffer);
					if (book_buffer == delete_book) {
						remove(delete_book.c_str());
						list.close();
						list.open("library.txt");
						while (!list.eof()) {
							getline(list, second_buffer);
							if (second_buffer == delete_book) {

							}
							else if (!second_buffer.empty()) {
								new_list << second_buffer << endl;
							}
							if (list.eof()) {
								break;
							}
						}
						new_list.close();
						list.close();
						r_list.close();
						remove("library.txt");
						rename("newlist.txt", "library.txt");
						system("cls");
						break;
					}
					else if(list.eof()){
						cout << "Такой книги нет в библиотеке!\n";
						break;
					}
				}
				break;
			case 6:
				system("cls");
				return;
				break;
			default:
				system("cls");
				cout << "Ошибка! Такой опции не существует.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin.get();
				system("cls");
				break;
			}
		}
		else {
			cout << "Ваша библиотека пуста!\n"; 
            break;
		}
	}
	
}
void OpenFileMenu() {
	system("cls");
	ifstream lstbk;
	char first_buffer[MAX_PATH];
	string lsbk_dir = login + "lstbk.txt";
	string chosed_book, sec_buffer, third_buffer;
	while (true) {
	int option_submenu = 0;
	cout << "Подменю библиотеки.\nПожалуйста, я уже третью ночь не сплю...\n";
	cout << "1)Открыть книгу. 2)Найти книги и редактировать. 3)Последние открытые книги. 4)Вернуться в главное меню.\n";
	cin >> option_submenu;
	cin.get();
	switch (option_submenu) {
	case 1:
		system("cls");
		cout << "Введите название книги:";
		getline(cin, chosed_book);
		OemToCharA(chosed_book.c_str(), first_buffer);
		chosed_book = first_buffer;
		TextViewer(chosed_book);
		break;
	case 2:
		SearchAndRedact();
		break;
	case 3:
		lstbk.open(lsbk_dir.c_str());
		cout << "Ваши последние книги:\n";
		if (lstbk.is_open()) {
			while (!lstbk.eof()) {
				getline(lstbk, sec_buffer);
				if(!sec_buffer.empty()) {
					third_buffer = open_file_decode(password, sec_buffer);
					cout << third_buffer << endl;
				}

			}
			lstbk.close();
			cin.get();
		}
		else cout << "Вы не открывали в последнее время книг.\n"; 
			break;
		case 4:
			system("cls");
			return;
			break;
		default:
			system("cls");
			cout << "Ошибка! Такой опции не существует.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get();
			system("cls");
			break;
		}
	}
	system("cls");
}

bool authorization() {
	int autho_option;
	string buffer, logon;
	while (true) {
		ifstream login_file;
		ofstream reg_file;
		cout << "Добро пожаловать в Консольную Библиотеку Текстовых Документов!\nПожалуйста, 1)Авторизуйтесь или 2)Зарегестрируйтесь:\n";
		cin >> autho_option;
		cin.get();
		switch (autho_option) {
		case 1:
			login_file.open("log.txt");
			cout << "Введите логин: ";
			getline(cin, login);
			cout << "Введите пароль: ";
			getline(cin, password);
			logon = new_file_coding(password, login);
			if(!logon.empty()){
				while (!login_file.eof()) {
					getline(login_file, buffer);
					if (buffer == logon) {
						system("cls");
						return true;
					}
				}
			}
			return false;
			break;
		case 2:
			reg_file.open("log.txt", ios_base::app);
			cout << "Придумайте логин: ";
			getline(cin, login);
			cout << "Придумайте пароль: ";
			getline(cin, password);
			logon = new_file_coding(password, login);
			if (!logon.empty()) {
				reg_file << new_file_coding(password, login) << endl;
				reg_file.close();
			}
			else return false;
			break;
		default:
			system("cls");
			cout << "Ошибка! Такой опции не существует.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get();
			system("cls");
			break;
		}
	}
}

string open_file_decode(string decode_key, string decode_text) {
	for (int i = 0; decode_key[i] != '\0' || decode_key.empty(); i++) { //проверка на тип данных в буфере
		if (!isdigit(decode_key[i])) {
			cout << "Error!\nTry again!\n";
			return "";
		}
	}
	for (int i = decode_key.size(), a = 0; i < decode_text.size(); i++, a++) {
		decode_key += decode_key[a];
	}
	for (int i = 0; i < decode_text.size(); i++) {
		decode_text[i] -= (decode_key[i] - '0');
	}
	return decode_text;
}

string new_file_coding(string user_key, string user_text) {
	for (int i = 0; i < user_key.size() || user_key.empty(); i++) { //проверка на тип данных в буфере
		if (!isdigit(user_key[i])) {
			i = -1;
			cout << "Error!\nTry again!\n";
			return "";
		}
	}
	while (user_text.empty()) {
		cout << "Empty input!\nTry again!\n";
		return "";
	}
	for (int i = user_key.size(), a = 0; i < user_text.size(); i++, a++) {
		user_key += user_key[a];
	}
	for (int i = 0; i < user_text.size(); i++) {
		user_text[i] += (user_key[i] - '0');
	}
	return user_text;
}

int main()
{
	setlocale(0, "");
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	bool auth;
	string time_buff, tf_adress;
	ofstream time_out;
	ifstream time_in;
	while (true) {
		system("cls");
		auth = authorization();
		if (auth == true) {
			int option = 0;
			tf_adress = login + "time.txt";
			while (true) {
				time_in.open(tf_adress.c_str());
				cout << "Консольная Библиотека Текстовых Документов (с)\n";
				cout << "Добро пожаловать, " << login << "! ";
				if (time_in.is_open()) {
					getline(time_in, time_buff);
					cout << "В последний раз вы заходили: " << time_buff << endl;
				}
				else cout << endl;
				cout << "Главное Меню:\n1)Открыть подменю библиотеки.\n2)Добавить новый текстовый документ в библиотеку.\n3)Выйти.\n";
				cin >> option;
				cin.get();
				switch (option)
				{
				case 1:
					OpenFileMenu();
					break;
				case 2:
					AddNewFile();
					break;
				case 3:
					time_out.open(tf_adress.c_str());
					time_out << asctime(timeinfo);
					time_out.close();
					return 0;
					break;
				default:
					system("cls");
					cout << "Ошибка! Такой опции не существует.\n";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					system("cls");
					break;
				}
			}
		}
	}
}




