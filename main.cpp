//POP_2020_01_11_projekt_2_Szczerba_Katarzyna_IBM_1_184385  DEV-C++ 5.11  Kompilator:TDM-GCC 4.9.2 64-bit
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdint.h>

using namespace std;


const int liczba_pobieranych_bajtow = 3;
const int rozmiar_napisow_max = 80;
const char dopelnienie='=';
const int szesc_jedynek_bitowych_1=63;
const int szesc_jedynek_bitowych_2=4032;
const int szesc_jedynek_bitowych_3=258048;
const int szesc_jedynek_bitowych_4=16515072;


void wczytaj_klucz(char tablica_kod[64], char nazwa_p_klucz[rozmiar_napisow_max]);
bool pobierz_nazwy_plikow(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy,char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[]);
void koduj_plik_wejsciowy(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy, char tablica_kod[],char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[]);
int sprawdz_rozmiar_pliku(ifstream& plik_wejsciowy,char nazwa_p_wejsciowy[]);
void koduj_trzy_bajty(unsigned int trzy_bajty, char tablica_kod[],char zakodowane_jednostki[]);
void zakoduj_pozostale(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy, char tablica_kod[],char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[],int ile);
bool czy_dalej();


int main()
{
	char nazwa_plik_klucz[] = "tablica_kodowania.txt";
	char tablica_kodowania[64];

	wczytaj_klucz(tablica_kodowania, nazwa_plik_klucz);

	cout << "Program realizuje kodowanie Base64 wybranych plikow. ";

	do
	{
		ifstream plik_wejsciowy;
		ofstream plik_wyjsciowy;
		char nazwa_p_wejsciowy[rozmiar_napisow_max]={'\0'};
        char nazwa_p_wyjsciowy[rozmiar_napisow_max]={'\0'};

		while (pobierz_nazwy_plikow(plik_wejsciowy, plik_wyjsciowy, nazwa_p_wejsciowy, nazwa_p_wyjsciowy) == false);
		koduj_plik_wejsciowy(plik_wejsciowy, plik_wyjsciowy, tablica_kodowania, nazwa_p_wejsciowy, nazwa_p_wyjsciowy);
		
	} while (czy_dalej() == true);

	return 0;
}


void wczytaj_klucz(char tablica_kod[64], char nazwa_p_klucz[rozmiar_napisow_max])
{
	ifstream klucz(nazwa_p_klucz);
	if (!klucz)
	{
		cout << "Blad odczytu pliku wejsciowego z kluczem" << endl;
	}

	int liczba;
	char znak;

	while (klucz >> liczba >> znak)
	{
		tablica_kod[liczba] = znak;
	}

	klucz.close();
	klucz.clear();
}


bool pobierz_nazwy_plikow(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy, char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[])
{
	cout << "Podaj nazwe pliku wejsciowego: ";

	cin.getline(nazwa_p_wejsciowy, rozmiar_napisow_max);
	cin.clear();

	plik_wejsciowy.open(nazwa_p_wejsciowy, ios::binary);
	if (!plik_wejsciowy)
	{
		cout << "Blad otwierania pliku wejsciowego" << endl;
		return false;
	}
	else
	{
		cout << "Podaj nazwe pliku do zapisu wyniku kodowania: ";

		cin.getline(nazwa_p_wyjsciowy, rozmiar_napisow_max);
		cin.clear();

		plik_wyjsciowy.open(nazwa_p_wyjsciowy);
		if (!plik_wyjsciowy)
		{
			cout << "Blad otwierania pliku wyjsciowego" << endl;
			return false;
		}
		else
		{
		    plik_wejsciowy.close();
		    plik_wyjsciowy.close();
			return true;
		}
	}
}


void koduj_plik_wejsciowy(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy, char tablica_kod[],char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[])
{
	int rozmiar_pliku = sprawdz_rozmiar_pliku(plik_wejsciowy, nazwa_p_wejsciowy);
	char zakodowane_jednostki[4];
	int i = 0;
	
	while ((i + liczba_pobieranych_bajtow) <= rozmiar_pliku)
	{   
	    uint8_t bajty[3];
		plik_wejsciowy.open(nazwa_p_wejsciowy,ios::binary);
		if (!plik_wejsciowy)
		{
			cout << "Blad otwierania pliku wejsciowego podczas kodowania" << endl;
			exit(1);
		}
		plik_wejsciowy.seekg(i*sizeof(char), ios::beg);
		plik_wejsciowy.read((char*)&bajty, liczba_pobieranych_bajtow);
		plik_wejsciowy.close();
		plik_wejsciowy.clear();
		
		unsigned int trzy_bajty = 0;
		trzy_bajty = bajty[2] | (bajty[1] << 8) | (bajty[0] << 16); //suma trzech roznych osemek bitow, najmlodsze, potem srednie przesuniete o 8 w lewo, najstarsze przesuniete o 16 w lewo
		koduj_trzy_bajty(trzy_bajty, tablica_kod, zakodowane_jednostki);
		plik_wyjsciowy.open(nazwa_p_wyjsciowy,ios::app);
		if (!plik_wyjsciowy)
		{
			cout << "Blad otwierania pliku wyjsciowego podczas kodowania" << endl;
			exit(1);
		}
		plik_wyjsciowy << zakodowane_jednostki[0] << zakodowane_jednostki[1] << zakodowane_jednostki[2] << zakodowane_jednostki[3];
		plik_wyjsciowy.close();
		plik_wyjsciowy.clear();
		i = i + liczba_pobieranych_bajtow;
	}

	if (rozmiar_pliku % liczba_pobieranych_bajtow == 1)	//zostal jeden bajt zamiast 3 
	{	
		int k=1;
		zakoduj_pozostale(plik_wejsciowy, plik_wyjsciowy, tablica_kod, nazwa_p_wejsciowy, nazwa_p_wyjsciowy, k);
	}
	else if (rozmiar_pliku % liczba_pobieranych_bajtow == 2) //zostaly dwa bajty 
	{ 	
		int k=2;
		zakoduj_pozostale(plik_wejsciowy, plik_wyjsciowy, tablica_kod, nazwa_p_wejsciowy, nazwa_p_wyjsciowy, k);
	}
	
}


