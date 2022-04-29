#include "stdafx.h"
#include "Endpoint.h"

Endpoint::Endpoint()
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	// �ּ� Ÿ���� �����Ѵ�.
	// �� sin������ �� �𸣰ڴ�.
	// socket... in??
	m_ipv4Endpoint.sin_family = AF_INET;
	// AF_INET - ipv4 ����Ѵٴ� ��
	// AF_INET - ipv6
	// AF_BTH - �������
}

Endpoint::Endpoint(const char* address, int port)
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	m_ipv4Endpoint.sin_family = AF_INET;

	// p to n ? printable to numeric
	// ���ڿ��� 2������ �ٲ��ִ� �Լ���� �Ѵ�. pszAddrString -> pAddrBuf
	// �Էµ� ip�ּҰ� ���� �ּ����� Ŭ���̾�Ʈ �ּ�����
	// ���⼭�� �𸣰� �ּҶ� ��Ʈ�� ����ü�� �����ϴ� �� ����.
	inet_pton(AF_INET, address, &m_ipv4Endpoint.sin_addr);

	// h to n ? s�� short�ΰ� ����. host to network
	// ����� �� ����Ʈ ������ ���� ����ϴ� �Լ���.
	// ��Ʈ��ũ������ Big-endian ����� ����ϱ� ������
	// ����� ȯ���� �����̵簣�� Big-endian���� �����ϱ� ���� ���Ǵ� �Լ�����Ѵ�.
	// ��.. �ٵ� �𸣰ڴ�. ���⼭ �ᱹ unsigned short ������
	// ���� little endian ����̴ϱ� little endian���� ����Ǵ°� �ƴѰ�..
	// ����Ʈ ������ ���ؼ���� ���� ������ �ؾ� �ϴ°� �ƴұ� �ϴ� ������ ���.
	// �ñ��ؼ� ������غô��� 4823�� �����ϱ� 55058�� ���Դ�.
	// little endian ������� ����Ǵ°��� �̿��ؼ� �׳� ���� �ٲ������ �� ����.
	m_ipv4Endpoint.sin_port = htons(static_cast<u_short>(port));
}

Endpoint::~Endpoint()
{

}

// �� ���ڱ� ���⿡ ����Ǿ� �ִ��� �� �𸣰ڴ�.
Endpoint Endpoint::Any;

std::string Endpoint::ToString()
{
	char addrString[1000];
	addrString[0] = 0;

	// n to p ? numeric to printable
	// �����ڿ��� ����� ��Ʈ��ũ �ּҸ� ���ڿ��� �ٲٴ°� ����.
	inet_ntop(AF_INET, &m_ipv4Endpoint.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];

	// ������ ���ڿ��� ġȯ�� ��Ʈ��ũ �ּҿ� �ݷ� + ��Ʈ��ȣ�� ���ۿ� �ִ´�.
	sprintf_s(finalString, 1000, "%s:%d", addrString, htons(m_ipv4Endpoint.sin_port));

	return finalString;
}