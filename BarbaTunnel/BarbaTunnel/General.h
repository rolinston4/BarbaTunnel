#pragma once

#define BARBA_MAX_CONFIGS 100
#define BARBA_MAX_CONFIGITEMS 100
#define BARBA_MAX_CONNECTIONS 1000
#define BARBA_MAX_PORTITEM 100
#define BARBA_MAX_KEYLEN 1500
#define BARBA_MAX_VIRTUALIP 0xFFFF
#define BARBA_CURRENT_VERSION 1
#define BARBA_CONNECTION_TIMEOUT (3*3600*1000)

//BarbaModeEnum
enum BarbaModeEnum
{
	BarbaModeNone,
	BarbaModeUdpTunnel,
	BarbaModeUdpRedirect,
	BarbaModeTcpTunnel,
	BarbaModeTcpRedirect,
};

//PortRange
struct PortRange
{
	u_short StartPort;
	u_short EndPort;
};

//IpRange
struct IpRange
{
	DWORD StartIp;
	DWORD EndIp;
};


//ProtocolPort
struct ProtocolPort
{
	BYTE Protocol;
	u_short Port; 
};

//BarbaHeader
struct BarbaHeader
{
	GUID Signature;
	BYTE Version;
};


BarbaModeEnum BarbaMode_FromString(LPCTSTR mode);
LPCTSTR BarbaMode_ToString(BarbaModeEnum mode);
u_char BarbaMode_GetProtocol(BarbaModeEnum mode);
GUID* GetBarbaSign();