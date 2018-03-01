#include "Stimulation.h"

Stimulation::Stimulation(string name)
{
	setName(name);
}
Stimulation::~Stimulation()
{}

string Stimulation::getName()
{
	return m_name;
}
void Stimulation::setName(string name)
{
	m_name = name;
}