#include "Logger.h"

Logger::~Logger()
{
	stop();
}

bool Logger::start()
{
	file = fopen( "./log.txt", "w" );

	if( file )
	{
		char buffer[128] = {};

		time( &startTime );
		struct tm* timeinfo = localtime( &startTime );

		strftime( buffer, 128, "%Y-%m-%d %I:%M:%S", timeinfo );

		fprintf( file, "Initializing logger at %s.\n\n", buffer );
	}

	return ( file != NULL );
}

void Logger::stop()
{
	if( file )
	{
		time_t endTime;
		time( &endTime );

		double timedif = difftime( endTime, startTime );

		char buffer[128] = {};
		struct tm* timeinfo = localtime( &endTime );

		strftime( buffer, 128, "%Y-%m-%d %I:%M:%S", timeinfo );

		fprintf( file, "\nExecution terminated at %s.\n", buffer );
		fprintf( file, "Execution time: %f.\n", timedif );

		fclose( file );
		file = NULL;
	}
}

void Logger::log( int verb, const char* author, const char* message )
{
	assert( file );

	fprintf( file, "%s: %s\n", author, message );

	if( verbosity <= verb )
	{
		printf( "%s: %s\n", author, message );
	}
}

void Logger::setVerbosity( int v )
{
	verbosity = v;
}

char* Logger::getTemporaryBuffer()
{
	return tempBuffer;
}