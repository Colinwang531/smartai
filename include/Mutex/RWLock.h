// Copyright (c) 2019, *** Inc.
// All rights reserved.
//
// Author : Íõ¿ÆÍþ
// E-mail : wangkw531@icloud.com
//
// Read and write locker definitions.
//

#ifndef RW_LOCK_H
#define RW_LOCK_H

#include "boost/thread/locks.hpp"
#include "boost/thread/shared_mutex.hpp"

using SharedMutex = boost::shared_mutex;
using WriteLock = boost::unique_lock<SharedMutex>;
using ReadLock = boost::shared_lock<SharedMutex>;

#endif//RW_LOCK_H

