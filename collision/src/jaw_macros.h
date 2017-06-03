#pragma once

#include <assert.h>
#include "log.h"

//assertions are meant for catching bugs at debug programming
//with assertions you don't care about cleaning up
//you just wanna abort and find the bug

//exceptions are meant for catching bugs at release programming
//with exceptions you do care about cleaning up
//you don't just wanna abort and leave a mess

#define JAW_ASSERT_DO(x) for (; !(x); assert(x)) //evaluates x twice by the way
#define JAW_ASSERT_MSG(x, msg) JAW_ASSERT_DO(x) { jaw::log_line(std::string("ASSERTION FAIL: ") + msg); }