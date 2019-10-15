#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
#include <Wt/WObject.h>
#include <Wt/WSignal.h>
#include <Wt/WString.h>

struct Account
{
	std::string m_Name;
	size_t m_HashedPassword;
};

class AccountServer:public Wt::WObject
{
public:
	void SignUp(const std::string& name,std::string& pwd);
	void SignIn(const std::string& name,std::string& pwd);

	Wt::Signal<Wt::WString> m_ErrorHappenedSignal;
	Wt::Signal<Wt::WString> m_SignUpSignal;
};

class AccountManager:public Wt::WObject
{
public:
	friend AccountServer;

	~AccountManager();

	void LoadAccount();
	void SaveAccount();

	static AccountManager& GetInstance();
private:
	AccountManager();

	static const std::string sm_FileName;

	std::fstream m_File;
	std::map<std::string,Account> m_Content;

};