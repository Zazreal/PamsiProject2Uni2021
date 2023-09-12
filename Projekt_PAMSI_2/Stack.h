#pragma once
#include <iostream>
template<typename type>
class Stack
{
private:
	int m_size = 0; //ilosc zajetych miejsc
	int m_capacity; //pojemnosc staku
	type* m_elements; //tu jest przechowywany stack
public:
	Stack();
	Stack(int new_size); //konstruktor ktory przyjmuje pojemnosc poczatkowa
	Stack(const Stack& other); //copy construkor, nic ciekawego
	~Stack(); //dekonstruktor trzeba napisac zeby nie bylo memory leak'u
	type& top(); //zwraca to co jest na gorze staku (nie usuwa)
	type pop(); //to samo co top (tutaj usuwa), tylko jeszcze zmniejsza stack
	void push(type new_el); //dodaje nowy element do staku, moze tez powiekszyc pojemnosc
	int GetSize(); //zwraca m_size
	type operator[](const int index) const; //operatoru napisane po to, zebym mogl robic shuffle
	type& operator[](const int index);  //tak naprawde to je po prostu skopiowalem z zadania robionego w 2 semestrze
	inline bool IsEmpty() { if (m_size <= 0) return true; else return false; }; //is empty

};


template<typename type>
inline Stack<type>::Stack()
{
	m_elements = new type[10];
	m_capacity = 10;
}
template<typename type>
inline Stack<type>::Stack(int new_size): m_capacity{new_size}
{
	m_elements = new type[new_size];
}

template<typename type>
inline Stack<type>::Stack(const Stack& other):m_size(other.m_size),m_capacity(other.m_capacity)
{
	m_elements = new type[m_capacity];
	for (int i = 0; i < other.m_size; ++i)
	{
		m_elements[i] = other.m_elements[i];
	}
}

template<typename type>
inline Stack<type>::~Stack()
{
	delete[] m_elements;
}

template<typename type>
inline type& Stack<type>::top()
{
	return m_elements[m_size-1];
}

template<typename type>
inline type Stack<type>::pop()
{

	type temp = m_elements[m_size-1];
	//m_elements[m_size-1] = -1; //traktuje -1 jako usuniecie tutaj bo nie chce zmniejszac o 1
	m_size--;
	return temp;

}

template<typename type>
inline void Stack<type>::push(type new_el)
{
	//to bedzie nowy rozmiar
	int new_capacity = 1 + m_capacity * 1.5;
	if (m_size >= m_capacity) {

		type* new_elements = new type[new_capacity];

		//tutaj kopiuje wszystkie obiekty ze staku
		//memcpy moze by dzialalo, ale na przyklad kazdy packet ma inny rozmiar, wiec by nie dzialalo.
		for (int i = 0; i < m_capacity; i++)
		{
			//std::cout << m_elements[i] << std::endl;
			new_elements[i] = m_elements[i];
			//std::cout << new_elements[i] << std::endl;
		}
		m_capacity = new_capacity;
		delete[] m_elements; //bez tego bylby memory leak!
		m_elements = new_elements;
	}
	
	m_elements[m_size] = new_el;
	m_size++;
}

template<typename type>
inline int Stack<type>::GetSize()
{
	return this->m_size;
}

template<typename type>
type Stack<type>::operator[](const int index) const
{

	try {
		if (index >= m_size) throw "Index out of range!";
		//dzieki %m_size nie bedzie crasha, zakladajac ze jakos obejdzie throw
		return this->m_elements[index % m_size];
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
		//zwraca 0 jako default
		return this->m_elements[0];
	}
}

template<typename type>
type& Stack<type>::operator[](const int index)
{
	try {
		if (index >= m_size) throw "Index out of range!";
		//dzieki %m_size nie bedzie crasha, zakladajac ze jakos obejdzie throw
		return this->m_elements[index % m_size];
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
		//zwraca 0 jako default
		return this->m_elements[0];
	}
}