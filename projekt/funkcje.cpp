#include "Header.h"

using namespace std;

double przelicznik2[10] = { 0.91, 0.819, 0.728, 0.637, 0.546, 0.455, 0.364, 0.273, 0.182, 0.091 };

void znajdz_najg_sklep(vector<sklep *> &wns, int *x, int *y,int *nr)
{
	double najmn_zysk = (*(wns[0])).zysk();
	*x = (*(wns[0])).wspx();
	*y= (*(wns[0])).wspy();
	*nr = 0;
	double zysk;
	for (int i = 1; i < wns.size(); i++)
	{
		zysk = (*(wns[i])).zysk();
		//cout << "zysk w sas2 "<<zysk << "\n";
		if (zysk < najmn_zysk)
		{
			najmn_zysk = zysk;
			*x = (*(wns[i])).wspx();
			*y = (*(wns[i])).wspy();
			*nr = i;
		}
	}

}
int max(int a, int b)
{
	if (a > b)return a;
	else return b;
}
void przydziel_nowe_sklepy(int xx, int yy, int nr, int zasieg, int rozx, int rozy, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns)
{

	int xmin, xmax, ymin, ymax;
	przeskaluj_obszar(xx, yy, zasieg, rozx, rozy, &xmin, &xmax, &ymin, &ymax);
	for (int x = xmin; x < xmax; x++)
	{
		for (int y = ymin; y < ymax; y++)
		{
			if (k(x, y) == -(nr + 1))
			{
				double ws, ws_naj = 0;
				int x_s = -1, y_s = -1, nr_s;
				int xn, yn;//wsp innego naszego sklepu
				int kat;//kategoria innego naszego sklepu
				for (int i = 0; i < wns.size(); i++)
				{
					if (i != nr)
					{
						xn = (*(wns[i])).wspx();
						yn = (*(wns[i])).wspy();//wspolrzedne sklepu nr i
						kat = (*(wns[i])).rodz();
						if (max(abs(x - xn), abs(y - yn)) <= zasieg)
						{
							double b = (xn - x)*(xn - x) + (yn - y)*(yn - y);
							double odl = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
							ws = (double)kat / odl;
							if (ws > ws_naj)
							{
								ws_naj = ws;
								x_s = xn;
								y_s = yn;
								nr_s = i;
							}
						}

					}
				}
				//pzeszukanie sklepow konkurencji
				double wsk, wsk_naj = 0;
				int xk_s = -1, yk_s = -1, nrk_s;
				int xkn, ykn;//wsp innego naszego sklepu
				int katk;//kategoria innego naszego sklepu
				for (int i = 0; i < wk.size(); i++)
				{


					xkn = (*(wk[i])).wspx();
					ykn = (*(wk[i])).wspy();
					katk = (*(wk[i])).rodz();
					if (max(abs(x - xkn), abs(y - ykn)) <= (*(wk[i])).zas())
					{
						double b = (xkn - x)*(xkn - x) + (ykn - y)*(ykn - y);
						double odl = sqrt(b) + 1;//odleglosc od punktu i,j do naszej lokalizacji
						wsk = (double)katk / odl;
						if (wsk > wsk_naj)
						{
							wsk_naj = wsk;
							xk_s = xkn;
							yk_s = ykn;
							nrk_s = i;
						}
					}


				}

				if ((ws_naj >= wsk_naj) && x_s != -1)
				{
					k(x, y) = -(nr_s + 1);
					(*(wns[nr_s])).zysk() += 400 * zaludnienie(x, y)*przelicznik2[(*(wns[nr_s])).rodz() - 1];

				}
				else if ((ws_naj < wsk_naj) && xk_s != -1)
				{
					k(x, y) = nrk_s + 1;
				}
				else
					k(x, y) = 0;
			
			}
		}
	}


}


bool przesun_sklep(int xx, int yy, int nr, int zasieg, int rozx, int rozy, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns, macierz<int> &mac_tabo)
{
	double S_war_funkcji = 0;
	int S_naj_kat = 0;
	int x_s = -1;
	int y_s = -1;//wspolrzedne najlepszego sklepu
	int xmin, xmax, ymin, ymax;
	int budzet0 = *budzet;
	przeskaluj_obszar(xx, yy, zasieg, rozx, rozy, &xmin, &xmax, &ymin, &ymax);
	for (int i = xmin; i < xmax; i++)
	{
		for (int j = ymin; j < ymax; j++)
		{
			int naj_kat = 0;//najlepsza kategoria dla lokalizacji x,y
			double war_funkcji = 0;
			double pomoc = 0;
			if ((*budzet >= czynsz(i, j)) && (mac_tabo(i,j)==0))
			{
				//int x = 0, y = 0; //wspolrzedne danego sklepu


				//	//wybranie najlepszej kategori sklepu dla lokalizacji x,y
				for (int kat = 1; kat <= 10; kat++)
				{
					pomoc = dobierz_sklep(i, j, kat, zasieg, rozx, rozy, k, zaludnienie, wk, wns);
					if (pomoc > war_funkcji)
					{
						war_funkcji = pomoc;
						naj_kat = kat;
					}
				}


				int cz = czynsz(i, j);
				if ((war_funkcji > S_war_funkcji) && (war_funkcji > cz))
				{
					budzet0 =*budzet- cz;
					S_war_funkcji = war_funkcji;
					S_naj_kat = naj_kat;
					x_s = i;
					y_s = j;
					//cout << "tutaj " << x_s << "  " << y_s << " " << S_war_funkcji << "\n";
				}
			}

			}
		}
	if ((x_s != (-1)) && (y_s != (-1)))
	{
		cout << "przy tworzeniu nowego sklepu \n" << S_war_funkcji << " " << x_s << " " << y_s << "\n";
		utworz_sklep(x_s, y_s, S_naj_kat, S_war_funkcji, zasieg,nr, rozx, rozy, wk, wns, k, zaludnienie);
		(*(wns[nr])).wspx() = x_s;
		(*(wns[nr])).wspy() = y_s;
		(*(wns[nr])).zas() = zasieg;
		(*(wns[nr])).rodz() = S_naj_kat;
		(*(wns[nr])).zysk() = S_war_funkcji;
		*budzet = budzet0;
		return true;
	}
	else return false;

}


void sasiedztwo2(int rozx, int rozy, int zasieg, int *budzet, macierz<int> &czynsz, macierz<int> &k, macierz<int> &zaludnienie, vector<sklep *> &wk, vector<sklep *> &wns,list<ele_tabo*> &lista_tabo, macierz<int> &mac_tabo)
{
	int x, y; //wsp najgorszego sklepu
	int nr; //numer najgorszego sklepu 
	//znalezienie najgorszego sklepu
	znajdz_najg_sklep(wns, &x, &y,&nr);
	cout << "najgorszy sklep ma numer "<<nr+1 << "\n";
	//przydzielenie nowych sklepow do miejsc na ktore dzialal sklep przesuwany
	przydziel_nowe_sklepy(x, y, nr, zasieg, rozx, rozy, k, zaludnienie, wk, wns);
	*budzet = *budzet + czynsz(x, y);
	//cout << k<<"\n";
	bool wynik=przesun_sklep(x, y, nr, zasieg, rozx, rozy, budzet,czynsz,k, zaludnienie, wk, wns,mac_tabo);
	if (wynik)
		aktualizuj_liste_tabo(2, x, y, max_roz_tabo, lista_tabo, mac_tabo);

}
