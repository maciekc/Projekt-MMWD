#ifndef HEADER
#define HEADER

#include <fstream> //otwieranie, zamykanie pliku
#include <iostream>
#include <cstdio>
#include <vector>
#include <list>
#include <math.h>
#include <ctime>
#include <cstdlib>

#include "sklep.h"
#include "macierz.h"
#include "ele_tabo.h"

#define max_roz_tabo 5

using namespace std;

int max(int a, int b);
void otworz_plik(string nazwa_pliku, ifstream &plik);
void przeskaluj_obszar(int wspx, int wspy, int zasieg, int rozx, int rozy, int *xmin, int *xmax, int*ymin, int *ymax);
void wczytaj_skl_konkurencj(vector<sklep *> &konkurencja);
void k_macierz(vector<sklep *> &konkurencja, macierz<int> &m, int rozx, int rozy);
void losuj(int *x, int *y, int rozx, int rozy);
double calkowity_zysk(vector<sklep *> &wns);
void aktualizuj_liste_tabo(int rodzaj_sas, int x, int y, int max_dl, list<ele_tabo*> &lista_tabo, macierz<int> &mac_tabo);
//void dobierz_sklep(int rozx, int rozy);
double dobierz_sklep(int x, int y, int kat, int zasieg, int rozx, int rozy, macierz<int>&k, macierz<int>&zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns);
void dodaj_sklep(int x, int y, int kat, int zasieg, double zysk, vector<sklep *> &wns);
void utworz_sklep(int x, int y, int kat, double zysk, int zasieg, int numer_sklepu, int rozx, int rozy, vector<sklep *> &wk, vector<sklep *> &wns, macierz<int> &k, macierz<int> &zaludnienie);
bool sasiedztwo1(int t, int rozx, int rozy, int zasieg, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns,list<ele_tabo*> &lista_tabo, macierz<int> &mac_tabo);
void znajdz_najg_sklep(vector<sklep *> &wns, int *x, int *y, int *nr);
void przydziel_nowe_sklepy(int x, int y, int nr, int zasieg, int rozx, int rozy, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns);
bool przesun_sklep(int xx, int yy, int nr, int zasieg, int rozx, int rozy, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns, macierz<int> &mac_tabo);

void sasiedztwo2(int rozx, int rozy, int zasieg, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns, list<ele_tabo*> &lista_tabo, macierz<int> &mac_tabo);




#endif