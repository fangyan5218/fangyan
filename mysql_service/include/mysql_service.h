#include "common.h"
#include "log.h"
#include "mysql.h"

enum MysqlErrorNo
{
	MYSQL_ERR_DUP_ENTRY = 1602,
};

struct ConnectionInfo
{
	int     port_;
	string  host_;
	string  user_;
	string  passwd_;
	string  database_;
	string  unix_sock_;

	ConnectionInfo(){};
	ConnectionInfo(const int port, const string host, const string user, const string passwd, const string database, const string unix_sock):
		port_(port),
		host_(host),
		user_(user),
		passwd_(passwd),
		database_(database),
		unix_sock_(unix_sock){}

};

class CMysqlService
{
	public:
		static const string m_sql_host;
		static const string m_sql_tcp;
		static const string m_sql_user;
		static const string m_sql_passwd;
		static const string m_sql_unixsock;

	public:
		CMysqlService();
		virtual ~CMysqlService();

		int Connect(const ConnectionInfo& connection_info);
		void Close();
		bool IsAvailable();
		int Execute(const char* p_sql, const int sql_len);
		int GetOneRowResult(vector<string>& m_row_vec);

		const char* MysqlErrorInfo();

	private:
		ConnectionInfo m_connection_info;
		MYSQL* m_mysql;
		MYSQL_RES* m_res;
		//vector<string> m_row_vec;
		char* m_err_buf;

		/*
		//FieldValue* m_result_row_value;
		char* m_result_row_buf;
		char* m_large_field_buf;
		int result_row_buf_len;
		int large_field_buf_len;
		*/
};

