#include "stdafx.h"
#include "Endpoint.h"

Endpoint::Endpoint()
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	// 주소 타입을 저장한다.
	// 왜 sin인지는 잘 모르겠다.
	// socket... in??
	m_ipv4Endpoint.sin_family = AF_INET;
	// AF_INET - ipv4 사용한다는 뜻
	// AF_INET - ipv6
	// AF_BTH - 블루투스
}

Endpoint::Endpoint(const char* address, int port)
{
	memset(&m_ipv4Endpoint, 0, sizeof(m_ipv4Endpoint));
	m_ipv4Endpoint.sin_family = AF_INET;

	// p to n ? printable to numeric
	// 문자열을 2진수로 바꿔주는 함수라고 한다. pszAddrString -> pAddrBuf
	// 입력된 ip주소가 서버 주소인지 클라이언트 주소인지
	// 여기서는 모르고 주소랑 포트만 구조체에 저장하는 것 같다.
	inet_pton(AF_INET, address, &m_ipv4Endpoint.sin_addr);

	// h to n ? s는 short인거 같다. host to network
	// 통신할 때 바이트 정렬을 위해 사용하는 함수다.
	// 네트워크에서는 Big-endian 방식을 사용하기 때문에
	// 사용자 환경이 무엇이든간에 Big-endian으로 전송하기 위해 사용되는 함수라고한다.
	// 음.. 근데 모르겠다. 여기서 결국 unsigned short 받으면
	// 내가 little endian 방식이니까 little endian으로 저장되는거 아닌가..
	// 바이트 정렬을 위해서라면 전송 직전에 해야 하는게 아닐까 하는 생각이 든다.
	// 궁금해서 디버그해봤더니 4823을 넣으니까 55058이 나왔다.
	// little endian 방식으로 저장되는것을 이용해서 그냥 값을 바꿔버리는 거 였다.
	m_ipv4Endpoint.sin_port = htons(static_cast<u_short>(port));
}

Endpoint::~Endpoint()
{

}

// 왜 갑자기 여기에 선언되어 있는지 잘 모르겠다.
Endpoint Endpoint::Any;

std::string Endpoint::ToString()
{
	char addrString[1000];
	addrString[0] = 0;

	// n to p ? numeric to printable
	// 생성자에서 저장된 네트워크 주소를 문자열로 바꾸는거 같다.
	inet_ntop(AF_INET, &m_ipv4Endpoint.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];

	// 위에서 문자열로 치환된 네트워크 주소와 콜론 + 포트번호를 버퍼에 넣는다.
	sprintf_s(finalString, 1000, "%s:%d", addrString, htons(m_ipv4Endpoint.sin_port));

	return finalString;
}