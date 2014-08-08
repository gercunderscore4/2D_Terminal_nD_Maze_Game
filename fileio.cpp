#include "fileio.h"

using namespace std;

void writeToFile (char message[FILE_BUFFER_SIZE])
{
	message[FILE_BUFFER_SIZE-1] = '\0';
	// open file
	FILE* ofp = fopen(FILENAME, "a");
	if (ofp == NULL) {
		return;
	}
	// get time
	time_t rawTime;
	time(&rawTime);
	struct tm* localTime = localtime(&rawTime);
	// print to file
	fprintf(ofp, "%04i-%02i-%02i | %02i:%02i:%02i %s\n", localTime->tm_year+1900, localTime->tm_mon+1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec, message);
	// close file
	fclose(ofp);
	ofp = NULL;
}
