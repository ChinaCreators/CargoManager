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

	static AccountManager& GetInstance();
private:
	AccountManager();

	void LoadAccount();
	void SaveAccount();

	static const std::string sm_FileName;

	std::fstream m_File;
	std::map<std::string,Account> m_Content;

};

struct Cargo
{
	std::string m_Name;
	size_t m_Size;
};

struct Shop
{
	std::map<std::string,Cargo> m_Content;
};

class ShopManager:public Wt::WObject
{
public:
	ShopManager(const std::string& user_name);
	~ShopManager();

	Shop& GetShop(const std::string& name);
private:
	void LoadShop();
	void SaveShop();

	std::string m_FileName;
	std::fstream m_File;

	std::map<std::string,Shop> m_Content;

	Wt::Signal<Wt::WString> m_ErrorHappenedSignal;
};