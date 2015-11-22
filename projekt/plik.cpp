#include "Header.h"
//#include "macierz.h"
//#include "sklep.h"

using namespace std;

double przelicznik[10] = { 0.91, 0.819, 0.728, 0.637, 0.546, 0.455, 0.364, 0.273, 0.182, 0.091 };


void otworz_plik(string nazwa_pliku, ifstream &plik)
{
	int t = 5;
	bool s = 0;
	while (!s &&t--)
	{
		plik.open(nazwa_pliku.c_str(), ios::in);
		if (!plik)
		{
			cout << "otwarcie pliku nie powiodlo sie\n";
			plik.clear(plik.rdstate() & ~ios::failbit); //skasoowanie flagi bledu
		}
		else
			s = 1;
	}

}

void przeskaluj_obszar(int wspx, int wspy,int zasieg,int rozx,int rozy, int *xmin, int *xmax, int*ymin, int *ymax)
{
	if (wspx < zasieg)
		*xmin = 0;
	else
		*xmin = wspx - zasieg;
	if (wspx + zasieg>=rozx)
		*xmax = rozx;
	else
		*xmax = wspx + zasieg+1;
	//to ssamo dla wsp y
	if (wspy < zasieg)
		*ymin = 0;
	else
		*ymin = wspy - zasieg;
	if (wspy + zasieg>=rozy)
		*ymax = rozy;
	else
		*ymax = wspy + zasieg+1;

}
//wczytanie do wektora sklep�w konkurencji
void wczytaj_skl_konkurencj(vector<sklep *> &konkurencja)
{
	string nazwa_pliku;
	cout << "podaj nazwe pliku ze sklepami konkurencji\n";
	cin >> nazwa_pliku;
	ifstream plik;
	otworz_plik(nazwa_pliku, plik);
	int ile; //liczba sklepow konkurencji
	int x, y; //wspolrzedne sklepu
	int rodzaj, zasieg;
	plik >> ile;
	for (int i = 0; i < ile; i++)
	{
		plik >> x >> y >> rodzaj >> zasieg;
		sklep *k = new sklep(x, y, rodzaj, zasieg);
		konkurencja.push_back(k);
	}
	plik.close();
	/*cout << "wypisanie rodzai sklepow konkurencji\n";
	for (int i = 0; i < ile; i++)
	{
		cout << (*(konkurencja[i])).rodz()<<"\n";
	}*/
	
	
}
//naniesienie sklepow konkurencji na mape
void k_macierz(vector<sklep *> &konkurencja, macierz<int> &m,int rozx,int rozy)
{
	int wspx, wspy; //wspolrzedne danego sklepu
	int rodzaj, zasieg;
	int ile = konkurencja.size();
	int xmin, xmax, ymin, ymax;
	for (int i = 0; i < ile; i++)  //i jest identyfikatorem sklepu
	{
		wspx = (*(konkurencja[i])).wspx();
		wspy = (*(konkurencja[i])).wspy();
		zasieg = (*(konkurencja[i])).zas();
		rodzaj = (*(konkurencja[i])).rodz();
		przeskaluj_obszar(wspx, wspy, zasieg, rozx, rozy, &xmin, &xmax, &ymin, &ymax);
		for (int x = xmin; x < xmax;x++)
		for (int y = ymin; y < ymax; y++)
		{
			if (m(x,y)==0)
				m(x, y) = i+1;
			else
			{
				int iden = m(x, y)-1;
				double a = (x - (*(konkurencja[iden])).wspx())*(x - (*(konkurencja[iden])).wspx()) + (y - (*(konkurencja[iden])).wspy())*(y - (*(konkurencja[iden])).wspy());
				double odl_iden = sqrt(a)+1;
				double b = (x - wspx)*(x - wspx) + (y - wspy)*(y - wspy);
				double odl_i = sqrt(b)+1;
				double wyniden = ((double)(*(konkurencja[iden])).rodz() / odl_iden);
				double wyniki = ((double)rodzaj / odl_i);
				if (wyniden < wyniki)
					m(x, y) = i+1;
				
			}
		}

	}
}
void losuj(int *x, int *y, int rozx,int rozy)
{
	//srand((unsigned int)time(NULL));
	//srand(time(NULL));
	*x = (rand()) % rozx;
	*y = (rand()) % rozy;
}

double calkowity_zysk(vector<sklep *> &wns)
{
	double c_zysk = 0;
	for (int i = 0; i < wns.size(); i++)
	{
		c_zysk += (*(wns[i])).zysk();
	}
	return c_zysk;
}

