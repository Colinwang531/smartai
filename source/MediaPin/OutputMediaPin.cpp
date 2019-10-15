#include "error.h"
#include "MediaPin/OutputMediaPin.h"

NS_BEGIN(pin, 1)

OutputMediaPin::OutputMediaPin() : MediaPin()
{}

OutputMediaPin::~OutputMediaPin()
{}

int OutputMediaPin::connectInputPin(boost::shared_ptr<MediaPin> inputPinPtr)
{
	int status{ MediaPin::connectInputPin(inputPinPtr) };

	if (ERR_OK == status)
	{
		mtx.lock();
		inputPinPtrGroup.push_back(inputPinPtr);
		mtx.unlock();
	}

	return status;
}

int OutputMediaPin::inputData(MediaDataPtr dataPtr)
{
	int status{ MediaPin::inputData(dataPtr) };

	if (ERR_OK == status)
	{
		//Please do not blocking.
		mtx.lock();
		for (std::vector<boost::shared_ptr<MediaPin>>::iterator it = inputPinPtrGroup.begin();
			it != inputPinPtrGroup.end();
			++it)
		{
			(*it)->inputData(dataPtr);
		}
		mtx.unlock();
	}

	return status;
}

bool OutputMediaPin::isOutputPin() const
{
	return true;
}

NS_END
