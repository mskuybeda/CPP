#ifndef ARAIG_SENSORS_H
#define ARAIG_SENSORS_H

#include "Stims.h"
#include "Exoskeleton.h"
#include "Task.h"
#include "CSV_row.h"

#include <vector>
#include <fstream>

using namespace std;

class ARAIG_Sensors
{
public:
	ARAIG_Sensors(string, string);
	~ARAIG_Sensors();

	void dump(ostream&);
	Task* get_task(string);

private:
	Location string_to_location(string); // Transfer of the string type to Location type

	vector<Stimulation*> m_stimulations;
	vector<Task*> m_tasks;
};

#endif // !ARAIG_SENSORS_H