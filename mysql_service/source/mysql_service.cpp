#include "mysql_service.h"

const string m_sql_host = "localhost";
const string m_sql_tcp = "127.0.0.1";
const string m_sql_user = "fangyan";
const string m_sql_passwd = "fangyan";
const string m_sql_database = "mysql";
const string m_sql_unixsock = "/var/lib/mysql/mysql.sock";

CMysqlService::CMysqlService(){}

CMysqlService::~CMysqlService(){}

int CMysqlService::Connect(const ConnectionInfo& connection_info)
{
	if(NULL == (m_mysql = mysql_init(NULL)))
	{
		PRINT_LOG_ERROR("mysql init error");
		return -1;
	}

	m_connection_info = connection_info;
	MYSQL* sock = NULL;
	
	if(m_connection_info.unix_sock_.length() > 0)
	{
		sock = mysql_real_connect(m_mysql, m_connection_info.host_.c_str(), m_connection_info.user_.c_str(), 
								m_connection_info.passwd_.c_str(), m_connection_info.database_.c_str(), 
								m_connection_info.port_, m_connection_info.unix_sock_.c_str(), 0);
		
	}
	else
	{
		sock = mysql_real_connect(m_mysql, m_connection_info.host_.c_str(), m_connection_info.user_.c_str(), 
								m_connection_info.passwd_.c_str(), m_connection_info.database_.c_str(), 
								m_connection_info.port_, NULL, 0);
	}

	if(sock == NULL)
	{
		Close();
		PRINT_LOG_ERROR("mysql real connect error");
		return -1;
	}

	PRINT_LOG_DEBUG("mysql real connect success");
	return 0;
}

void CMysqlService::Close()
{
	if(m_mysql)
	{
		mysql_close(m_mysql);
		m_mysql = NULL;
	}
}

bool CMysqlService::IsAvailable()
{
	if(NULL == m_mysql)
	{
		PRINT_LOG_ERROR("mysql is unavailable");
		return false;
	}
	else if(0 != mysql_ping(m_mysql))
	{
		PRINT_LOG_ERROR("mysql_ping error : %s", MysqlErrorInfo());
		return false;
	}

	return true;
}

int CMysqlService::Execute(const char* p_sql, const int sql_len)
{
	if((NULL == m_mysql)||(NULL == p_sql)||(sql_len <= 0))
	{
		PRINT_LOG_ERROR("mysql connection error or sql error, sql_len: %d", sql_len);
		return -1;
	}
	
	if(0 != mysql_real_query(m_mysql, p_sql, (unsigned long)sql_len))
	{
		PRINT_LOG_ERROR("mysql_real_query error: %s", MysqlErrorInfo());
		if(MYSQL_ERR_DUP_ENTRY == mysql_errno(m_mysql))
		{
			PRINT_LOG_ERROR("mysql_real_query dup entry error");
		}
		return -1;
	}

	m_res = mysql_store_result(m_mysql);
	if(NULL == m_res)
	{
		if(0 == mysql_field_count(m_mysql))
		{
			return 0;
		}
		else
		{
			PRINT_LOG_ERROR("mysql store result error: %s", MysqlErrorInfo());
			return -1;
		}
	}

	return 0;
}


int CMysqlService::GetOneRowResult(vector<string>& row_vec)
{
	if(!m_res)
	{	
		PRINT_LOG_ERROR("m_res null error");
		return -1;
	}

	MYSQL_ROW row = mysql_fetch_row(m_res);
	if(!row)
	{
		//数据已取完
		return -2;
	}

	//获取结果集列数
	int result_fields_num = (int)mysql_num_fields(m_res);

	if(row_vec.size() > 0)
	{
		row_vec.clear();
	}

	string str_one_field("");
	for(int iloop = 0; iloop < result_fields_num; iloop++)
	{
		str_one_field.assign(row[iloop]);
		row_vec.push_back(str_one_field);
	}

	return 0;
}

const char* CMysqlService::MysqlErrorInfo()
{
	return NULL;
}


