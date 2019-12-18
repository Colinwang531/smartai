// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Global definitions.
//

#ifndef PREDEF_H
#define PREDEF_H

#ifdef _WINDOWS
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif//CALLBACK
#ifndef NULL
#define NULL 0
#endif//NULL
#else
#define CALLBACK
#endif//_WINDOWS

static const unsigned short minPortNumber = 5000;
static const unsigned short maxPortNumber = 65535;
// WARNING : REQ <-> REP mode is not compatible for splitting package of data.
static const unsigned int maxTransportMTUBytes = 1024 * 1024/*65530*/;

#endif//PREDEF_H
