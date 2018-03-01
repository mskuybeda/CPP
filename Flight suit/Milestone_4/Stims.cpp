#include "Stims.h"

Stims::Stims(string name, Location location, int intensity, double frequency, double duration):
	Stimulation(name) //Call the parent class contructor
{
	setLocation(location);
	setIntensity(intensity);
	setFrequency(frequency);
	setDuration(duration);
}

Stims::~Stims()
{}

void Stims::setLocation(Location location)
{
	m_location = location;
}
void Stims::setIntensity(int intensity)
{
	if (intensity < 0 || intensity > 100) 
	{
		cout << "Wrong argument value. Intensity should be between 0 and 100" << endl;
		exit(1);
	}
	m_intensity = intensity;
}
void Stims::setFrequency(double frequency)
{
	if (frequency < 0)
	{
		cout << "Wrong argument value. Frequency cannot be negative" << endl;
		exit(1);
	}
	m_frequency = frequency;
}
void Stims::setDuration(double duration)
{
	if (duration < 0)
	{
		cout << "Wrong argument value. Duration cannot be negative" << endl;
		exit(1);
	}
	m_duration = duration;
}

void Stims::display(ostream& stream)
{
	stream << string(20, '-') << endl; 

	stream << "Name: " << getName() << endl;
	stream << "Location: ";
	switch (m_location) 
	{
	case Location::m_abs:
		stream << "abs" << endl;
		break;
	case Location::m_front:
		stream << "front" << endl;
		break;
	case Location::m_back:
		stream << "back" << endl;
		break;
	case Location::m_traps:
		stream << "traps" << endl;
		break;
	default:
		break;
	}
	stream << "Intensity: " << m_intensity << "%" << endl;
	stream << "Frequency: " << m_frequency << endl;
	stream << "Duration: " << m_duration << " seconds" << endl;

	stream << string(20, '-') << endl; 
}