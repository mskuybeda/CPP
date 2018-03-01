#include "Exoskeleton.h"

Exoskeleton::Exoskeleton(string name, int intensity, double duration) :
	Stimulation(name) //Call the constructor of the parent class
{
	setIntensity(intensity);
	setDuration(duration);
}

Exoskeleton::~Exoskeleton()
{}

void Exoskeleton::setIntensity(int intensity)
{
	if (intensity < 0 || intensity > 100) 
	{
		cout << "Wrong argument value. Intensity should be between 0 and 100" << endl;
		exit(1);
	}
	m_intensity = intensity;
}

void Exoskeleton::setDuration(double duration)
{
	if (duration < 0)
	{
		cout << "Wrong argument value. Duration cannot be negative" << endl;
		exit(1);
	}
	m_duration = duration;
}

void Exoskeleton::display(ostream& stream)
{
	stream << string(20, '-') << endl; //splitter

	stream << "Name: " << getName() << endl;
	stream << "Intensity: " << m_intensity << "%" << endl;
	stream << "Duration: " << m_duration << " seconds" << endl;

	stream << string(20, '-') << endl; 
}