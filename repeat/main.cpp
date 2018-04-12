#ifdef _WIN32
#	include "resource.h"
#endif // _WIN32

// Required in g++-4.4
#define GLIBCXX_USE_NANOSLEEP

#define USAGE_DEFAULT_STR	L"USAGE: repeat <params> <command(s) to execute>\r\n"
#define USAGE_EXTENDED_STR	USAGE_DEFAULT_STR L"\r\nParams:\r\n\t-t<times>\tTimes to execute the command.\r\n\t\t\tUse \"-tI\" or \"-ti\" to execute it infinitely.\r\n\t-s<time>\tTime to sleep between command executions in ms.\r\n\t-f\t\tForce repeating while generating errors.\r\n\t-h\t\tCalls this help\r\n\r\n"
#define USAGE_HINT_STR		L"\tType \"repeat -h\" for help\r\n\r\n"
#define USAGE_STR			USAGE_DEFAULT_STR USAGE_HINT_STR

#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace chrono;

/// <summary>
/// Main entry point
/// </summary>
/// <param name="argc">Argument count</param>
/// <param name="argv">Argument values</param>
/// <returns>Exit code</returns>
int main(int argc, char *argv[])
{
	int ret = 0, i, times = 0, sleep_time = 0;
	string cmd;
	bool cmd_flag = false, times_flag = false, sleep_time_flag = false, force_flag = false, first = true;
	if (argc > 1)
	{
		for (i = 1; i < argc; i++)
		{
			if (cmd_flag)
			{
				cmd.push_back(' ');
				cmd += argv[i];
			}
			else
			{
				if (argv[i][0] == '-')
				{
					switch (argv[i][1])
					{
					case 't':
						if (times_flag)
						{
							wcout << L"Multiple \"-t\" arguments found\r\n" << USAGE_HINT_STR;
							return 0;
						}
						else
						{
							if ((argv[i][2] == 'I') || (argv[i][2] == 'i'))
							{
								times = -1;
							}
							else if ((argv[i][2] >= '0') && (argv[i][2] <= '9'))
							{
								times = atoi(&(argv[i][2]));
							}
							else
							{
								cout << "Argument \"" << argv[i] << "\" is invalid\r\n";
								wcout << USAGE_HINT_STR;
								return 0;
							}
							times_flag = true;
						}
						break;
					case 's':
						if (sleep_time_flag)
						{
							wcout << L"Multiple \"-s\" arguments found\r\n" << USAGE_HINT_STR;
							return 0;
						}
						else
						{
							if (argv[i][2] == '\0')
							{
								cout << "Argument \"" << argv[i] << "\" is invalid\r\n";
								wcout << USAGE_HINT_STR;
								return 0;
							}
							else
							{
								if ((argv[i][2] >= '0') && (argv[i][2] <= '9'))
								{
									sleep_time = atoi(&(argv[i][2]));
									sleep_time_flag = true;
								}
								else
								{
									cout << "Argument \"" << argv[i] << "\" is invalid\r\n";
									wcout << USAGE_HINT_STR;
									return 0;
								}
							}
						}
						break;
					case 'f':
						if (force_flag)
						{
							wcout << L"Multiple \"-f\" arguments found\r\n" << USAGE_HINT_STR;
							return 0;
						}
						else
						{
							if (argv[i][2] == '\0')
							{
								force_flag = true;
							}
							else
							{
								cout << "Argument \"" << argv[i] << "\" is invalid\r\n";
								wcout << USAGE_HINT_STR;
								return 0;
							}
						}
						break;
					case 'h':
						wcout << USAGE_EXTENDED_STR;
						return 0;
					}
				}
				else
				{
					cmd_flag = true;
					cmd += argv[i];
				}
			}
		}
		if (cmd_flag)
		{
			if (times_flag)
			{
				if (times == -1)
				{
					while (true)
					{
						if (sleep_time_flag)
						{
							if (first)
							{
								first = false;
							}
							else
							{
								this_thread::sleep_for(milliseconds(sleep_time));
							}
						}
						if (force_flag)
						{
							system(cmd.c_str());
						}
						else if (ret = system(cmd.c_str()))
						{
							break;
						}
					}
				}
				else
				{
					for (i = 0; i < times; i++)
					{
						if (sleep_time_flag)
						{
							if (first)
							{
								first = false;
							}
							else
							{
								this_thread::sleep_for(milliseconds(sleep_time));
							}
						}
						if (force_flag)
						{
							system(cmd.c_str());
						}
						else if (ret = system(cmd.c_str()))
						{
							break;
						}
					}
				}
			}
			else
			{
				wcout << L"Parameter \"-t\" has to be defined\r\n" << USAGE_HINT_STR;
			}
		}
		else
		{
			wcout << USAGE_STR;
		}
	}
	else
	{
		wcout << USAGE_STR;
	}
	return ret;
}
