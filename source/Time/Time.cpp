#include "boost/date_time/posix_time/posix_time.hpp"
#include "Time/Time.h"

NS_BEGIN(time, 1)

Time::Time()
{}

Time::~Time()
{}

long long Time::expiredMilliseconds(const std::string startTime /* = "1970-01-01 00:00:00" */)
{
	boost::posix_time::ptime endTime{ boost::posix_time::microsec_clock::local_time() };
	boost::posix_time::ptime beginTime{ boost::posix_time::time_from_string(startTime) };
	boost::posix_time::time_duration timeDuration = endTime - beginTime;
	return timeDuration.total_milliseconds();
}

NS_END
