#include "log.h"

int main()
{
	CLog::GetInstance()->Init("log_test", LOG_OPEN, LOG_OPEN, LOG_OPEN ,LOG_OPEN);

	PRINT_LOG_INFO("info_log: hello world");
	PRINT_LOG_DEBUG("debug_log: hello world");
	PRINT_LOG_WARN("warn_log: hello world");
	PRINT_LOG_ERROR("error_log: hello world");

	return 0;
}

