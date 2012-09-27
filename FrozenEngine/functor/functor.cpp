// BoostCommandPattern.cpp : Defines the entry point for the console application.
//
//#include <stdafx.h>
//#include <stdarg.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "boost\function.hpp"
#include "boost\bind.hpp"
#include "boost\shared_ptr.hpp"

///////////////////////////////////////////////////////////
/// Final Approach
struct Final
{
	void incAlt() {cout << "Throttle up..." << endl;}
	void decAlt() {cout << "Throttle down..." << endl;}
	void incSpeed() {cout << "Decrease back pressure. yoke forward..." << endl;}
	void decSpeed() {cout << "Increase back pressure..." << endl;}
	void comm(string s) {cout << "This is N123456, " << s << endl;}
};

///////////////////////////////////////////////////////////
/// command object
class command
{
private:
	boost::function<void ()> _f;
public:
	command() {;}
	command(boost::function<void ()> f) : _f(f) {;}

	template <typename T> void setFunction (T t) {_f = t;}
	void execute()
	{
		if(!_f.empty())
			_f();
	}
};

///////////////////////////////////////////
/// main
int main(int argc, char* args[])//, _TCHAR* argv[])
{
	/// prepare for final
	boost::shared_ptr<Final> final(new Final);

	string s = "request landing One-Four with information Papa.";

	// PART 1
	/////////////////////////////////
	/// create commands and execute
	command c;
	c.setFunction(boost::bind(&Final::comm, final, s));

	command da(boost::bind(&Final::decAlt, final));
	command ds(boost::bind(&Final::decSpeed, final));

	c.execute();
	da.execute();
	ds.execute();

	cout << endl << "Part2" << endl;
	// PART 2
	/////////////////////////////////////////////////////////////////////////////////////
	//create a series of commands and stuff them into a container to be sequenced later
	vector<boost::shared_ptr<command> > v;

	boost::shared_ptr<command> spCmd1(new command(boost::bind(&Final::incAlt, final)));
	boost::shared_ptr<command> spCmd2(new command(boost::bind(&Final::decSpeed, final)));
	boost::shared_ptr<command> spCmd3(new command(boost::bind(&Final::decAlt, final)));

	v.push_back(spCmd1);
	v.push_back(spCmd2);
	v.push_back(spCmd3);

	// run the command sequence
	for_each(v.begin(), v.end(), boost::bind(&command::execute, _1));

	system("pause");

	return 0;
}
