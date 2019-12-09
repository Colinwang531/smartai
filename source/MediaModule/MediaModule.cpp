#include "MediaData/MediaData.h"
#include "MediaModule/MediaModule.h"

namespace framework
{
	namespace multimedia
	{
		MediaModule::MediaModule()
		{}

		MediaModule::~MediaModule()
		{}

		void MediaModule::setPostInputMediaDataCallback(PostInputMediaDataCallback callback /* = NULL */)
		{
			postInputMediaDataCallback = callback;
		}
	}//namespace multimedia
}//namespace framework
