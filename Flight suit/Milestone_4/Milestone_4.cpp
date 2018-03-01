// Milestone_4.cpp : Defines the entry point for the console application.
//
#include "Profile.h"
#include <fstream>

int main(int argc, char *argv[])
{
	string stimulation_config, task_config, profile_config, output;

	if (argc < 5)
	{
		stimulation_config = "StimulationConfig.csv";
		task_config = "TaskConfiguration.csv";
		profile_config = "SampleProfileConfiguration.csv";
		output = "output.txt";
	}
	else
	{
		stimulation_config = argv[1];
		task_config = argv[2];
		profile_config = argv[3];
		output = argv[4];
	}

	ARAIG_Sensors* sensors = new ARAIG_Sensors(stimulation_config, task_config);
	ofstream output_file(output);
	Profile* profile = new Profile(profile_config, output_file, sensors);
	profile->display_student();
	profile->display_instructor();
	profile->display_intensity();
	profile->display_tasks_to_run();
	
	while (profile->get_tasks())
		profile->run();

	cout << "Output in file: " << output << endl;
	system("pause");

	delete profile;

	return 0;
}

