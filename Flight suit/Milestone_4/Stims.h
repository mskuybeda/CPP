#ifndef STIMS_H
#define STIMS_H

#include "Stimulation.h"

enum Location {m_abs, m_front, m_back, m_traps}; //location field count type

class Stims: public Stimulation
{
public:
	Stims(string, Location, int, double, double); // constructor with parameters

	~Stims(); // Destructor

	// Setters
	void setLocation(Location);
	void setIntensity(int);
	void setFrequency(double);
	void setDuration(double);

	virtual void display(ostream&); //Abstarct class overloading for an object output

private:
	Location m_location; // the location of the Stims on the suit, which can be {abs, front, back, traps}
	int m_intensity; // the intensity value of the Stims ranging from 0% - 100%
	double m_frequency; // the frequency value of the Stims
	double m_duration; // the duration (in seconds) of how long the Stims stimulation should be active
};

#endif // !STIMS_H