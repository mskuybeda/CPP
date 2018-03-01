#include "ARAIG_Sensors.h"

ARAIG_Sensors::ARAIG_Sensors(string stimulation_config_filename, string task_config_filename)
{
	ifstream stimulation_config(stimulation_config_filename);
	ifstream task_config(task_config_filename);

	CSV_row row;

	while (stimulation_config >> row)
	{
		if (row.size() < 1) // empty filed protection
			break;

		if (row[0] == "stim")
		{
			Stims* new_stims = new Stims(row[1], string_to_location(row[2]), stoi(row[3]), stod(row[4]), stod(row[5]));
			m_stimulations.push_back(new_stims);
		}
		else
			if (row[0] == "exoskeleton")
			{
				Exoskeleton* new_exoskeleton = new Exoskeleton(row[1], stoi(row[2]), stod(row[3]));
				m_stimulations.push_back(new_exoskeleton);
			}
	}

	CSV_row row1;

	while (task_config >> row1)
	{
		if (row1.size() < 1) 
			break;	

		if (row1[0] == "TASK")
		{
			Task* new_task = new Task(row1[1]);
			m_tasks.push_back(new_task);
		}
		else
		{
			for (int i = 0; i < m_stimulations.size(); i++)
				if (row1[0] == m_stimulations[i]->getName())
				{
					m_tasks[m_tasks.size() - 1]->add(m_stimulations[i]);
					break;
				}
		}
	}
}

ARAIG_Sensors::~ARAIG_Sensors()
{
	for (int i = 0; i < m_stimulations.size(); i++)
		delete m_stimulations[i];
	for (int i = 0; i < m_tasks.size(); i++)
		delete m_tasks[i];
}

Location ARAIG_Sensors::string_to_location(string str)
{
	if (str == "front")
		return Location::m_front;
	else
		if (str == "back")
			return Location::m_back;
		else
			if (str == "abs")
				return Location::m_abs;
			else
				if (str == "traps")
					return Location::m_traps;
				else
				{
					cout << "Wrong location in stimulation config: " << str << endl;
					exit(1);
				}
}

void ARAIG_Sensors::dump(ostream& stream)
{
	for (int i = 0; i < m_tasks.size(); i++)
	{
		stream << string(20, '*') << endl;
		m_tasks[i]->dump(stream);
		m_tasks[i]->execute(stream);
	}
}

Task* ARAIG_Sensors::get_task(string name)
{
	for (int i = 0; i < m_tasks.size(); i++)
		if (m_tasks[i]->get_name() == name)
			return m_tasks[i];
	cout << "No tasks found with name " << name << endl;
	return NULL;
}