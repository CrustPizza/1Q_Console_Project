#pragma once

// exception 존재만 알고 잘 모르는 클래스인데
// 프로젝트 끝나고 공부하든가 해야할거같다.
// 지금 왜 이걸 상속받아서 사용해야하는지도 이해 못하고 있음
class Exception : public std::exception
{
public:
	Exception(const std::string& text);
	~Exception();

	std::string m_text;

	const char* what() { return m_text.c_str(); }
};