#pragma once

#include "BaseIncludes.h"
#include "LogMessage.h"

#define LOGGER_TEMP_BUFFER_MAX_LENGTH 256

#ifdef _DEBUG
/*#define LOG( author, message, verbosity ) Logger::instance().log( author, message, verbosity )
#define LOGBUF Logger::instance().getTemporaryBuffer()
#define LOGFMT( author, message, verbosity, ... ) snprintf( LOGBUF, LOGGER_TEMP_BUFFER_MAX_LENGTH, message, __VA_ARGS__ ); LOG( author, LOGBUF, verbosity )*/
#define LOG( verbosity, author, message, ... ) snprintf( Logger::instance().getTemporaryBuffer(), LOGGER_TEMP_BUFFER_MAX_LENGTH, message, __VA_ARGS__ ); Logger::instance().log( verbosity, author, Logger::instance().getTemporaryBuffer() )
#else
#define LOG( author, message, verbosity ) 
#define LOGBUF 
#define LOGFMT 
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
	void log( int verbosity, const char* author, const char* message );

	void setVerbosity( int verbosity );

	char* getTemporaryBuffer();

private:
	Logger() : file( NULL ), verbosity( VERBOSITY_ERROR ) {}

	FILE* file;
	int verbosity;
	char tempBuffer[LOGGER_TEMP_BUFFER_MAX_LENGTH];

	time_t startTime;
};