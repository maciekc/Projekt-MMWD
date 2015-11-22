// mmwd.cpp : Defines the entry point for the console application.
//
using namespace std;


#include <iostream>
#include "Header.h"


int main(int argc, char* argv[])
{
	srand(time(NULL));
	ifstream plik;
	
	int r_x = 10;//rozmiar wczytywanych z plików macierzy
	int r_y = 10;
	int zasieg_sklepu = 1;//zasieg dzialania sklepu +- od lokalizacji sklepu
	int budzet = 7000;
	macierz<int>konkurencja_macierz(r_x, r_y);
	macierz<int> macierz_tabo(r_x, r_y);
	vector<sklep *> konkurencja_wektor;
	vector<sklep *> nasze_sklepy;
	list<ele_tabo *>lista_tabo;



	wczytaj_skl_konkurencj(konkurencja_wektor);//wczytanie sklepow konkurencji do wektora
	k_macierz(konkurencja_wektor, konkurencja_macierz, r_x, r_y);//naniesienie sklepow konkurencji na mape
	cout << konkurencja_macierz;
	macierz<int>czynsz(r_x, r_y);
	macierz<int>zaludnienie(r_x, r_y);
	otworz_plik("cz.txt", plik);
	wczytaj_macierz<int>(czynsz, r_x, r_y, plik);
	otworz_plik("z.txt", plik);
	wczytaj_macierz<int>(zaludnienie, r_x, r_y, plik);
	cout << "czynsz\n" << czynsz << "\n";
	cout << "zaludnienie\n" << zaludnienie << "\n";
	//wywolywanie sasiedztwa1 tak dlugo az trzy razy pod rzad ono nie zadziala
	int sas1 = 3;
	while (sas1)
	{
		bool wynik = sasiedztwo1(3, r_x, r_y, zasieg_sklepu, &budzet, czynsz, konkurencja_macierz, zaludnienie, konkurencja_wektor, nasze_sklepy,lista_tabo,macierz_tabo);
		if (wynik == true)
			sas1 = 3;
		else sas1--;
	}
	/*for (int i = 0; i < 5; i++)
	{
		sasiedztwo1(3, 10, 10, 1, &budzet, czynsz, konkurencja_macierz, zaludnienie, konkurencja_wektor, nasze_sklepy);
	}*/

	cout << "wektor naszych sklepow po 1 sasiedztwie\n";
	for (vector<sklep*>::const_iterator i = nasze_sklepy.begin(); i != nasze_sklepy.end(); ++i)
		cout << (**i) << "\n";
	cout << "macierz wszystkich sklepow\n";
	cout << konkurencja_macierz;
	cout << "dzialanie sasiedztwa2\n";
	int sas2 = 3;
	while (sas2--)
		sasiedztwo2(r_x, r_y, zasieg_sklepu, &budzet, czynsz, konkurencja_macierz, zaludnienie, konkurencja_wektor, nasze_sklepy,lista_tabo,macierz_tabo);
	cout << "macierz wszystkich sklepow po drugim sasiedztwie\n";
	cout << konkurencja_macierz;
	cout << "wektor naszych sklepow \n";
	for (vector<sklep*>::const_iterator i = nasze_sklepy.begin(); i != nasze_sklepy.end(); ++i)
		cout << (**i) << "\n";
	system("PAUSE");
	return 0;
}

