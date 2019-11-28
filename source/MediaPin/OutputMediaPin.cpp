#include "error.h"
#include "MediaPin/OutputMediaPin.h"

namespace framework
{
	namespace multimedia
	{
		OutputMediaPin::OutputMediaPin() : MediaPin()
		{}

		OutputMediaPin::~OutputMediaPin()
		{
			inputPinGroup.clear();
		}

		int OutputMediaPin::connectPin(MediaPinRef pin)
		{
			inputPinGroup.insert(pin);
			return ERR_OK;
		}

		int OutputMediaPin::inputMediaData(MediaDataPtr mediaData)
		{
			int status{ mediaData ? ERR_OK : ERR_INVALID_PARAM };

			if (ERR_OK == status)
			{
				for (int i = 0; i != inputPinGroup.size(); ++i)
				{
					MediaPinRef nextInputPinRef{ inputPinGroup.at(i) };
					if (!nextInputPinRef.expired())
					{
						status = nextInputPinRef.lock()->inputMediaData(mediaData);
					}
				}
			}

			return status;
		}
	}//namespace multimedia
}//namespace framework