int sprawdz_rozmiar_pliku(ifstream& plik_wejsciowy,char nazwa_p_wejsciowy[])
{
    plik_wejsciowy.open(nazwa_p_wejsciowy);
	plik_wejsciowy.seekg(0, ios::end); //ustawiamy pointer na koniec, i odczytujemy jego pozycje
	
	int rozmiar_pliku = plik_wejsciowy.tellg();

	plik_wejsciowy.seekg(0,ios::beg); //przywroc pointer na poczatek pliku
    plik_wejsciowy.close();
    plik_wejsciowy.clear();
    
	return rozmiar_pliku;
}


void koduj_trzy_bajty(unsigned int trzy_bajty, char tablica_kod[],char zakodowane_jednostki[])
{
	unsigned int pierwsza_j = 0;
	unsigned int druga_j = 0;
	unsigned int trzecia_j = 0;
	unsigned int czwarta_j = 0;

	czwarta_j = trzy_bajty & szesc_jedynek_bitowych_1; //pierwsze szejsc bitow
	trzecia_j = (trzy_bajty & szesc_jedynek_bitowych_2) >> 6;//iloczyn bitowy z konkretna potega liczby 2 sluzy do zerowania niepotrzebnych bitow
	druga_j = (trzy_bajty & szesc_jedynek_bitowych_3) >> 12;
	pierwsza_j = (trzy_bajty & szesc_jedynek_bitowych_4) >> 18;

	zakodowane_jednostki[0] = tablica_kod[pierwsza_j];
	zakodowane_jednostki[1] = tablica_kod[druga_j];
	zakodowane_jednostki[2] = tablica_kod[trzecia_j];
	zakodowane_jednostki[3] = tablica_kod[czwarta_j];
}

void zakoduj_pozostale(ifstream& plik_wejsciowy, ofstream& plik_wyjsciowy, char tablica_kod[],char nazwa_p_wejsciowy[], char nazwa_p_wyjsciowy[],int ile) 
{
	char zakodowane_jednostki[4];
	uint8_t bajty[3];
	int indeks_koniec=2;
	for(int i=liczba_pobieranych_bajtow-ile; i>0; i--)
	{
		bajty[indeks_koniec] = 0;
		indeks_koniec--;
	}
	plik_wejsciowy.open(nazwa_p_wejsciowy, ios::binary);
	if (!plik_wejsciowy)
	{
		cout << "Blad otwierania pliku wejsciowego podczas kodowania" << endl;
		exit(1);
	}
	plik_wejsciowy.seekg(-ile, ios::end);
	plik_wejsciowy.read((char*)&bajty, ile);
	plik_wejsciowy.close();
	plik_wejsciowy.clear();

	unsigned int trzy_bajty = 0;
	trzy_bajty = bajty[2] | (bajty[1] << 8) | (bajty[0] << 16);
	koduj_trzy_bajty(trzy_bajty, tablica_kod, zakodowane_jednostki);
	plik_wyjsciowy.open(nazwa_p_wyjsciowy, ios::app);
	if (!plik_wyjsciowy)
	{
		cout << "Blad otwierania pliku wyjsciowego podczas kodowania" << endl;
		exit(1);
	}
	plik_wyjsciowy << zakodowane_jednostki[0] << zakodowane_jednostki[1];
	if(ile==1)
	{
		plik_wyjsciowy<< dopelnienie << dopelnienie;
	}
	else 
	{
		plik_wyjsciowy<< zakodowane_jednostki[2] << dopelnienie;
	}	
		
	plik_wyjsciowy.close();
	plik_wyjsciowy.clear();
}


bool czy_dalej()
{
	char klawisz[3];

	cout << "Czy zakodowac cos jeszcze?(t/n): ";

	cin.getline(klawisz, 3);
	cin.clear();
	cin.ignore();

	if (klawisz[0] == 't' and klawisz[1] == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}

}

