#ifndef STIMULATION_H
#define STIMULATION_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

class Stimulation
{
public:
	Stimulation(string);
	~Stimulation();

	string getName(); //getter method for m_name
	void setName(string); // setter method m_name

	virtual void display(ostream&) = 0;  // Abstarct method for an object output

private:
	string m_name;

};
#endif // !STIMULATION_H