#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/filesystem.h"

bool directoryExists(const std::string& dir)
{
	struct stat dirStat;
	if (stat(dir.c_str(), &dirStat) != 0)
		return false;

	return S_ISDIR(dirStat.st_mode);
}

bool createDirectory(const std::string& dir)
{
	return (mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
}
