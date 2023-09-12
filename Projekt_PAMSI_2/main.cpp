
#include "Stack.h"
#include "Timer.h"
#include "Entry.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>



enum DataType 
{
  Integer,String,Float
};

//To jest funkcja ktorej uzywalem do debugowania.
//Po prostu wyswietla co jest w calym stacku, bardzo pomogla
//potrzebny jest overload << dla typu w stacku
void PrintStack(Stack<Entry>& stack);

//jedyny powod dla ktorego jest low i high to rekursja
void MergeSort(Stack<Entry>& stack, int low, int high);


//wszystko tak naprawde dzieje sie w separate
//jedyny powod dla ktorego jest low i high to rekursja
//stack overflow przy rozmiarze stack >40k, wytlumaczone nizej, przy implementacji
//nie wazne co probowalem, dalej sie to dzialo, wiec sie poddalem
//mozliwe ze to blad w moim srodowisku
void Quicksort(Stack<Entry>& stack,const int& low,const int& high);

//Tutaj pewnie lista by�aby lepsza
//Ten bucketsort jest specyficznie napisany dla tej sytuacji
void Bucketsort(Stack<Entry>& stack);

//nazwa mowi sama za siebie
float Average(Stack<Entry>& stack);

//tutaj tez
float Median(Stack<Entry>& stack) { return stack[(int)stack.GetSize() / 2].GetRating(); };


int main()
{

	//Rozmiar bazy do pobrania
	int database_size = 10;

	//Tutaj b�d� trzymane wpisy
	Stack<Entry> Database(database_size);
	//pomocnicy do operacji
	bool Brackets_title_helper = false;
	std::string Title_helper;
	int imported_el = 0;
	int i = 0;
	std::string tempstr = " ";
	std::string tempstr2 = " ";
	char sep = ',';
	std::stringstream ss;
	std::string sarray[3] = { "int","string","float" };

	//ifstream do pliku z danymi
	std::ifstream ExcelFile("projekt2_dane.csv");

	std::cout << "Searching Database!\n";

	//Tu sie zaczyna odczytywanie pliku
	//Najpierw pobieram ca�y wpis
	while (std::getline(ExcelFile, tempstr) && ExcelFile.good())
	{
		//unikam poczatkowego przypadku i bledu we wpisach
		if (tempstr[0] == ',') continue;

		std::stringstream ss(tempstr);

		//tu dziele wpis na fragmenty
		while(std::getline(ss, tempstr2, sep))
		{
			//Ten fragment odpowiada za przetworzenie danych gdzie
			//tytul jest zapisany w "" zamiast po przecinku 
			if (tempstr2.empty()) continue;
			std::size_t found = tempstr2.find('"');
			if (found != std::string::npos)
			{
				//substring, bo 0 element tempstr2 jest ", a tego nie chcemy
				Title_helper = tempstr2.substr(1,tempstr2.length()-1);
				sep = '"';
				Brackets_title_helper = true;
				continue;
			}
			if (Brackets_title_helper == true)
			{
				Title_helper += "," + tempstr2;
				sep = ',';
				Brackets_title_helper = false;
			}
			if (Integer == i)
			{
				sarray[i] = tempstr2;
			}
			else if (String == i)
			{
				if(!Title_helper.empty()) sarray[i] = Title_helper;
				else sarray[i] = tempstr2;
			}
			else if (Float == i)
			{
				sarray[i] = tempstr2;
			}
			++i;
		}

		//czyszcze bo inaczej problemy
		Title_helper.clear();
		//w try bo stoi i stof moga rzucic wyjatkiem
		try {
		Entry newEntry(stoi(sarray[0]), sarray[1], stof(sarray[2]));
		Database.push(newEntry);
		}
		catch (std::exception e) 
		{ 
			std::cerr << e.what() << std::endl; 
			std::cerr << sarray[0] << sarray[1] << sarray[2] << std::endl;
			//std::cerr << "THIS DATABASE IS FULL OF CURRUPTED ITEMS!" << std::endl;
		};

		i = 0;
		//tu sie upewniam, ze odpowiednia ilosc danych sie pobierze
		++imported_el;
		if (imported_el == database_size)
		{
			std::cout << "\n Imported Elements: " << imported_el << std::endl;
			break;
		}
	}
	
	
	PrintStack(Database);

	//Tutaj sa rozne sortowania, 2 zakomentowane, 1 aktywne

	std::cout << "Entering Quicksort\n";
	{
		Timer timer;
		int zero = 0;
		int DS = Database.GetSize() - 1;
		Quicksort(Database, zero, DS);

	}
	std::cout << "Quicksort End\n";
	
	
	std::cout << "Entering MergeSort\n";
	{
		Timer timer;
		MergeSort(Database, 0, Database.GetSize() - 1);
	}
	std::cout << "Mergesord End\n";
	
	
	std::cout << "Entering BucketSort\n";
	{
		
		Timer timer;
		Bucketsort(Database);
	}
	std::cout << "Bucketsort End\n";
	


	PrintStack(Database);

	//tutaj wyswietlam mediane i srednia
	std::cout << "Median: " << Median(Database) << ", Average: " << Average(Database) << std::endl;

	return EXIT_SUCCESS;
}


void PrintStack(Stack<Entry>& stack)
{
	bool sorted = true;
	std::cout << std::endl;
	for (int i = 0; i < stack.GetSize(); i++)
	{
		//ta czesc sprawdza czy jest dobrze posortowane
		if (i<stack.GetSize() - 1 &&stack[i].GetRating() > stack[i + 1].GetRating()) sorted = false;
		std::cout << "Element " << i << ": " << stack[i] << std::endl;
	}
	if (sorted == false) {
		std::cerr << "The stack is not sorted Properly!";
	}
}

