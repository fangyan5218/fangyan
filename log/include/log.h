#ifndef _LOG_H_
#define _LOG_H
#include "common.h"

#define NONE                 "\e[0m" 
#define RED                  "\e[0;31m" 
#define GREEN                "\e[0;32m" 
#define YELLOW               "\e[1;33m"
#define PURPLE               "\e[0;35m"

#define PRINT_NONE			 printf(NONE)
#define PRINT_RED            printf(RED)
#define PRINT_GREEN			 printf(GREEN)
#define PRINT_YELLOW		 printf(YELLOW)
#define PRINT_PURPLE		 printf(PURPLE)

#define CLOG_INSTANCE        (CLog::GetInstance())
#define PROCESS_NAME         (CLOG_INSTANCE->GetProcessName()).c_str()
#define LOG_LOCK             (pthread_mutex_lock(CLOG_INSTANCE->GetLogMutex()))
#define LOG_UNLOCK           (pthread_mutex_unlock(CLOG_INSTANCE->GetLogMutex()))

#define PRINT_LOG(format, args...)       	printf("[%s] [0x%lx] %s(%d)<%s ", PROCESS_NAME, pthread_self(), __FILE__, __LINE__, __FUNCTION__);\
										 	printf(format, ##args);printf(">\n")
#define PRINT_LOG_INFO(format, args...)  	if(CLOG_INSTANCE->GetInfoFlag())\
												do {LOG_LOCK;PRINT_NONE;PRINT_LOG(format, ##args);PRINT_NONE;LOG_UNLOCK;}while(0)
#define PRINT_LOG_DEBUG(format, args...)    if(CLOG_INSTANCE->GetDebugFlag())\
												do {LOG_LOCK;PRINT_GREEN;PRINT_LOG(format, ##args);PRINT_NONE;LOG_UNLOCK;}while(0) 
#define PRINT_LOG_WARN(format, args...)		if(CLOG_INSTANCE->GetDebugFlag())\
												do {LOG_LOCK;PRINT_YELLOW;PRINT_LOG(format, ##args);PRINT_NONE;LOG_UNLOCK;}while(0) 
#define PRINT_LOG_ERROR(format, args...)	if(CLOG_INSTANCE->GetDebugFlag())\
												do {LOG_LOCK;PRINT_RED;PRINT_LOG(format, ##args);PRINT_NONE;LOG_UNLOCK;}while(0) 
enum ELogCfgFlag
{
	LOG_CLOSE,
	LOG_OPEN,
};

class CLog
{
	public:
		static CLog* GetInstance();

		void Init(const string& process_name, int info_flag, int debug_flag, int warn_flag, int error_flag);
		
		inline void SetInfoFlag(int value)
		{
			m_info_flag = value;
		}
		
		inline void SetDebugFlag(int value)
		{
			m_debug_flag = value;
		}

		inline void SetWarnFlag(int value)
		{
			m_warn_flag = value;
		}
		
		inline void SetErrorFlag(int value)
		{
			m_warn_flag = value;
		}

		inline int GetInfoFlag()
		{
			return m_info_flag;
		}
		
		inline int GetDebugFlag()
		{
			return m_debug_flag;
		}
		
		inline int GetWarnFlag()
		{
			return m_warn_flag;
		}
		
		inline int GetErrorFlag()
		{
			return m_error_flag;
		}

		inline pthread_mutex_t* GetLogMutex()
		{
			return (&m_mutex);
		}

		inline string GetProcessName()
		{
			return m_process_name;
		}
		
	private:
		CLog():m_info_flag(1){};

		static CLog*  m_instance;

		string m_process_name;
		int m_info_flag;
		int m_debug_flag;
		int m_warn_flag;
		int m_error_flag;
		pthread_mutex_t m_mutex;
};

#endif

