#include "Entry.h"
#include <iostream>
Entry::Entry() : m_order(-1), m_rating(5),m_title("none")
{
}

Entry::Entry(const int& order, const std::string& title, const float& rating):
	m_order(order), m_rating(rating), m_title{ title }
{
}

Entry::~Entry()
{
}

std::string Entry::GetTitle()
{
	return m_title;
}

int Entry::GetOrder()
{
	return m_order;
}

float Entry::GetRating()
{
	return m_rating;
}
