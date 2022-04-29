#include "stdafx.h"
#include "Iocp.h"
#include "Socket.h"
#include "Exception/Exception.h"

Iocp::Iocp(int threadCount)
{
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, threadCount);
	m_threadCount = threadCount;
}

Iocp::~Iocp()
{
	CloseHandle(m_hIocp);
}

void Iocp::Add(Socket& socket, void* userPtr)
{
	if (!CreateIoCompletionPort((HANDLE)socket.m_fd, m_hIocp, (ULONG_PTR)userPtr, m_threadCount))
	{
		throw Exception("IOCP add failed!");
	}
}

void Iocp::Wait(IocpEvents& output, int timeoutMs)
{
	BOOL r = GetQueuedCompletionStatusEx(m_hIocp, output.m_events, MaxEventCount, (ULONG*)&output.m_eventCount, timeoutMs, FALSE);
	if (r == false)
	{
		output.m_eventCount = 0;
	}
}