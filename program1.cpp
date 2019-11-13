#include <iostream>	
#include <fstream>
#include <string>			//zmienilam konstruktora rgb, sprawdz czy dziala		
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <algorithm>
using namespace std;

//struktury używane w programie
struct wspolrzedne
{
	int x;
	int y;
};
struct rgb
{
	int r=0;	//czarnobiale przechowuje w r
	int g=0;
	int b=0;
};

class obraz
{
private:

    vector<vector<rgb> > pixels;		//pierwsza współrzędna - kolumna wierszy

    short typdanych;			//czy ascii czy binary
	bool typkoloru;				//dla 0 - szare, 1 - kololki
    short kolumny, wiersze;
    short maxi;

    void zmien_typ (bool);
    void zmien_kolumny_wiersze (int, int);		
    void zmien_maxi (int);
	bool pytanie (string);

public:	
	obraz();
	obraz(string);
//	int usun_komentarze (int, string);		//w trakcie testów
    string wczytaj_plik (string);			
    bool przycinanie ();
    bool odbijanie_x ();
    bool odbijanie_y ();
    bool usuniecie_koloru ();
    bool skalowanie ();
    bool transformacja ();
    bool zapisz_plik (string);

    bool blad;			//zeby nie pracowal program, gdy jest blad
};

obraz::obraz()
{
	zmien_typ (1);
	typdanych=0;
	zmien_kolumny_wiersze(0,0);
	zmien_maxi (0);
}
obraz::obraz(string nazwapliku)
{
	wczytaj_plik (nazwapliku);
}
void obraz::zmien_typ (bool a)
{
    this->typkoloru=a;		
}
void obraz::zmien_kolumny_wiersze (int a, int b)
{
	this->kolumny=a;
	this->wiersze=b;
}
void obraz::zmien_maxi (int a)		//maksymalna wartosc pixela
{
	this->maxi=a;
}
// int usun_komentarze (int pozycja, string nazwapliku)
// {
//     ifstream  plikin;
//     plikin.open(nazwapliku.c_str());		//otwieram plik i ustawiam pozycję
//  	plikin.seekg( pozycja );

