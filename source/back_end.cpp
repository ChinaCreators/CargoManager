#include "back_end.h"

const std::string AccountManager::sm_FileName="account.txt";

AccountManager::AccountManager()
{
	LoadAccount();
}

AccountManager::~AccountManager()
{
	SaveAccount();
}

void AccountManager::LoadAccount()
{
	m_File.open(sm_FileName,std::ios::in);
	size_t size;
	m_File>>size;
	std::string name;
	size_t hpwd;
	for(int i=0;i<size;i++)
	{
		m_File>>name>>hpwd;
		auto iter=m_Content.find(name);
		if(iter==m_Content.end())
			m_Content.insert(std::make_pair(name,Account{name,hpwd}));
	}
	m_File.close();
}

void AccountManager::SaveAccount()
{
	m_File.open(sm_FileName,std::ios::out);
	m_File<<m_Content.size();
	for(auto& i:m_Content)
	{
		m_File<<i.second.m_Name<<" "<<i.second.m_HashedPassword<<std::endl;
	}
	m_File.close();
}

void AccountManager::SignUp(const std::string& name,std::string& pwd)
{
	auto iter=m_Content.find(name);
	if(iter!=m_Content.end())
		m_ErrorHappenedSignal.emit(L"重复的账号名");
	else
	{
		m_Content.insert(std::make_pair(name,Account{name,std::hash<std::string>{}(pwd)}));
		m_SignUpSignal.emit(Wt::WString(name));
	}
}

void AccountManager::SignIn(const std::string& name,std::string& pwd)
{
	auto iter=m_Content.find(name);
	if(iter==m_Content.end())
		m_ErrorHappenedSignal.emit(L"没有该账号");
	else
	{
		if(std::hash<std::string>{}(pwd)==iter->second.m_HashedPassword)
		{
			m_SignUpSignal.emit(Wt::WString(name));
		}
		else
		{
			m_ErrorHappenedSignal.emit(L"密码错误");
		}
	}
}

AccountManager& AccountManager::GetInstance()
{
	static AccountManager g;
	return g;
}