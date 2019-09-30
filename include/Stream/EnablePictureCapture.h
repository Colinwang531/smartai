//
//		Copyright :					@2019, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2019-09-30
//		Description :				ʵʱ��ץͼ�ӿ�
//
//		History:						Author										Date													Description
//											������										2019-09-30									����
//

#ifndef ENABLE_PICTURE_CAPTURE_H
#define ENABLE_PICTURE_CAPTURE_H

#include "predef.h"

NS_BEGIN(stream, 1)

class EnablePictureCapture
{
public:
	EnablePictureCapture(void) {}
	virtual ~EnablePictureCapture(void) {}

protected:
	virtual unsigned long long captureJPEGPicture(
		const char* data = NULL, const unsigned long long dataBytes = 0) = 0;
};//class EnablePictureCapture

NS_END

#endif//ENABLE_PICTURE_CAPTURE_H