// 	char ctemp1;	plikin>>ctemp1;
//  	string stemp1;
//  	while (ctemp1==35)				//jeśli jest #
//  	{
//  		getline (plikin, stemp1);			//wywala komentarz
//  		plikin>>ctemp1;
//  	}
//  	plikin.seekg( - 1, ios::cur );
//  	pozycja=plikin.tellg();
//  	return pozycja;
// }
string obraz::wczytaj_plik (string nazwapliku) 
{
	blad=1;
    ifstream  plikin;
    plikin.open(nazwapliku.c_str());	
    if (!plikin.good())
        return "Zla nazwa\n";		//gdyby była zła lokalizacja

    string stemp1;
	char ctemp1;		//zwraca aktualną pozycję wskaźnika
    plikin>>ctemp1;
	while (ctemp1==35)				//jeśli jest #
	{
		getline (plikin, stemp1);			//wywala komentarz
		plikin>>ctemp1;
	}	
	plikin.seekg( - 1, ios::cur );
    plikin>>ctemp1>>typdanych;

    if (!ctemp1==80)              //to jest "P", musi byc w ASCII
        return "Nie ma P na poczatku\n";		//gdyby plik nie zawieral poczatku
    if (typdanych==2 || typdanych==5)
    	this->zmien_typ(0);
    else if (typdanych==3 || typdanych==6)
    	this->zmien_typ(1);
    else return "Inny typ\n";			//inny typ pliku
    						//zaczynam wczytywać właściwości
    int kol, wier;

    plikin>>ctemp1;
	while (ctemp1==35)				//jeśli jest #
	{
		getline (plikin, stemp1);			//wywala komentarz
		plikin>>ctemp1;
	}	
	plikin.seekg( - 1, ios::cur );
	plikin>>kol;

    plikin>>ctemp1;
	while (ctemp1==35)				//jeśli jest #
	{
		getline (plikin, stemp1);			//wywala komentarz
		plikin>>ctemp1;
	}	
	plikin.seekg( - 1, ios::cur );
    plikin>>wier;

    this->zmien_kolumny_wiersze (kol, wier);	//tu jest wymiar

    int max;
    plikin>>max;
    this->zmien_maxi (max);		//tu jest max
    if (typdanych==2||typdanych==3)			//dla ascii
    {
    	rgb wczytywanepixele;
		if (this->typkoloru)			//kolorowe wczytywanie
		{
    		for (int i = 0; i < this->wiersze; i++)
    		{
    			vector<rgb> ivtemp;
    			for (int j = 0; j < this->kolumny; j++)		
    			{

					plikin>>ctemp1;
					while (ctemp1==35)				//jeśli jest #
					{
						getline (plikin, stemp1);			//wywala komentarz
						plikin>>ctemp1;
					}	
					plikin.seekg( - 1, ios::cur );    			
    				plikin>>wczytywanepixele.r;		//wczytuje po kolei składowe kololowe 
    												//do zmiennej wczytywanepixele
    				plikin>>wczytywanepixele.g;

    				plikin>>wczytywanepixele.b;

    				ivtemp.push_back(wczytywanepixele);					//wypełniam wiersz
    			}
    			pixels.push_back(ivtemp);			//i wczytuje do kolumny
    		}
		}
		else		//to samo dla czarnobiałych
		{
    		for (int i = 0; i < this->wiersze; i++)
    		{
    			vector<rgb> ivtemp;
    			for (int j = 0; j < this->kolumny; j++)
    			{

					plikin>>ctemp1;
					while (ctemp1==35)				//jeśli jest #
					{
						getline (plikin, stemp1);			//wywala komentarz
						plikin>>ctemp1;
					}	
					plikin.seekg( - 1, ios::cur );    			
    				plikin>>wczytywanepixele.r;

    				wczytywanepixele.g=0;

    				wczytywanepixele.b=0;		//składowe kolorowe=0

    				ivtemp.push_back(wczytywanepixele);
    			}
    			pixels.push_back(ivtemp);
    		}
		}
   	}

   	blad=0;
	return "Plik prawidlowo wczytany\n";
}
bool obraz::pytanie (string komenda)		//pytanie do metod
{
	cout<<"Czy chcialbys "<<komenda<<"?\n T - Tak, N - Nie\n";
	string pytanie;	cin>>pytanie;
	if (pytanie=="Tak"||pytanie=="T"||pytanie=="t"||pytanie=="tak")
		return 1;
	else
		return 0;
}
bool obraz::przycinanie () 
{
	if (!this->pytanie ("przyciac obraz"))
		return 0;

	wspolrzedne poczatek, koniec;
	cout<<"wymiary obrazka to "<<kolumny<<" x "<<wiersze<<endl;
	cout<<"Podaj wspolrzedne lewego, gornego pixela, w ktorym obraz ma byc przyciety mieszczace sie w danym zakresie."<<endl<<
	"(x,y)"<<endl<<"x=";
	cin>>poczatek.x;		//wprowadzanie współrzędnych
	cout<<"y=";
	cin>>poczatek.y;
	cout<<"Podaj wspolrzedne prawego, dolnego pixela, w ktorym obraz ma byc przyciety mieszczace sie w danym zakresie."<<endl<<
	"(x,y)"<<endl<<"x=";
	cin>>koniec.x;
	cout<<"y=";
	cin>>koniec.y;
	
	if (poczatek.x<0)			//gdyby użytkownik zrobił coś źle
		poczatek.x=0;
	if (poczatek.y<0)
		poczatek.y=0;
	if (koniec.x>this->wiersze)
		koniec.x=this->wiersze-1;
	if (koniec.y>this->kolumny)
		koniec.y=this->kolumny-1;
	if (poczatek.x>koniec.x)
	{
		int itemp1=poczatek.x;
		poczatek.x=koniec.x;
		koniec.x=itemp1;
		cout<<"Odwrotna kolejnosc wpisywania zmiennych!\n";
	}
	if (poczatek.y>koniec.y)
	{
		int itemp1=poczatek.y;
		poczatek.y=koniec.y;
		koniec.y=itemp1;
		cout<<"Odwrotna kolejnosc wpisywania zmiennych!\n";
	}


	rgb wczytywanepixele;
	vector<vector<rgb> > ivtemp2D;
    for (int i=poczatek.y; i<koniec.y; i++)
    {
    	vector<rgb> ivtemp;
        for (int j=poczatek.x; j<koniec.x; j++)			//od początku do końca wpisuj do pierwszej komórki
			ivtemp.push_back(pixels.at(i).at(j));			//wypełniam wiersz i push_back
        ivtemp2D.push_back(ivtemp);
    }
    pixels.swap(ivtemp2D);
	this->zmien_kolumny_wiersze(koniec.x-poczatek.x, koniec.y-poczatek.y);
	return 1;
}
bool obraz::odbijanie_x ()		
{
	if(!this->pytanie("odbić względem osi x"))
		return 0;
	vector<vector<rgb> > ivtemp2D;
	for (int i = 0; i<pixels.size(); i++)		
	{
		vector<rgb> ivtemp;
		for (int j = 0; j<pixels.at(i).size(); j++)	//każdy i-ty temp jest pixelem (rozmiar-i-1, i)
			ivtemp.push_back(pixels.at(i).at(pixels.at(i).size()-j-1));
		ivtemp2D.push_back(ivtemp);
	}
	pixels.swap(ivtemp2D);		//zamiana wektorów
	// 	for (int i = 0; i<pixels.size(); i++)		// to działa do obracania
	// {													
	// 	for (int j = 1; j<=pixels.at(i).size(); j++)		
	// 	{
	// 		ivtemp.push_back(pixels.at (pixels.at(i).size()-j) .at (i) );
	// 	}
	// 	ivtemp2D.push_back(ivtemp);
	// 	ivtemp.clear();
	// } 
	// pixels.swap(ivtemp2D);	
	// ivtemp2D.clear();

	return 1;
}
bool obraz::odbijanie_y ()	
{
	if(!this->pytanie("odbić względem osi y"))
		return 0;
	vector<vector<rgb> > ivtemp2D;
	for (int i = 1; i<=pixels.size(); i++)
		ivtemp2D.push_back(pixels.at(pixels.size()-i));	//po prostu przepisuje poczatkowe wektory do koncowych
	pixels.swap(ivtemp2D);
	return 1;
}
bool obraz::usuniecie_koloru()	
{
	rgb zwracane;		//gdyby użytkownik chciał usunąć wszystkie kolory
	int srednia;
	if(this->pytanie("usunac kolor czerwony"))
	{
		if (!typkoloru)
		{
			cout<<"Twoj obraz nie ma kolorow\n";
			return 0;
		}
		zwracane.r=1;		
	}
	if(this->pytanie("usunac kolor zielony"))
	{
		if (!typkoloru)
		{
			cout<<"Twoj obraz nie ma kolorow\n";
			return 0;
		}
		zwracane.g=1;		
	}
	if(this->pytanie("usunac kolor niebieski"))
	{
		if (!typkoloru)
		{
			cout<<"Twoj obraz nie ma kolorow\n";
			return 0;
		}
		zwracane.b=1;		
	}
	if(zwracane.r && zwracane.g && zwracane.b)		//dla wszystkich zmieniamy typ!
	{
		zmien_typ(0);	typdanych--;
		for (int i = 0; i < pixels.size(); i++)
		{
			for (int j = 0; j < pixels.at(i).size(); j++)
			{
				srednia=(pixels.at(i).at(j).r+pixels.at(i).at(j).g+pixels.at(i).at(j).b)/3;	//uśredniam
				pixels.at(i).at(j).r=srednia;		//zeruje kolor i zapisuje tylko czarne
				pixels.at(i).at(j).g=0;
				pixels.at(i).at(j).b=0;
			}
		}
	}
	else if(zwracane.r)
		for (int i = 0; i < pixels.size(); i++)
			for (int j = 0; j < pixels.at(i).size(); j++)
				pixels.at(i).at(j).r=0;		//zerowanie
	else if(zwracane.g)
		for (int i = 0; i < pixels.size(); i++)
			for (int j = 0; j < pixels.at(i).size(); j++)
				pixels.at(i).at(j).g=0;
	else if(zwracane.b)
		for (int i = 0; i < pixels.size(); i++)
			for (int j = 0; j < pixels.at(i).size(); j++)
				pixels.at(i).at(j).b=0;
	return 1;
}
bool obraz::skalowanie()
{
	if (!this->pytanie("przeskalowac obraz"))
		return 0;

	cout<<"Podaj ile razy chcesz przeskalowac obraz."<<endl;
	cout<<"Dla skali dodatniej - powiekszenie."<<endl<<"Dla skali ujemnej - pomniejszenie\n";
	int skala;	cin>>skala;	//zmiana skali
	if (skala>0) 
	{		//tu jest przeładowanie wektora
   		zmien_kolumny_wiersze(this->kolumny*skala, this->wiersze*skala);	
		vector<vector<rgb> > ivtemp2D;			//powiekszam
    	for (int i = 0; i < pixels.size(); i++)
   		{
    		vector<rgb> ivtemp;
    		for (int j = 0; j < pixels.at(i).size(); j++)
    			for (int k = 0; k < skala; k++)			//ileś razy wczytuje tą samą linijkę
    				ivtemp.push_back(pixels.at(i).at(j));
    		for (int k = 0; k < skala; k++)			//ileś razy wczytuje wiersz, zwiększa każdy pixel o wiersz x kolumna
    			ivtemp2D.push_back(ivtemp);
   		}
   		pixels.swap(ivtemp2D);			//teges śmeges fą fą fą
   		return 1;
	}
	if (skala<0)	//zmiejszam
	{
		skala*=-1;
		zmien_kolumny_wiersze(this->kolumny/skala, this->wiersze/skala);		//przeskalowanie
		vector<vector<rgb> > ivtemp2D;		
    	for (int i = 0; i < pixels.size()/skala; i++)
   		{
    		vector<rgb> ivtemp;
    		for (int j = 0; j < pixels.at(i).size()/skala; j++)
    		{
				rgb srednia;
    			for (int k = 0; k < skala; k++)			//powinnam wczytać skala*j, skala*j+1, skala*j+2
    			{
    				for (int l = 0; l<skala; l++)
	    			{
	    				srednia.r+=pixels.at(i*skala+k).at(j*skala+l).r;		//tu sumuje
	    				srednia.g+=pixels.at(i*skala+k).at(j*skala+l).g;		//jezu, ile forów
	    				srednia.b+=pixels.at(i*skala+k).at(j*skala+l).b;
    				}
    			}
    			srednia.r/=skala*skala;		//tu licze srednia z kwadratów o boku skala*skala
    			srednia.g/=skala*skala;
    			srednia.b/=skala*skala;
    			ivtemp.push_back(srednia);
    	   	}
    		ivtemp2D.push_back(ivtemp);
   		}
   		pixels.swap(ivtemp2D);
   		return 1;
	}
}
bool obraz::transformacja()		//ta funkcja działa tylko dla kwadratów!!!!!!!!
{
	if (!this->pytanie("nalozyc transformacje"))
		return 0;
	int liczbakafelek;
	int temp = sqrt(this ->kolumny)+1;
	liczbakafelek=rand()%temp+1;
	vector <int> x(liczbakafelek);
	for (int i = 0; i < liczbakafelek; i++)
		x.at(i)=i;
	random_shuffle(x.begin(), x.end()); 
	vector <int> y(liczbakafelek);
	for (int i = 0; i < liczbakafelek; i++)
		y.at(i)=i;
	random_shuffle(y.begin(), y.end());

	vector<vector<rgb> > ivtemp2D;
	for (int i=0; i<liczbakafelek; i++)		//kafelkowanie x
    {
    	for (int j=0; j < (pixels.size())/liczbakafelek; j++)
    	{
    		vector<rgb> ivtemp;
    	   	for (int k=0; k < pixels.at(j).size(); k++)
    			ivtemp.push_back(pixels.at(k).at(x.at(i)*(pixels.size()/liczbakafelek)+j));
    	   	ivtemp2D.push_back(ivtemp);
    	}
    }
    pixels.swap(ivtemp2D);
   	ivtemp2D.clear();

   	rgb proszedzialaj;
   	proszedzialaj.r=0;
   	proszedzialaj.g=0;
   	proszedzialaj.b=0;
   	for (int i = pixels.size(); i < this->kolumny; i++)		//to robi kwadrat
   	{
   		vector<rgb> ivtemp;
   		for (int j = 0; j < this->kolumny; j++)
   			ivtemp.push_back(proszedzialaj);
   		pixels.push_back(ivtemp);
   	}
   	for (int i=pixels.at(0).size(); i <this->kolumny; i++)
   	{
   		for (int j = 0; j < this->kolumny; j++)
   			pixels.at(j).push_back(proszedzialaj);
   	}


	for (int i = 0; i<pixels.size(); i++)		// to działa do obracania
	{			
		vector<rgb> ivtemp;										
		for (int j = 1; j<=pixels.at(i).size(); j++)		
		{
			ivtemp.push_back(pixels.at (pixels.at(i).size()-j) .at (i) );
		}
		ivtemp2D.push_back(ivtemp);
		ivtemp.clear();
	}
	pixels.swap(ivtemp2D);	
	ivtemp2D.clear();


	for (int i=0; i<liczbakafelek; i++)		//kafelkowanie x
    {
    	for (int j=0; j < (pixels.size())/liczbakafelek; j++)
    	{
    		vector<rgb> ivtemp;
    	   	for (int k=0; k < pixels.at(j).size(); k++)
    			ivtemp.push_back(pixels.at(x.at(i)*(pixels.size()/liczbakafelek)+j).at(k));
    	   	ivtemp2D.push_back(ivtemp);
    	}
    }
    pixels.swap(ivtemp2D);
   	ivtemp2D.clear();
    zmien_kolumny_wiersze(pixels.at(0).size(), pixels.size());
	return 1;
}
bool obraz::zapisz_plik (string nazwapliku)
{
    ofstream  plikout;
    plikout.open(nazwapliku.c_str());
    if (!plikout.good())
        return false;
    plikout<<"P"<<typdanych<<endl<<this->kolumny<<endl<<
    this->wiersze<<endl<<this->maxi<<endl;	//struktura pliku
    if (this->typdanych==2||this->typdanych==3)		//dla ascii
    {
    	if (this->typkoloru)			//kololki
    		for (int i=0; i<this->wiersze; i++)         //wczytuję tablicę
        	{
        		for (int j=0; j<this->kolumny; j++)
        	   		plikout<<" "<<pixels.at(i).at(j).r<<" "<< pixels.at(i).at(j).g<<" "<<pixels.at(i).at(j).b<<" ";
       			plikout<<endl;
    		}
    	else if (!this->typkoloru)
    		for (int i=0; i<this->wiersze; i++)         //wczytuję tablicę
    	    {
    	    	for (int j=0; j<this->kolumny; j++)
    	       		plikout<<pixels.at(i).at(j).r<<" ";
    	   		plikout<<endl;
    		}
    }
    return true;
}   
int main ()
{
    srand(unsigned(time(0)));
	obraz photo;
    string nazwapliku;
    cout<<"Podaj nazwe pliku, gdzie znajduje sie obraz\n";
    cin>>nazwapliku;
    	cout<<photo.wczytaj_plik(nazwapliku);
    if (photo.blad)
    	return 0;
    photo.przycinanie();
    photo.odbijanie_x();		
    photo.odbijanie_y();
    photo.usuniecie_koloru();
    photo.skalowanie();
    photo.transformacja();
    cout<<"Podaj nazwe pliku, gdzie chcesz zapisac obraz\n";
    cin>>nazwapliku;
 	photo.zapisz_plik(nazwapliku);
    return 0;
}
