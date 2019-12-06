// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// The foundational components of media graph includes image formatter, 
// video renderer, sound player and data callback.
//

#ifndef FOUNDATIONAL_MEDIA_GRAPH_H
#define FOUNDATIONAL_MEDIA_GRAPH_H

#include "MediaGraph.h"

namespace framework
{
	namespace multimedia
	{
		class FoundationalMediaGraph : public MediaGraph
		{
		public:
			FoundationalMediaGraph(void);
			virtual ~FoundationalMediaGraph(void);

		protected:
			int createNewGraph(void) override;
			virtual int createNewImageFormatterFilter(void);
			virtual int createNewVideoRendererFilter(void);
			virtual int createNewSoundPlayerFilter(void);
			virtual int createNewDataCaptureFilter(void);

		private:
			void buildMediaGraph(void);
		};//class FoundationalMediaGraph
	}//namespace multimedia
}//namespace framework

#endif//FOUNDATIONAL_MEDIA_GRAPH_H
