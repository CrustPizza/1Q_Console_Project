#pragma once

// exception ���縸 �˰� �� �𸣴� Ŭ�����ε�
// ������Ʈ ������ �����ϵ簡 �ؾ��ҰŰ���.
// ���� �� �̰� ��ӹ޾Ƽ� ����ؾ��ϴ����� ���� ���ϰ� ����
class Exception : public std::exception
{
public:
	Exception(const std::string& text);
	~Exception();

	std::string m_text;

	const char* what() { return m_text.c_str(); }
};