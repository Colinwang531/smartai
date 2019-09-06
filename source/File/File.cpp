#include "error.h"
#include "File/File.h"

NS_BEGIN(file, 1)

File::File()
{}

File::~File()
{
	this->close();
}

FILE* File::open(const std::string path)
{
	return path.empty() ? NULL : openFile(path);
}

void File::close(FILE* fd /* = nullptr */)
{
	if (fd)
	{
		closeFile(fd);
	}
}

Int32 File::readingIntValueByKey(const std::string key)
{
	return ERR_NOT_IMPLEMENT;
}

std::string File::readingStringValueByKey(const std::string key)
{
	return std::string();
}

Int32 File::writtingWithLevel(const Int32 level, const std::string format, ...)
{
	return ERR_NOT_IMPLEMENT;
}

FILE* File::openFile(const std::string path)
{
	FILE* fd{ NULL };
	fopen_s(&fd, path.c_str(), "rw+");

	return fd;
}

void File::closeFile(FILE* fd /* = nullptr */)
{
	fclose(fd);
}

NS_END