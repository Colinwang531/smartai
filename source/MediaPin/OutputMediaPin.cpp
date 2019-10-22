#include "error.h"
#include "MediaPin/OutputMediaPin.h"

NS_BEGIN(pin, 1)

OutputMediaPin::OutputMediaPin() : MediaPin(MediaPinMode::MEDIA_PIN_OUTPUT)
{}

OutputMediaPin::~OutputMediaPin()
{}

int OutputMediaPin::connectPin(MediaPinRef inputPinRef)
{
	inputPinRefGroup.insert(inputPinRef);
	return ERR_OK;
}

int OutputMediaPin::inputData(MediaDataPtr dataPtr)
{
	int status{ MediaPin::inputData(dataPtr) };

	if (ERR_OK == status)
	{
		for (int i = 0; i != inputPinRefGroup.size(); ++i)
		{
			const MediaPinRef& pinRef{ inputPinRefGroup.at(i) };

			if (!pinRef.expired())
			{
				// WARNING : Please do not blocking.
				pinRef.lock()->inputData(dataPtr);
			}
		}
	}

	return status;
}

NS_END
