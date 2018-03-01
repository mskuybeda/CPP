#ifndef TASK_H
#define TASK_H

#include <list>

#include "Stimulation.h"

using namespace std;

class Task
{
public:
	Task(string);
	Task(const Task&); // Copy constructor
	Task(Task&&); // Move constructor

	void add(Stimulation*); //Add new element to list
	void remove(string); //remove the first element from the list

	Stimulation* operator[](size_t);

	string get_name(); // Getter method for m_name
	int get_size(); // method of output for stimulations amount

	void dump(ostream&);  // Method of the object configuration
	void execute(ostream&); // Method of the object outout

private:
	string m_name;
	list<Stimulation*> m_stimulations;

};
#endif // !TASK_H