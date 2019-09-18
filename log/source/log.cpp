#include "log.h"

CLog* CLog::m_instance;
CLog* CLog::GetInstance()
{
	if(NULL == m_instance)
	{
		m_instance = new CLog();
	}
	return m_instance;
}

void CLog::Init(const string& process_name, int info_flag, int debug_flag, int warn_flag, int error_flag)
{
	m_process_name.assign(process_name);
	m_info_flag = info_flag;
	m_debug_flag = debug_flag;
	m_warn_flag = warn_flag;
	m_error_flag = error_flag;
}

/*
int main()
{
	CLog::GetInstance()->Init("log_test", LOG_OPEN, LOG_OPEN, LOG_OPEN ,LOG_OPEN);

	PRINT_LOG_INFO("info_log: hello world");
	PRINT_LOG_DEBUG("debug_log: hello world");
	PRINT_LOG_WARN("warn_log: hello world");
	PRINT_LOG_ERROR("error_log: hello world");

	return 0;
}
*/
