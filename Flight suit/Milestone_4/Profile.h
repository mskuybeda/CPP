#ifndef PROFILE_H
#define PROFILE_H

#include <vector>
#include <iterator>
#include "Task.h"
#include "ARAIG_Sensors.h"

using namespace std;

struct Person
{
	string first_name;
	string last_name;
	int number;
};

class Profile
{
public:
	Profile(string, ostream&, ARAIG_Sensors*);
	~Profile();

	void display_tasks_to_run();
	void display_completed_tasks();
	void display_next_task();
	void display_last_completed_task();
	void run();
	void display_student();
	void display_instructor();
	void display_intensity();
	int get_tasks();

private:
	ostream& m_stream;

	ARAIG_Sensors* m_sensors;

	Person m_student;
	Person m_instructor;
	int m_min_intensity;
	int m_max_intensity;

	vector<Task*> m_to_run;
	vector<Task*> m_completed;
};


#endif // PROFILE_H
