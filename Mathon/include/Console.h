#pragma once

#include "Compute.h"
#include <iostream>
#include <string>
#include <fstream>


class Console
{
    Compute pc;
	std::fstream fs;
    int sig = 0;
    
    void welcome();
	void solve(const std::string&);
    void set(std::string);
    void help();
    void credits();
    void reset();
    void exit();
    
    //Command parser
    bool checkCmd(const std::string&);
    
    //Logger
    void log() { std::cout<<std::endl; }
    template<typename First, typename ... Strings> 
    void log(const First& first, const Strings&... msg){ std::cout<<first; log(msg...); }
public:
    void start();

};
