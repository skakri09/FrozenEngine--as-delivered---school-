#ifndef BoostButton_h__
#define BoostButton_h__

#include <boost/function.hpp>

#include "Logger.h"

class CBoostButton
{
public:
	CBoostButton():log("boostbtn", INFO){};
	~CBoostButton(){};

	boost::function<void(bool)> onClick;

	/*
	template<typename T_1)
		void onClick(T_1, boost::function<void()>)
	{

	}

	template<typename T_1, T_2)
		void onClick(T_1, T_2, boost::function<void()>)
	{

	}

	template<typename T_1, T2, T3)
		void onClick(T_1, T2, T3 boost::function<void()>)
	{

	}*/
private:
	Logger log;
};

#endif // BoostButton_h__
