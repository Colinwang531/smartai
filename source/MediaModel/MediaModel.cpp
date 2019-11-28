#include "MediaData/MediaData.h"
#include "MediaModel/MediaModel.h"

namespace framework
{
	namespace multimedia
	{
		MediaModel::MediaModel()
		{}

		MediaModel::~MediaModel()
		{}

		void MediaModel::setPostInputMediaDataCallback(PostInputMediaDataCallback callback /* = NULL */)
		{
			postInputMediaDataCallback = callback;
		}
	}//namespace multimedia
}//namespace framework
