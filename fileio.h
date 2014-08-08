#ifndef FILEIO_H_
#define FILEIO_H_

#include <cstdio>
#include <ctime>

#define FILE_BUFFER_SIZE 256
#define FILENAME "savefile.txt"

void writeToFile (char message[FILE_BUFFER_SIZE]);

#endif /* FILEIO_H_ */
