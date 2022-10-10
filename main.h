#ifndef main_h
#define main_h

#include <iostream>

bool koniec_gry;
bool wczytywanie_gry;
// OPCJE - zawartoœæ poni¿szych zmiennych mo¿na zmieniæ w menu gry
char znak_statku, znak_trafienia, znak_pudlo;
int czteromasztowce, trojmasztowce, dwumasztowce, jednomasztowce;
bool faza_ai;


void menu();
void opcje();
void inicjalizacja();
void kolorowanie(int kolor);
int wybor_statku(int typ_statku);

class Plansza
{
private:
	int liczba_wierszy = 10;
	int liczba_kolumn = 10;
	char pola_planszy_gracza[15][15] = {};
	char pola_planszy_ai[15][15] = {};

public:
	Plansza(int a, int b) : liczba_kolumn(a), liczba_wierszy(b)
	{}

	void narysuj()
	{
		std::cout << "   ";
		for (int i = 0; i < liczba_kolumn; i++)
		{
			printf("%c ", 65 + i); // numeracja kolumn pierwszej planszy
		}
		std::cout << "             ";
		for (int i = 0; i < liczba_kolumn; i++)
		{
			printf("%c ", 65 + i); // numeracja kolumn drugiej planszy
		}

		for (int i = 0; i < liczba_wierszy; i++)
		{
			std::cout << std::endl;
			if (i < 9)
			{
				std::cout << " ";		// wyrównanie liczb jednocyftrowych do dwucyfrowych
			}
			std::cout << i + 1 << " ";	//numeracja wierszy pierwszej planszy
			for (int j = 0; j < liczba_kolumn; j++) //wyœwietla wiersz pierwszej planszy
			{
				/*if (statki[10 * i + j] == znak_statku)
				{
					KOLOR(11); //jasnoniebieski
					cout << statki[10 * i + j] << " "; //zaznaczenie kolorem statków gracza
					KOLOR(7); //normalny
				}
				else if (10 * i + j == pole)
				{
					KOLOR(12); //czerwony
					cout << statki[10 * i + j] << " "; //zaznaczenie na czerwono ostatniego strza³u przeciwnika
					KOLOR(7); //normalny

				}
				else*/
				std::cout << pola_planszy_gracza[i][j] << " ";
			}
			std::cout << "          ";			// przerwa miêdzy planszami
			if (i < 9)
			{
				std::cout << " ";
			}
			std::cout << i + 1 << " ";	//numeracja wierszy drugiej planszy
			for (int j = 0; j < liczba_kolumn; j++) //wyœwietla wiersz drugiej planszy obok pierwszej
			{
				/*	if (10 * i + j == poprzednie_pole)
				{
					KOLOR(11); //jasnoniebieski
					cout << strzaly[10 * i + j] << " ";	// zaznaczenie na niebiesko ostatniego strza³u gracza
					KOLOR(7); //normalny

				}
				else*/
				std::cout << pola_planszy_ai[i][j] << " ";
			}
		}
	}


};


class Wspolrzedne
{
	int wiersz;
	int kolumna;
};

class Statek
{
private:
	bool statek_gracza;
	int wytrzymalosc;
public:
	virtual void ustaw_statek() = 0;
};

class Jednomasztowiec : public Statek
{
	
};

class Dwumasztowiec : public Statek
{

};

class Trojmasztowiec : public Statek
{

};

class Czteromasztowiec : public Statek
{

};














#endif