//aktualizacja listy tabo
void aktualizuj_liste_tabo(int rodzaj_sas,int x,int y,int max_dl,list<ele_tabo*> &lista_tabo,macierz<int> &mac_tabo)
{
	if (lista_tabo.size() < max_dl)
	{
		cout << "dodanie do listy tabo " << x << " " << y << "\n";
		ele_tabo* nowy = new ele_tabo(x, y, rodzaj_sas);
		lista_tabo.push_front(nowy);
		mac_tabo(x, y) = rodzaj_sas;
	}
	else
	{
		int xx = (lista_tabo.back())->wspx();
		int yy = (lista_tabo.back())->wspy();
		cout << "usuniecie z listy tabo "<<xx << " " << yy << "\n";
		mac_tabo(xx, yy) = 0;
		lista_tabo.pop_back();
		ele_tabo* nowy = new ele_tabo(x, y, rodzaj_sas);
		lista_tabo.push_front(nowy);
	}
}

//funkcja zwraca wertosc funkcji celu dla sklepu o kategori kat w lokalizacji x, y
double dobierz_sklep(int x, int y, int kat, int zasieg, int rozx, int rozy, macierz<int>&k, macierz<int>&zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns)
{
	double zysk = 0;
	int xmin, xmax, ymin, ymax;
	przeskaluj_obszar(x, y, zasieg, rozx, rozy, &xmin, &xmax, &ymin, &ymax);
	for (int i = xmin; i < xmax;i++)
	for (int j = ymin; j < ymax; j++)
	{
		if (k(i, j) == 0)
		{
			zysk += zaludnienie(i, j) * 400*przelicznik[kat-1];
		}
		else if (k(i,j)>0)//gdy lokalizacja i,j jest pod dzialanie sklepu konkurencji
		{
			int iden = k(i, j) - 1;
			double a = (i - (*(wk[iden])).wspx())*(i - (*(wk[iden])).wspx()) + (j - (*(wk[iden])).wspy())*(j - (*(wk[iden])).wspy());
			double odl_kon = sqrt(a) + 1;//odleglosc od punktu i,j do sklepu konkurencji
			double b = (i - x)*(i - x) + (j -y)*(j - y);
			double odl_ns = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
			double wyn_kon = ((double)(*(wk[iden])).rodz() / odl_kon);//wskaznik dla konkurencji
			double wyn_ns = ((double)kat / odl_ns);//wskaznik dla naszego sklepu
			if (wyn_kon < wyn_ns)
				zysk += zaludnienie(i, j) * 400 * przelicznik[kat - 1];
		}
		else//gdy lokalizacja i,j jest pod dzialanie innego naszego sklepu
		{
			int iden = -(k(i, j) + 1);
			double a = (i - (*(wns[iden])).wspx())*(i - (*(wns[iden])).wspx()) + (j - (*(wns[iden])).wspy())*(j - (*(wns[iden])).wspy());
			double odl_kon = sqrt(a) + 1;//odleglosc od punktu i,j do sklepu konkurencji
			double b = (i - x)*(i - x) + (j - y)*(j - y);
			double odl_ns = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
			double wyn_kon = ((double)(*(wns[iden])).rodz() / odl_kon);//wskaznik dla konkurencji
			double wyn_ns = ((double)kat / odl_ns);//wskaznik dla naszego sklepu
			if (wyn_kon < wyn_ns)
				zysk += zaludnienie(i, j) * 400 * przelicznik[kat - 1];
		}
	}
	return zysk;
}
//dodaje nowy sklep do wektora sklepow
void dodaj_sklep(int x, int y, int kat, int zasieg, double zysk, vector<sklep *> &wns)
{
	//utworzenie nowego sklepu i dodanie go do tablicy naszych sklepow
	sklep* nowy = new sklep(x, y, kat, zasieg, zysk);

	wns.push_back(nowy);
}

