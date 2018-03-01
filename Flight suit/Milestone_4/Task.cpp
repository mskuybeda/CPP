#include "Task.h"

Task::Task(string name)
{
	m_name = name;
}
Task::Task(const Task& other)
{
	m_stimulations = other.m_stimulations;
	m_name = other.m_name;
}
Task::Task(Task&& other)
{
	m_stimulations = other.m_stimulations;
	m_name = other.m_name;

	other.m_stimulations.clear();
	other.m_name.clear();
}
string Task::get_name()
{
	return m_name;
}
int Task::get_size()
{
	return m_stimulations.size();
}
void Task::add(Stimulation* stimulation)
{
	m_stimulations.push_back(stimulation);
}
void Task::remove(string name)
{
	list<Stimulation*>::iterator i = m_stimulations.begin();
	while (i != m_stimulations.end())
	{
	    if (!name.compare((*i)->getName()))
	    {
	        m_stimulations.erase(i++);
	    }
	    else
	    {
	        ++i;
	    }
	}
}
Stimulation* Task::operator[](size_t index)
{
	if (index >= m_stimulations.size())
	{
		cout << "Error: wrong index(" << index << "). List size is " << m_stimulations.size() << endl;
		exit(1);
	}
	list<Stimulation*>::iterator i = m_stimulations.begin();

	advance(i, index);

	return *i;
}
void Task::dump(ostream& stream)
{
	stream << "Task name: " << m_name << endl;
	stream << "# of stimulations: " << m_stimulations.size() << endl;
}
void Task::execute(ostream& stream)
{
	std::list<Stimulation*>::const_iterator iterator;
	for (iterator = m_stimulations.begin(); iterator != m_stimulations.end(); ++iterator) {
		(*iterator)->display(stream);
	}
}