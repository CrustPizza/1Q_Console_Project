#pragma once
#include <Windows.h>

class Socket;
class IocpEvents;

class Iocp
{
public:
	static const int MaxEventCount = 1000;
	
	Iocp(int threadCount);
	~Iocp();

	void Add(Socket& socket, void* userPtr);

	HANDLE m_hIocp;
	int m_threadCount;
	void Wait(IocpEvents& output, int timeoutMs);
};

class IocpEvents
{
public:
	OVERLAPPED_ENTRY m_events[Iocp::MaxEventCount];
	int m_eventCount;
};