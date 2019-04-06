#include "Console.h"
#include "Compute.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

void Console::start()
{
    
    welcome();
    string buf;
	fs.open("answer.txt", std::fstream::out);
	while(true)
	{
	    if(sig) break;
	    
		cout<<">>> ";
		//read input from user
		if(getline(cin, buf))
		{
		    //cout<<buf<<endl;
		    if(!checkCmd(buf))
    		{
				solve(buf);
    		}
		}
		else //EOF
		{
		    break;
		}
	}
	exit();
}

void Console::solve(const string& input)
{
	string ans = pc(input);
	fs << ans << endl;
	fs << "==========================" << endl;
	cout << ans << endl;
}

bool Console::checkCmd(const string& input)
{
    stringstream ss(input);
    string cmd;
    ss >> cmd;
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if(cmd == "help")
    {
        help();
    }
    else if(cmd == "credits")
    {
        credits();
    }
    else if(cmd == "reset")
    {
        reset();
    }
    else if(cmd == "exit")
    {
        sig = 1;
    }
	else if (cmd == "print")
	{
		string buf;
		getline(ss, buf);
		this->solve(buf);
	}
    else
    {
        return false;
    }
    return true;
}

namespace
{
    void except()
    {
        srand(time(NULL));
        cerr<<"Your cpu has overheated ("<<rand()%30 + 100<<" degree Celcius). Mathon is shutting down now to keep you out from danger."<<endl;
        abort();
    }
}

void Console::welcome()
{
    set_terminate(except);
    log("Mathon (sianz-me-release-1.0) (b.105.15.034)");
    log("Type 'help', 'credits' for more informations");
    log();
}

void Console::help()
{
    log("----------------------------------------help----------------------------------------");
    log();
    log("set [type] [name] = [value]               | set variable with specific type value");
    log("help                                      | help manual");
    log("reset                                     | reset the calculator (clear variables)");
    log("exit                                      | exit console");
    log();
    log("supported arithmetic operators:");
    log();
    log("[value] [+,-,*,/,^,%] [value]             | performs basic arithmetic operation");
    log("sqrt([value])                             | squaring the value");
    log("pow([base], [exp])                        | multiply the [base] up to [exp] power");
    log("[value] [==,<=,>=,!=,<,>] [value]         | performs comparison among two values");
    log();
    log("------------------------------------------------------------------------------------");
}

void Console::credits()
{
    log("----------------------------------------credits-------------------------------------");
    log();
    log("---######---######-----###-----###---##---#######---##----##-----###-----###---##---");
    log("---##---------##------##-##----##-#--##-------##----##----##----##-##----##-#--##---");
    log("---######-----##-----#######---##-##-##-----###-----########---#######---##-##-##---");
    log("-------##-----##-----##---##---##--#-##----##-------##----##---##---##---##-##-##---");
    log("---######---######---##---##---##---###---#######---##----##---##---##---##---###---");
    log();
    log("------------------------------------------------------------------------------------");
}

void Console::reset()
{
    this->pc = Compute();
    log("Resetted.");
}

void Console::exit()
{
    log();
    log("Quitting...");
    log("Have a nice day :)");
    log();
}