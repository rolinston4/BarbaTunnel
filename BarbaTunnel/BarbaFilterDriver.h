#pragma once
#include "General.h"
#include "BarbaSocket.h"
#include "BarbaClient\BarbaClientConfig.h"
#include "BarbaServer\BarbaServerConfig.h"

class BarbaFilterDriver
{
private:
	struct CapturePacket
	{
		//packet will be deleted at destructor
		CapturePacket(PacketHelper* packet, bool outbound)
		{
			Packet = packet;
			Outbound = outbound;
		}
		~CapturePacket()
		{
			delete Packet;
		}

		bool Outbound;
		PacketHelper* Packet;
	};

public:
	enum NetworkLayerEnum{
		NetworkLayerDateLink,
		NetworkLayerTransport,
	};

public:
	explicit BarbaFilterDriver(size_t maxCaptureMessageQueue=2000);
	virtual ~BarbaFilterDriver(void);
	//@return false when job finished and ready to dispose
	virtual void Dispose();
	virtual void Initialize()=0;
	virtual void Start();
	virtual void Stop();
	virtual LPCTSTR GetName()=0;
	virtual bool SendPacketToOutbound(PacketHelper* packet)=0;
	virtual bool SendPacketToInbound(PacketHelper* packet)=0;
	virtual NetworkLayerEnum GetNetworkLayer()=0;
	virtual DWORD GetMTUDecrement()=0;
	virtual void SetMTUDecrement(DWORD value)=0;
	virtual size_t GetMaxPacketLen();
	bool IsStarted() {return this->_IsStarted;}
	bool IsStopping() {return this->StopEvent.IsSet();}

protected:
	volatile bool _IsStarted;
	virtual void StartCaptureLoop()=0;
	SimpleEvent StopEvent;
	void AddPacket(PacketHelper* packet, bool outbound);
	void SendPacketWithSocket(PacketHelper* packet);
	void UpdateMTUDecrement();
	//@param filter object that should handle by subclass implementer
	virtual void AddFilter(void* filter, bool send, u_long srcIpStart, u_long srcIpEnd, u_long desIpStart, u_long desIpEnd, u_char protocol, u_short srcPortStart, u_short srcPortEnd, u_short desPortStart, u_short desPortEnd)=0;
	virtual void AddPacketFilter(void* filter);
	
private:
	void ProcessCapturedPackets();
	size_t MaxCaptureMessageQueue;
	SimpleSafeList<CapturePacket*> CapturePackets;
	SimpleEvent CaptureEvent;
	static unsigned int __stdcall CaptureThread(void* data);
	volatile HANDLE CaptureThreadHandle;

	//SocketHelper should not be shutdown or close after send packet, so we keep it in member
	BarbaSocket SocketHelper;

	//PacketFilter
	void AddClientFilters(void* filter, BarbaArray<BarbaClientConfig>* configs);
	void AddServerFilters(void* filter, BarbaArray<BarbaServerConfig>* configs);
};

