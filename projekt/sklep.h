#pragma once
#include <fstream> //otwieranie, zamykanie pliku
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class sklep
{
	int x;
	int y;
	int rodzaj;
	int zasieg;
	double dochod;
	static int licznik;  //licznik sklepow
public:
	int & wspx()
	{
		return x;
	}
	int & wspy()
	{
		return y;
	}
	int & zas()
	{
		return zasieg;
	}
	int& rodz()
	{
		return rodzaj;
	}
	double& zysk()
	{
		return dochod;
	}
	void zmien_rodzaj(int nowy);
	sklep(const sklep &s);
	sklep(int xx,int yy,int r=10,int z=3,double d=0);
	~sklep();
	friend ostream & operator<< (ostream & ekran, const sklep & m);
};


