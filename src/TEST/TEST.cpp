#include <stdafx.h>
#include <Winsock2.h>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct Weather_Info     //天气信息结构体
{
  string city;        //城市
  string weather;     //天气
  string temp;        //气温
  string wind;        //风向
  string sDetail;     //详细信息
};

bool InitSocket();
string GetFile();
wchar_t* Utf8ToUnicode(const char* buf);
string UnicodeToAnsi(const wchar_t* buf);

int main()
{
  InitSocket(); // 初始化Socket
  GetFile();

  return 0;
}

bool InitSocket()
{
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  wVersionRequested = MAKEWORD( 2, 2 );
  err = WSAStartup( wVersionRequested, &wsaData );
  
  if( err != 0 )
  {
    return false;
  }
  
  if (LOBYTE( wsaData.wVersion )!=2 || HIBYTE( wsaData.wVersion )!=2)
  {
    WSACleanup();
    return false;
  }

  return true;
}

string GetFile()
{
  struct hostent *pURL;
  struct sockaddr_in addr; 
  SOCKET sockfd; 
  string header;
  
  sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
  
  pURL = gethostbyname("www.webxml.com.cn");
  if(NULL == pURL)
    return "";
  
  addr.sin_family = AF_INET; 
  addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr); 
  addr.sin_port = htons(80);
  connect(sockfd,(SOCKADDR *)&addr,sizeof(addr));
  
  string sCode("北京");
  
  header="GET ";
  header.append("/WebServices/WeatherWebService.asmx/getWeatherbyCityName?theCityName=");
  header.append(sCode);
  header.append(" HTTP/1.1\r\n");
  header.append("Host: ");
  header.append("www.webxml.com.cn\r\n");
  header.append("Connection: keep-alive\r\n");
  header.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
  header.append("User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36\r\n");
  header.append("Accept-Encoding: gzip,deflate,sdch\r\n");
  header.append("Accept-Language: zh-CN,zh;q=0.8\r\n\r\n");
  
  char text[1024];
  send(sockfd, header.c_str(), header.size(), 0);
  
  int clen = 0;
  wchar_t *tmp;
  wstring s;
  
  while (recv(sockfd, text, 1024, 0) != 0) 
  { 
    tmp = ::Utf8ToUnicode(text);
    s.append(tmp);		
  } 
  
  closesocket(sockfd); 
  WSACleanup(); 
  
  return UnicodeToAnsi(s.c_str());
}

wchar_t* Utf8ToUnicode(const char* buf)
{
  int len = ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
  
  wchar_t *tmp = new wchar_t[len*2+1];
  
  ::MultiByteToWideChar(CP_UTF8, 0, buf, -1, tmp, len);
  
  return tmp;
}
string UnicodeToAnsi(const wchar_t* buf)
{
  int len = ::WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
  if (len == 0) return "";
  
  vector<char> utf8(len);
  ::WideCharToMultiByte(CP_ACP, 0, buf, -1, &utf8[0], len, NULL, NULL);
  
  return &utf8[0];
}