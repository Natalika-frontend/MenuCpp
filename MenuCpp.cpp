// MenuCpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

//#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdarg.h"
#include <iostream>
#include "string"
#include "conio.h"
#include "ctime"
#include <fstream>

using namespace std;

class Student {
private:
	// размещаем данные для защиты
	string nzk;
	string fio;
	string pol;
	int kurs;
	string spec;

public:
	// конструктор по умолчанию
	Student() { nzk = ""; fio = ""; pol = ""; kurs = 0; spec = ""; }

	// конструктор c параметрами
	Student(bool fl) {
		cout << "Nzk=  "; cin >> nzk;
		cout << "FIO=  "; cin >> fio;
		cout << "Pol=  "; cin >> pol;
		do {
			cout << "Kurs=  "; cin >> kurs;
		} while (kurs < 0 || kurs>7);
		cout << "Spec=  "; cin >> spec;
	}

	// конструктор c параметрами
	Student(string p1, string p2, string p3, int p4, string p5) {
		nzk = p1; fio = p2; pol = p3;
		if (p4 > 0 && p4 <= 6) kurs = p4; else kurs = 0;
		spec = p5;
	}

	// конструктор копирования
	Student(Student& o) { nzk = o.nzk; fio = o.fio; pol = o.pol; kurs = o.kurs; spec = o.spec; }

	// деструктор
	~Student() {}


	void setNzk(string p) { nzk = p; }
	void setFio(string p) { fio = p; }
	void setPol(string p) { pol = p; }
	void setKurs(int p) { if (p > 0 && p <= 6) kurs = p; else kurs = 0; }
	void setSpec(string p) { spec = p; }

	string getNzk() { return nzk; }
	string getFio() { return fio; }
	string getPol() { return pol; }
	int getKurs() { return kurs; }
	string getSpec() { return spec; }

	void print() {
		cout << nzk << "  " << fio << "  " << pol << "  " << kurs << "  " << spec << endl;
	}
};

class Room
{
private:
	int nomer;
	int nMax;
	int t; //текущее количество проживающих
	Student** MS; // массив проживающих в комнате студентов

public:
	// конструктор по умолчанию
	Room() { nomer = 0; nMax = 4; t = 0; MS = new Student * [nMax]; }

	// конструктор c параметром
	Room(int nom, int Max) {
		nomer = nom;
		if (Max > 0 && Max < 101) nMax = Max; else nMax = 4;
		t = 0; MS = new Student * [nMax];
		for (int i = 0; i < nMax; ++i) {
			MS[i] = NULL; // Инициализируем элементы массива
		}
	}

	// конструктор копии
	Room(const Room& o) {
		nomer = o.nomer;
		nMax = o.nMax;
		t = o.t;
		MS = new Student * [nMax];
		for (int i = 0; i < nMax; ++i) {
			MS[i] = NULL; // Инициализируем элементы массива
		}
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				MS[i] = new Student(o.MS[i]);
			}
		}
	}

	// деструктор
	~Room() {
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				delete MS[i];
			}
		}

		delete[]MS;
	}

	void print() {
		cout << nomer << " (" << t << " / " << nMax << " )" << endl;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				cout << i + 1 << ". ";
				MS[i]->print();
			}
		}
	}

	void print_small() {
		cout << nomer << " (" << t << " / " << nMax << " )" << endl;
	}

	bool add(Student& o) {
		bool vz = false;
		if (o.getPol() == "m" || o.getPol() == "w") {
			if (t < nMax) {
				if (t == 0) {
					MS[t] = new Student(o);
					t++;
					vz = true;
				}
				else {
					if (o.getPol() == MS[0]->getPol()) {
						MS[t] = new Student(o);
						t++;
						vz = true;
					}
				}
			}
		}
		return vz;
	}

	bool del(string p_nzk) {
		bool vz = false;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				if (p_nzk == MS[i]->getNzk()) {
					delete MS[i];
					if (i < t - 1) {
						for (int j = i; j < t; j++) {
							MS[i] = MS[i + 1]; // тут посмотреть правильность индексов
						}
					}
					MS[t - 1] = NULL;
					t--;
					vz = true;
				}
			}
		}
		return vz;
	}

	int getT() { return t; }
	int getNMax() { return nMax; }
	int getNom() { return nomer; }
};