//nanosi na mape sklepow nasz nowy sklep
void utworz_sklep(int x, int y, int kat, double zysk, int zasieg, int numer_sklepu,int rozx, int rozy, vector<sklep *> &wk, vector<sklep *> &wns, macierz<int> &k, macierz<int> &zaludnienie)
{
	
	int xmin, xmax, ymin, ymax;
	przeskaluj_obszar(x, y, zasieg, rozx, rozy, &xmin, &xmax, &ymin, &ymax);
	for (int i = xmin; i < xmax; i++)
	for (int j = ymin; j < ymax; j++)
	{
		if (k(i, j) == 0)
		{
			k(i, j) = -(numer_sklepu + 1);
		}
		else if (k(i, j)>0)//gdy lokalizacja i,j jest pod dzialanie sklepu konkurencji
		{
			int iden = k(i, j) - 1;
			double a = (i - (*(wk[iden])).wspx())*(i - (*(wk[iden])).wspx()) + (j - (*(wk[iden])).wspy())*(j - (*(wk[iden])).wspy());
			double odl_kon = sqrt(a) + 1;//odleglosc od punktu i,j do sklepu konkurencji
			double b = (i - x)*(i - x) + (j - y)*(j - y);
			double odl_ns = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
			double wyn_kon = ((double)(*(wk[iden])).rodz() / odl_kon);//wskaznik dla konkurencji
			double wyn_ns = ((double)kat / odl_ns);//wskaznik dla naszego sklepu
			if (wyn_kon < wyn_ns)
				k(i, j) = -(numer_sklepu + 1);
		}
		else//gdy lokalizacja i,j jest pod dzialanie innego naszego sklepu
		{
			int iden = -(k(i, j) + 1);
			double a = (i - (*(wns[iden])).wspx())*(i - (*(wns[iden])).wspx()) + (j - (*(wns[iden])).wspy())*(j - (*(wns[iden])).wspy());
			double odl_kon = sqrt(a) + 1;//odleglosc od punktu i,j do sklepu konkurencji
			double b = (i - x)*(i - x) + (j - y)*(j - y);
			double odl_ns = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
			double wyn_kon = ((double)(*(wns[iden])).rodz() / odl_kon);//wskaznik dla naszego starego sklepu
			double wyn_ns = ((double)kat / odl_ns);//wskaznik dla naszego nowego sklepu
			if (wyn_kon < wyn_ns)
			{
				k(i, j) = -(numer_sklepu + 1);
				(*(wns[iden])).zysk() -= 400 * zaludnienie(i, j)*przelicznik[(*(wns[iden])).rodz() - 1];
			}
				
		}
	}	
}

bool sasiedztwo1(int ttt, int rozx, int rozy, int zasieg, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns,list<ele_tabo*> &lista_tabo,macierz<int> &mac_tabo)
{
	double S_war_funkcji = 0;
	int S_naj_kat=0;
	int x_s=-1;
	int y_s=-1;//wspolrzedne najlepszego sklepu
	//int naj_war_funkcji;//najlepsza wartosc funkcji dla lokaliacji x,y
	int budzet1 = *budzet;
	while (ttt--)
	{
		int budzet0 = *budzet;
		int x=0, y=0; //wspolrzedne danego sklepu
		int naj_kat=0;//najlepsza kategoria dla lokalizacji x,y
		double war_funkcji = 0;
		double pomoc = 0;
		losuj(&x, &y, rozx, rozy);
		//cout << "wylosowane x,y" << x << y << "\n";
		if ((czynsz(x, y) < budzet0) && (mac_tabo(x,y)==0))
		{
			
			//wybranie najlepszej kategori sklepu dla lokalizacji x,y
			for (int kat = 1; kat <= 10; kat++)
			{
				pomoc =dobierz_sklep(x, y, kat, zasieg, rozx, rozy, k, zaludnienie, wk, wns);
				if (pomoc>war_funkcji)
				{
					war_funkcji = pomoc;
					naj_kat = kat;
				}
			}
		
		}
		int cz = czynsz(x, y);
		if ((war_funkcji > S_war_funkcji) && (war_funkcji>cz))
		{
			budzet1 =budzet0-cz;
			S_war_funkcji = war_funkcji;
			S_naj_kat = naj_kat;
			x_s = x;
			y_s = y;
		//	cout << "tutaj " << x_s <<"  "<<y_s<<" "<<S_war_funkcji<<"\n";
		}
	}
	if ((x_s != (-1))&&(y_s!= (-1)))
	{
		//cout << "przy tworzeniu nowego sklepu \n" << S_war_funkcji << " " << x_s << " " << y_s << "\n";
		utworz_sklep(x_s, y_s, S_naj_kat, S_war_funkcji, zasieg,wns.size(), rozx, rozy, wk, wns, k, zaludnienie);
		dodaj_sklep(x_s, y_s, S_naj_kat, zasieg, S_war_funkcji, wns);
		*budzet = budzet1;
		//aktualizacja listy tabo
		aktualizuj_liste_tabo(1,x_s,y_s,max_roz_tabo,lista_tabo, mac_tabo);


		return true;
	}
	else return false;
}