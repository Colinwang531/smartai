#include "error.h"
#include "DataStruct/FIFOList.h"

NS_BEGIN(datastruct, 1)

FIFOList::FIFOList(const int capacity /* = 5 */) : elementCapacity{ capacity }
{}

FIFOList::~FIFOList()
{}

int FIFOList::pushBack(void* element /* = NULL */)
{
	int status{ ERR_INVALID_PARAM };

	if (element)
	{
		rwLock.lock();
		if (elementCapacity > elements.size())
		{
			elements.push_back(element);
			status = ERR_OK;
		}
		else
		{
			status = ERR_OUT_RANGE;
		}
		rwLock.unlock();
	}

	return status;
}

void FIFOList::popFront()
{
	rwLock.lock();
	if (0 < elements.size())
	{
		elements.pop_front();
	}
	rwLock.unlock();
}

void* FIFOList::getFront()
{
	void* elem = NULL;

	rwLock.lock();
	if (0 < elements.size())
	{
		elem = elements.front();
	}
	rwLock.unlock();

	return elem;
}

bool FIFOList::isFull()
{
	bool status{ true };

	rwLock.lock();
	if (elementCapacity > elements.size())
	{
		status = false;
	}
	rwLock.unlock();

	return status;
}

NS_END