class Obshaga
{
private:
	string name;
	string adres;
	int nMax;
	int t;
	Room** MR;

public:
	Obshaga() { name = ""; adres = ""; nMax = 100; t = 0; MR = new Room * [nMax]; }

	Obshaga(string n_name, string n_adr, int Max) {
		name = n_name; adres = n_adr;
		if (Max > 0 && Max < 1000) nMax = Max; else nMax = 100;
		t = 0;
		MR = new Room * [nMax];
	}

	Obshaga(string p_file) :name(""), adres(""), nMax(100), t(0), MR(nullptr) {
		FILE* fp; // создали переменную типа файл
		if ((fp = fopen(p_file.c_str(), "r")) != NULL) {
			t = 0;
			int fl = 0, k = 0;
			char p_buf[255];
			string p_str;
			
			fgets(p_buf, 255, fp); // Считываем первую строчку в буферную переменную
			p_str = p_buf;
			name = p_str.substr(0, 20);
			adres = p_str.substr(20, 30);
			p_str = p_str.substr(51);
			k = atoi(p_str.c_str()); // преобразуем значение полученное выше к числу
			if (k > 10 && k < 1000) nMax = k;
			MR = new Room * [nMax];
			
			while (!feof(fp)) { // преобразование каждой строки характеристики комнаты
				fgets(p_buf, 255, fp);
				p_str = p_buf;

				int n_room, kol_pr, kurs;
				string p_nzk, fio, spec, pol;

				n_room = atoi(p_str.substr(0, 3).c_str());
				p_str = p_str.substr(4);

				kol_pr = atoi(p_str.substr(0, 2).c_str());
				p_str = p_str.substr(3);

				p_nzk = p_str.substr(0, 7);
				p_str = p_str.substr(8);

				fio = p_str.substr(0, 30);
				p_str = p_str.substr(31);

				pol = p_str.substr(0, 1);
				p_str = p_str.substr(2);

				kurs = atoi(p_str.substr(0, 1).c_str());
				p_str = p_str.substr(2);

				spec = p_str.substr(0, p_str.length() - 1);

				if (info_room(n_room, false) == false) {
					Room tmp(n_room, kol_pr);
					add(tmp);
				}

				Student o(p_nzk, fio, pol, kurs, spec);
				add_stud_v_room(n_room, o);

			}

		}
	}

	Obshaga(Obshaga& o) {
		name = o.name;
		adres = o.adres;
		nMax = o.nMax;
		t = 0;
		MR = new Room * [nMax];
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				MR[i] = new Room(*o.MR[i]);
			}
		}
	}


	~Obshaga() {
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				delete MR[i];
			}
		}

		delete[]MR;
	}


	void setName(string p) { name = p; }
	void setAdres(string p) { adres = p; }

	string getName() { return name; }
	string getAdres() { return adres; }
	int getT() { return t; }
	int getMax() { return nMax; }

	void print() {
		cout << name << "  " << adres << " (" << t << " / " << nMax << " )" << endl;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				cout << i + 1 << ". ";
				MR[i]->print();
			}
		}
	}

	bool add(Room& o) {
		bool vz = false;

		if (t < nMax) {
			MR[t] = new Room(o);
			t++;
			vz = true;
		}

		return vz;
	}

	bool dell(int p_nom) {
		bool vz = false;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				if (p_nom == MR[i]->getNom() && MR[i]->getT() == 0) {
					delete MR[i];
					if (i < t - 1) {
						for (int j = i; j < t; j++) {
							MR[i] = MR[i + 1]; // тут посмотреть правильность индексов
						}
					}
					MR[t - 1] = nullptr;
					t--;
					vz = true;
					break;
				}
			}
		}
		return vz;
	}


	int find_sv_mest() {
		int vz = 0;
		if (t > 0) {
			int k = 0;
			for (int i = 0; i < t; i++) {
				if (MR[i]->getT() < MR[i]->getNMax()) {
					cout << i + 1 << ". ";
					MR[i]->print();
					k++;
				}

			}
			vz = k;
		}
		return vz;
	}

	bool add_stud_v_room(int p_nomer, Student& o) {
		bool vz = false;
		int ind_kom = -1;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				if (MR[i]->getNom() == p_nomer) {
					vz = MR[i]->add(o);
				}
			}
		}
		return vz;
	}

	void spisok_room_all() {
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				MR[i]->print_small();
			}
		}
		else {
			cout << "No room!\n";
		}
	}

	bool info_room(int p_nom, bool fl_pr) {
		bool vz = false;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				if (MR[i]->getNom() == p_nom) {
					if (fl_pr) {
						MR[i]->print();
					}

					vz = true;
					break;
				}
			}
			if (vz == false) {
				if (fl_pr) cout << "Room not found!\n";
			}
		}
		else {
			if (fl_pr) cout << "No room!\n";
		}
		return vz;
	}

	bool del_stud_iz_room(int p_nk, string p_nzk) {
		bool vz = false;
		if (t > 0) {
			for (int i = 0; i < t; i++) {
				if (MR[i]->getNom() == p_nk) {
					vz = MR[i]->del(p_nzk);
					break;
				}
			}
		}
		return vz;
	}
};




