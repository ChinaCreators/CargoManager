#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cstdint>
#include <Wt/WObject.h>
#include <Wt/WSignal.h>
#include <Wt/WString.h>
#include <Wt/WLogger.h>

struct Account
{
	std::string m_Name;
	size_t m_HashedPassword;
};

class AccountServer : public Wt::WObject
{
public:
	void SignUp(const std::string& name, const std::string& pwd);
	void SignIn(const std::string& name, const std::string& pwd);
	void SignOut(const std::string& name);

	Wt::Signal<Wt::WString> m_ErrorHappenedSignal;
	Wt::Signal<Wt::WString> m_SignUpSignal;
};

class AccountManager : public Wt::WObject
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
	std::map<std::string, Account> m_Content;
};

struct Cargo
{
	std::string m_Name;
	size_t m_Size;
};

struct Shop
{
	std::map<std::string, Cargo> m_Content;
};

class ShopManager : public Wt::WObject
{
public:
	ShopManager();
	~ShopManager();

	void Init(const std::string& name);
	void LoadShop();
	void SaveShop();

	Wt::Signal<> m_InitSingal;

private:
	std::string m_FileName;
	std::fstream m_File;

public:
	std::map<std::string, Shop> m_Content;

	Wt::Signal<Wt::WString> m_ErrorHappenedSignal;
};
