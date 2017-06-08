#pragma once

#include "BaseIncludes.h"
#include "LogMessage.h"

#ifdef _DEBUG
//#define LOG( msg ) Logger::instance().log( msg )
#define LOG( author, message, verbosity ) Logger::instance().log( author, message, verbosity )
#else
#define LOG( author, message, verbosity ) 
#endif

class Logger
{
public:
	static Logger& instance()
	{
		static Logger logger;
		return logger;
	}
	virtual ~Logger();

	bool start();
	void stop();

	//void log( const LogMessage& message );
	void log( const char* author, const char* message, int verbosity );

	void setVerbosity( int verbosity );

private:
	Logger() : file( NULL ), verbosity( VERBOSITY_ERROR ) {}

	FILE* file;
	int verbosity;
};