int main()
{
	//Obshaga A("Dom studenta", "g.Donetsk, ul.Artema, 153a", 150);
	Obshaga A("F:\\DonNTY\\semestr2\\OOP\\obshaga.txt");

	int pm = 0;
	do {
		cout << "1. Add room\n";
		cout << "2. View room\n";
		cout << "3. Del room\n";
		cout << "4. Add stud v room\n";
		cout << "5. Dell stud is room\n";
		cout << "6. Pereselit stud\n";
		cout << "7. View room\n";
		cout << "8. Exit\n";
		cout << ">>"; cin >> pm;

		switch (pm) {
		case 1: {
			int p_nomer;
			int kol_s;
			cout << "Nomer room= "; cin >> p_nomer;
			cout << "Kolvo stud= "; cin >> kol_s;
			Room tmp(p_nomer, kol_s);
			if (A.add(tmp)) {
				cout << "Room add successfully!\n";
			}
			else {
				cout << "Room not add!\n";
			}
			_getch();
			break;
		}
		case 2: {
			A.print();

			_getch();
			break;
		}
		case 3: {
			int p_nomer;
			cout << "Nomer room= "; cin >> p_nomer;
			if (A.dell(p_nomer)) {
				cout << "Room delete successfully!\n";
			}
			else {
				cout << "Room not delete!\n";
			}
			_getch();
			break;
		}
		case 4: {

			if (A.find_sv_mest() > 0) {
				_getch();
				int p_nomer;
				cout << "Nomer room= "; cin >> p_nomer;
				Student tmp(true);
				if (A.add_stud_v_room(p_nomer, tmp)) {
					cout << "Stud add v " << p_nomer << " room successfully!\n";
				}
				else {
					cout << "Stud not add!\n";
				}
			}
			else {
				cout << "All rooms full!\n";
			}

			_getch();
			break;
		}
		case 5: {
			A.spisok_room_all();
			_getch();
			int p_nomer;
			cout << "Nomer room= "; cin >> p_nomer;
			if (A.info_room(p_nomer, true)) {
				string p_nzk;
				cout << "NZK= "; cin >> p_nzk;
				if (A.del_stud_iz_room(p_nomer, p_nzk)) {
					cout << "Udalenie proizvedeno!\n"; // вылетело при удалении
				}
				else {
					cout << "Not ydalenie!\n";
				}
			}

			_getch();
			break;
		}
		case 6: {
			_getch();
			break;
		}

		case 7: {
			A.spisok_room_all();
			_getch();
			int p_nomer;
			cout << "Nomer room= "; cin >> p_nomer;
			A.info_room(p_nomer, true);
			_getch();
			break;
		}
		}
	} while (pm != 8);


	return 0;
}
