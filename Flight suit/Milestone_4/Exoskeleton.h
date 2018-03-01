#ifndef EXOSKELETON_H
#define EXOSKELETON_H

#include "Stimulation.h"

class Exoskeleton: public Stimulation
{
public:
	Exoskeleton(string, int, double); // Constructor with parameters

	~Exoskeleton(); // Destructor

	void setIntensity(int);
	void setDuration(double);

	virtual void display(ostream&); // Abstarct class overloading for an object output

private:
	int m_intensity; // the intensity value of the Exoskeleton ranging from 0% - 100%
	double m_duration; // the duration (in seconds) of how long the Exoskeleton stimulation should be active
};

#endif // !EXOSKELETON_H