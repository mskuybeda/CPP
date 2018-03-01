#include "Profile.h"

Profile::Profile(string filename, ostream& stream, ARAIG_Sensors* sensors):
	m_sensors(sensors),
	m_stream(stream)
{
	ifstream profile_config(filename);

	CSV_row row;

	profile_config >> row;
	
	m_student.first_name = row[0];
	m_student.last_name = row[1];
	m_student.number = stoi(row[2]);

	profile_config >> row;

	m_instructor.first_name = row[0];
	m_instructor.last_name = row[1];
	m_instructor.number = stoi(row[2]);

	profile_config >> row;
	
	m_min_intensity = stoi(row[0]);
	m_max_intensity = stoi(row[1]);

	while (profile_config >> row)
	{
		if (row.size() < 1) // empty field protection
			break;

		m_to_run.push_back(m_sensors->get_task(row[0]));
	}
}
Profile::~Profile()
{
	delete m_sensors;
	
	m_to_run.clear();
	m_completed.clear();
}

int Profile::get_tasks()
{
	return m_to_run.size();
}

void Profile::display_tasks_to_run()
{
	m_stream << "Tasks to run:" << endl;
	if (m_to_run.size())
		for (int i = 0; i < m_to_run.size(); i++)
			m_to_run[i]->dump(m_stream);
	else
		m_stream << "No tasks to run" << endl;
}
void Profile::display_completed_tasks()
{
	m_stream << "Completed tasks:" << endl;
	if (m_completed.size())
		for (int i = 0; i < m_completed.size(); i++)
			m_completed[i]->dump(m_stream);
	else
		m_stream << "No completed tasks" << endl;
}
void Profile::display_next_task()
{
	if (m_to_run.size())
		m_to_run.front()->dump(m_stream);
	else
		m_stream << "No more tasks to run" << endl;
}
void Profile::display_last_completed_task()
{
	m_stream << "Last completed task: " << endl;
	if (m_completed.size())
		m_completed.back()->dump(m_stream);
	else
		m_stream << "No completed tasks" << endl;
}
void Profile::display_student()
{
	m_stream << "Student #" << m_student.number << ":" << endl;
	m_stream << m_student.first_name << " " << m_student.last_name << endl;
}
void Profile::display_instructor()
{
	m_stream << "Instructor #" << m_instructor.number << ":" << endl;
	m_stream << m_instructor.first_name << " " << m_instructor.last_name << endl;
}
void Profile::display_intensity()
{
	m_stream << "Min intensity: " << m_min_intensity << endl;
	m_stream << "Max intensity: " << m_max_intensity << endl;
}
void Profile::run()
{
	if (m_to_run.size())
	{
		
		m_to_run.front()->execute(m_stream);

		auto it = std::next(m_to_run.begin(), 1);

		std::move(m_to_run.begin(), it, std::back_inserter(m_completed));

		m_to_run.erase(m_to_run.begin(), it);
	}
	else
		m_stream << "No more tasks to run" << endl;
}