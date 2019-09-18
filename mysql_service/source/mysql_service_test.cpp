#include "mysql_service.h"

int main()
{
	CLog::GetInstance()->Init("mysql_service_test", LOG_OPEN, LOG_OPEN, LOG_OPEN ,LOG_OPEN);
	
	PRINT_LOG_DEBUG("test start");

	ConnectionInfo connection_info(3306, "localhost", "root", "fangyan", "mysql", "");
	CMysqlService* mysql_service = new CMysqlService();
	if(NULL == mysql_service)
	{
		PRINT_LOG_ERROR("new mysql_service error");
		return -1;
	}
	
	int ret = mysql_service->Connect(connection_info);
	if(ret < 0)
	{
		PRINT_LOG_ERROR("mysql connect error");
		return -1;
	}

	PRINT_LOG_DEBUG("connect mysql success");
	
	string show_tables_sql("show tables");
	ret = mysql_service->Execute(show_tables_sql.c_str(), show_tables_sql.size());
	if(ret < 0)
	{
		PRINT_LOG_ERROR("mysql service execute error");
		return -1;
	}

	char* p_buf = new char[1024];
	vector<string> row_vec;
	row_vec.clear();
	ret = 0;
	while(0 == ret)
	{
		ret = mysql_service->GetOneRowResult(row_vec);
		if(ret == -1)
		{
			PRINT_LOG_ERROR("get one row result error");
		}
		else if(ret == -2)
		{
			PRINT_LOG_WARN("result null");
		}

		int len = 0;
		memset(p_buf, 0, 1024);
		for(int iloop = 0; iloop < row_vec.size(); iloop++)
		{
			len += snprintf(p_buf + len, 1024, "|%s ", row_vec[iloop].c_str());
		}
		PRINT_LOG_INFO("%s", p_buf);
	}

	return 0;
}


