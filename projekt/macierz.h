#pragma once

#include <fstream>
#include <cctype>
#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
using namespace std;

template <class T>
class macierz
{
	T** mac;
	int rozx;
	int rozy;
	static int licznik;
public:
	macierz(int x=0,int y=0);
	~macierz();
	T& operator()(int x, int y);
	macierz(const macierz <T> &m); //konstruktor kopiujacy
	macierz<T> & operator=(const macierz<T> &m);
	friend ostream & operator<< <>(ostream & ekran, const macierz <T>& m);
};
template<class T>
int macierz<T>::licznik = 0;

template <class T>
void wczytaj_macierz(macierz<T>& mac, int x, int y, ifstream &plik)
{
	T dana;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			plik >> dana;
			(mac)(i, j) = dana;
		}
	}
	plik.close();
}


template <class T>
macierz<T>::macierz(int x, int y)
{
	rozx = x;
	rozy = y;
	mac = new T *[rozx];
	for (int i = 0; i < rozx; i++)
	{
		mac[i] = new T[rozy];
		for (int j = 0; j < rozy; j++)
			mac[i][j] = 0;
	}
	licznik++;
	cout << "tworzona nnowa macierz\n";
}
template <class T>
macierz<T>::macierz(const macierz <T> &m)
{
	rozx = m.rozx;
	rozy = m.rozy;
	mac = new T *[rozx];
	for (int i = 0; i < rozx; i++)
	{
		mac[i] = new T[rozy];
		for (int j = 0; j < rozy; j++)
			mac[i][j] = m.mac[i][j];
	}

}
template <class T>
macierz<T> & macierz<T>::operator=(const macierz<T> &m)
{
	if (&m == this) return *this;
	this->~macierz();
	rozx = m.rozx;
	rozy = m.rozy;
	mac = new T *[rozx];
	for (int i = 0; i < rozx; i++)
	{
		mac[i] = new T[rozy];
		for (int j = 0; j < rozy; j++)
			mac[i][j] = m.mac[i][j];
	}
	return *this;
}
template <class T>
macierz<T>::~macierz()
{
	for (int i = 0; i < rozx; i++)
		delete[] mac[i];
	delete[] mac;
	mac = NULL;
	licznik--;
	cout << "usunieta macierz\n"<<licznik;
}
template <class T>
T& macierz<T>::operator()(int x, int y)
{
	return (mac[x][y]);
}

template <class T>
ostream & operator<< (ostream &ekran, const macierz <T> & m)
{
	for (int i = 0; i < m.rozx; i++)
	{
		for (int j = 0; j < m.rozy; j++)
		{
			ekran << setw(2)<<m.mac[i][j] << " ";
		}
		ekran << "\n";
	}
	return ekran;
}