float Average(Stack<Entry>& stack)
{
	float average = 0;
	int i;
	for (i = 0; i < stack.GetSize(); i++)
	{
		average += stack[i].GetRating();
	}
	average = average / i;
	return average;
}

//MAGIC,a tak na powaznie, to tutaj efektywnie dzieje sie sortowanie
void Merge(Stack<Entry>& stack, int low, int mid, int high )
{
	//tymczasowa tablica, bardzo przydatna
	Entry* EnAr = new Entry[stack.GetSize()];

	int i = low;
	int j = mid + 1;
	
	int iter = low;

	//i czyta od lewej do srodka
	//j czyta od srodka do prawej
	while (i <= mid && j <= high)
	{
		if (stack[i].GetRating() < stack[j].GetRating())
		{
			EnAr[iter] = stack[i];
			iter++;
			i++;
		}
		else
		{
			EnAr[iter] = stack[j];
			iter++;
			j++;
		}
	}
	//to jest efektywnie else if tylko dla while
	while (i <= mid)
	{
		EnAr[iter] = stack[i];
		iter++;
		i++;
	}
	//to tez
	while (j <= high)
	{
		EnAr[iter] = stack[j];
		iter++;
		j++;
	}
	//przekopiowanie tymczasowej tablicy do prawdziwej
	for (i = low; i < iter; i++)
	{
		stack[i] = EnAr[i];
	}

	delete[] EnAr;
}



void MergeSort(Stack<Entry>& stack, int low, int high)
{
	if (low < high)
	{
		int mid;

		//tutaj ustalam srodek/miejsce podzialu
		mid = (low + high) / 2;

		//rekurencyjnie zrobione bo iteracyjnie to mordega
		//dziele tablice na dwie czesci bez tworzenia nowych tablic (pseudo tablice)
		MergeSort(stack, low, mid);
		MergeSort(stack, mid + 1, high);

		Merge(stack, low, mid, high);
	}

}


//okropnie napisana przeze mnie funkcja znajdujaca mediane miedzy
//poczatkiem, srodkiem a koncem
int MedianOfThree(Stack<Entry>& stack,const int& low,const int& high)
{
	float arr[3];
	arr[0] = stack[low].GetRating();
	arr[1] = stack[(low + high)/2].GetRating();
	arr[2] = stack[high].GetRating();

	if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
	if (arr[1] > arr[2]) std::swap(arr[1], arr[2]);
	if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);


	
	if (arr[1] == stack[low].GetRating())
	{
		return low;
	}
	else if (arr[1] == stack[(low + high) / 2].GetRating())
	{
		return (int)(low + high) / 2;
	}
	else if (arr[1] == stack[high].GetRating())
	{
		return high;
	}
	else 
	{
		return low;
	}
}

//funkcja dzielaca dane, tutaj efektywnie dzieje sie sortowanie
//tutaj sie dzieje stack overflow przy wyzszych rozmiarach stack (okolo 40k do 50k)
//nie wiem dlaczego, bo wysypuje sie na std::cout<<stack[cokolwiek]
// i  kiedy tworze pivot na stercie, co jest zupelnie absurdalne,
//bo czemu jest STACK overflow, skoro tworze na stercie
int separate(Stack<Entry>& stack,const int& low,const int& high)
{
	//idenks mediany trzech
	int k = MedianOfThree(stack, low, high);
	//std::cout << stack[k] << std::endl;
	Entry* pivot = new Entry(stack[k].GetOrder(), stack[k].GetTitle(), stack[k].GetRating());
	int left = low;

	for (int i = low; i <= high; i++)
	{
		if (stack[i].GetRating() < pivot->GetRating())
		{
			std::swap(stack[i], stack[left]);
			left++;
		}
	}

	//bez tego sie zle sortowalo
	//chodzi pewnie o to ze pivot zawsze musi byc na koncu
	stack[left] = *pivot;

	delete pivot;
	return left;
}



void Quicksort(Stack<Entry>& stack,const int& low,const int& high)
{
	if (low < high)
	{
		//ustalam punkt dzielenia
		int* pivot1 = new int(separate(stack,low,high));
		int* pivot2 = new int(*pivot1 + 1);

		//dziele na dwie pseudo tablice jak w merge sort
		Quicksort(stack, low, *pivot1);
		Quicksort(stack, *pivot2, high);

		delete pivot1;
		delete pivot2;
	}

}


void Bucketsort(Stack<Entry>& stack)
{
	//int Keys[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//tutaj m�g�bym zrobi� tak, �eby funkcja bra�a argument w postaci ilo�ci kluczy
	//ale skoro ilo�� jest znana przy kompilacji mo�na zrobi� tak
	Stack<Entry> B[11];
	Entry e;
	float k;
	int iter = 0;
	//Tutaj sortujemy wedlug "kluczy", czyli ocenie
	for (int i = 0; i<stack.GetSize(); i++)
	{
		e = stack[i];
		k = e.GetRating();
		B[(int)k].push(e);
	}

	//tutaj wrzucamy tablice B do tej
	//ktora chcielismy posortowac
	std::cout << "\nsorting completed, beginning transfer\n";
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < B[i].GetSize(); j++)
		{
			stack[iter] = B[i][j];
			++iter;
		}
	}

}