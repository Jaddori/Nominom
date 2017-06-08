#include "Logger.h"

Logger::~Logger()
{
	stop();
}

bool Logger::start()
{
	file = fopen( "./log.txt", "w" );
	return ( file != NULL );
}

void Logger::stop()
{
	if( file )
	{
		fclose( file );
	}
}

/*void Logger::log( const LogMessage& message )
{
	fprintf( file, "%s: %s\n", message.author, message.message );

	if( verbosity <= message.verbosity )
	{
		printf( "%s: %s\n", message.author, message.message );
	}
}*/

void Logger::log( const char* author, const char* message, int v )
{
	fprintf( file, "%s: %s\n", author, message );

	if( verbosity <= v )
	{
		printf( "%s: %s\n", author, message );
	}
}

void Logger::setVerbosity( int v )
{
	verbosity = v;
}