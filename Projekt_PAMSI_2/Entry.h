#pragma once

#include <string>
#include <iostream>
#include <memory>
class Entry
{
private:
	int m_order;
	std::string m_title;
	float m_rating;
public:
	Entry();
	Entry(const int& order, const std::string& title, const float& rating); //przyjmuje wszystkie potrzebne dane
	//Entry(const Entry& other); //copy constructor
	~Entry();
	std::string GetTitle(); //zwraca string m_word
	int GetOrder(); // zwraca m_order
	float GetRating();
	//Entry operator=(const int& a); // to jest uzywane do 2 rzeczy: przypisanie order i jesli a == -1 to m_word bedzie zamienione na pustke
	friend std::ostream& operator<<(std::ostream& stream, const Entry& EN) //wypisuje m_word do ekranu
	{
		stream << "Entry nr " << EN.m_order << ": " << EN.m_title << ", rating: " << EN.m_rating;
		return stream;
	}
};

