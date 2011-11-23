#pragma once

u_short ntohs( u_short netshort );
u_short htons( u_short value );
DWORD htonl( DWORD value );
DWORD ntohl( DWORD value );

class PacketHelper
{
public:
	ether_header_ptr ethHeader;
	iphdr_ptr ipHeader;
	tcphdr_ptr tcpHeader;
	udphdr_ptr udpHeader;

	void SetSrcEthAddress(BYTE* address);
	void SetDesEthAddress(BYTE* address);
	void SetEthHeader(ether_header_ptr ethHeader);
	void SetEthPacket(ether_header_ptr ethHeader);

	//void SetTcpPacket(tcphdr_ptr tcpHeader);
	//void SetUdpPacket(udphdr_ptr udpHeader);

	PacketHelper(void* packet);
	
	//Create new packet
	PacketHelper(void* packet, u_char ipProtocol); 
	void Reinit();

	bool IsIp() { return ipHeader!=NULL;}
	size_t GetIpLen() { return ntohs( ipHeader->ip_len ); }
	DWORD GetDesIp() { return ipHeader->ip_dst.S_un.S_addr; }
	DWORD GetSrcIp() { return ipHeader->ip_src.S_un.S_addr; }
	void SetDesIp(DWORD ip) { ipHeader->ip_dst.S_un.S_addr = ip; }
	void SetSrcIp(DWORD ip) { ipHeader->ip_src.S_un.S_addr = ip; }
	u_short GetDesPort(); 
	u_short GetSrcPort();
	void SetDesPort(u_short port);
	void SetSrcPort(u_short port);
	bool SetIpPacket(iphdr_ptr ipHeader);
	BYTE* GetIpExtraHeader(); 
	size_t GetIpExtraHeaderLen(); 
	void RecalculateChecksum();
	bool IsValidChecksum();

	
	bool IsTcp() { return tcpHeader!=NULL;}
	size_t GetTcpPayloadLen() { return GetIpLen() - ipHeader->ip_hl*4 - tcpHeader->th_off*4; }
	BYTE* GetTcpPayload() {  return (BYTE*)tcpHeader + tcpHeader->th_off*4; }
	void SetTcpPayload(BYTE* payload, size_t len);
	BYTE* GetTcpExtraHeader(); 
	size_t GetTcpExtraHeaderLen(); 

	bool IsUdp() { return udpHeader!=NULL;}
	size_t GetUdpPayloadLen() { return ntohs(udpHeader->length) - sizeof(udphdr); }
	BYTE* GetUdpPayload() { return (BYTE*)udpHeader + sizeof(udphdr); }
	void SetUdpPayload(BYTE* payload, size_t len);

	BYTE* GetPacket() {return (BYTE*)ethHeader;}
	size_t GetPacketLen() { return sizeof (ether_header) + GetIpLen(); }

	static u_short CheckSum(u_short *buffer, int size);
	static void RecalculateICMPChecksum(iphdr_ptr pIpHeader);
	static void RecalculateUDPChecksum(iphdr_ptr pIpHeader);
	static void RecalculateTCPChecksum(iphdr_ptr pIpHeader);
	static void RecalculateIPChecksum(iphdr_ptr pIpHeader, bool calculateProtoCheckSum=true);
	static bool IsValidIPChecksum(iphdr_ptr pIpHeader);
	static DWORD ConvertStringIp(LPCTSTR pszIp);
	static void ConvertIpToString(DWORD ip, TCHAR* buffer, rsize_t bufferCount);
	static BYTE ConvertStringProtocol(LPCTSTR protocol);
	static LPCTSTR ConvertProtocolToString(BYTE protocol);
};
