#include <CDebugLogfile.h>

triebWerk::CDebugLogfile& triebWerk::CDebugLogfile::Instance()
{
	static CDebugLogfile t;
	return t;
}

triebWerk::CDebugLogfile::CDebugLogfile() :
	m_pFile(nullptr),
	m_ErrorOccurred(false)
{
}

triebWerk::CDebugLogfile::~CDebugLogfile()
{
	LogText("-----------------------------------------------------------<br>");
	LogText("This Logfile was automatically generated by triebWerk: <br>");
	LogText("<a href='https://github.com/Chylix/triebWerk'>https://github.com/Chylix/triebWerk</a>");
	LogText("<br>End of Logfile</font></body></html>");

	fclose(this->m_pFile);

	if (m_ErrorOccurred)
	{
		ShellExecute(NULL, "open", "twLog.html", NULL, NULL, SW_SHOWNORMAL);
	}
}

void triebWerk::CDebugLogfile::CreateLogfile(const char * a_LogfileName)
{
	SYSTEMTIME  time;
	GetSystemTime(&time);

	std::string timeS = "Logfile created: " + std::to_string(time.wHour+2);
	timeS += ":" + std::to_string(time.wMinute);
	timeS += " | " + std::to_string(time.wDay);
	timeS += "/" + std::to_string(time.wMonth);
	timeS += "/" + std::to_string(time.wYear);
	timeS += "<br>";

	char filename[MAX_PATH];
	DWORD size = GetModuleFileNameA(NULL, filename, MAX_PATH);

	fopen_s(&m_pFile, a_LogfileName, "w");
	LogText("<html><head><title>Logfile</title></head>");
	LogText("<body><font face = 'courier new'>");
	WriteTopic("triebWerk - Logfile", 3);
	LogText("Logfile from: ");
	LogText(filename);
	LogText("<br>");
	LogText("BUILD: DEBUG<br>");
	LogText(timeS.c_str());
	LogText("-----------------------------------------------------------<br>");



	fclose(this->m_pFile);
	fopen_s(&m_pFile, a_LogfileName, "a");
}

void triebWerk::CDebugLogfile::WriteTopic(const char * a_Topic, unsigned int a_Size)
{
	LogText("<table cellspacing='0' cellpadding='0' width='100%%' ");
	LogText("bgcolor='#DFDFE5'>\n<tr>\n<td>\n<font face = 'arial' ");
	LogfText("size='+%i'>\n", a_Size);
	LogText(a_Topic);
	LogText("</font>\n</td>\n</tr>\n</table>\n<br>");
	fflush(this->m_pFile);
}

void triebWerk::CDebugLogfile::LogText(const char * a_Text)
{
	fprintf(this->m_pFile, a_Text);
	fflush(this->m_pFile);
}

void triebWerk::CDebugLogfile::LogText(EColor a_Color, bool a_List, const char * a_Text)
{
	if (a_List)
		LogText("<li>");

	switch (a_Color)
	{
	case EColor::Black:
		LogText("<font color=black>"); break;
	case EColor::Red:
		LogText("<font color=red>"); 
		m_ErrorOccurred = true;
		break;
	case EColor::Green:
		LogText("<font color=green>"); break;
	case EColor::Blue:
		LogText("<font color=blue>"); 
		break;
	case EColor::Yellow:
		LogText("<font color=#bfa904>");
		m_ErrorOccurred = true;
		break;
	};

	LogText(a_Text);
	LogText("</font>");

	if (!a_List)
		LogText("<br>");
	else
		LogText("</li>");
}

void triebWerk::CDebugLogfile::LogText(EColor a_Color, const char * a_Text)
{
	LogText(a_Color, false, a_Text);
}

void triebWerk::CDebugLogfile::LogfText(const char * a_Text, ...)
{
	TCHAR buffer[MAX_BUFFER];
	va_list pArgList;
	va_start(pArgList, a_Text);
	vsprintf_s(buffer, a_Text, pArgList);
	va_end(pArgList);

	LogText(buffer);
}

void triebWerk::CDebugLogfile::LogfText(EColor a_Color, const char * a_Text, ...)
{
	TCHAR buffer[MAX_BUFFER];
	va_list pArgList;
	va_start(pArgList, a_Text);
	vsprintf_s(buffer, a_Text, pArgList);
	va_end(pArgList);

	LogText(a_Color, buffer);
}

void triebWerk::CDebugLogfile::LogfText(EColor a_Color, bool a_List, const char * a_Text, ...)
{
	TCHAR buffer[MAX_BUFFER];
	va_list pArgList;
	va_start(pArgList, a_Text);
	vsprintf_s(buffer, a_Text, pArgList);
	va_end(pArgList);

	LogText(a_Color, a_List, buffer);
}

void triebWerk::CDebugLogfile::FunctionResult(const char * a_pFunctionName, bool a_Result)
{
	if (L_OK == a_Result)
	{
		LogText("<table width='100%%' cellSpacing='1' cellPadding ='5'");
		LogText(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
		LogfText("'#FFFFF' width='35%%'>%s</TD>", a_pFunctionName);
		LogText("<td bgcolor='#FFFFF' width='30%%'><font color = ");
		LogText("'green'>OK</FONT></TD><td bgcolor='#FFFFF' ");
		LogText("width='35%%'>-/-</TD></tr></table>");
	}
	else
	{
		LogText("<table width='100%%' cellSpacing='1' cellPadding ='5'");
		LogText(" border='0' bgcolor='#C0C0C0'><tr><td bgcolor=");
		LogfText("'#FFFFF' width='35%%'>%s</TD>", a_pFunctionName);
		LogText("<td bgcolor='#FFFFF' width='30%%'><font color = ");
		LogText("'red'>ERROR</FONT></TD><td bgcolor='#FFFFF' ");
		LogText("width='35%%'>-/-</TD></tr></table>");
	}
}
