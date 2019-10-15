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
		m_Content.insert(std::make_pair(name,Account{name,hpwd}));
	}
	m_File.close();
}

void AccountManager::SaveAccount()
{
	m_File.open(sm_FileName,std::ios::out);
	m_File<<m_Content.size()<<std::endl;
	for(auto& i:m_Content)
	{
		m_File<<i.second.m_Name<<" "<<i.second.m_HashedPassword<<std::endl;
	}
	m_File.close();
}

void AccountServer::SignUp(const std::string& name,std::string& pwd)
{
	auto iter=AccountManager::GetInstance().m_Content.find(name);
	if(iter!=AccountManager::GetInstance().m_Content.end())
		m_ErrorHappenedSignal.emit(L"重复的账号名");
	else
	{
		AccountManager::GetInstance().m_Content.insert(std::make_pair(name,Account{name,std::hash<std::string>{}(pwd)}));
		m_SignUpSignal.emit(Wt::WString(name));
	}
}

void AccountServer::SignIn(const std::string& name,std::string& pwd)
{
	auto iter=AccountManager::GetInstance().m_Content.find(name);
	if(iter==AccountManager::GetInstance().m_Content.end())
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

ShopManager::ShopManager(const std::string& user_name)
{
	m_FileName=user_name+".dat";
	LoadShop();	
}

ShopManager::~ShopManager()
{
	SaveShop();
}

void ShopManager::LoadShop()
{
	m_File.open(m_FileName,std::ios::in);

	size_t size;
	m_File>>size;
	for(int i=0;i<size;i++)
	{
		std::string shop_name;
		size_t cargo_type_size;
		m_File>>shop_name>>cargo_type_size;
		std::map<std::string,Cargo> buffer;
		for(int j=0;j<cargo_type_size;j++)
		{
			std::string cargo_name;
			size_t cargo_size;
			m_File>>cargo_name>>cargo_size;
			buffer.insert(std::make_pair(cargo_name,Cargo{cargo_name,cargo_size}));
		}
		m_Content.insert(std::make_pair(shop_name,Shop{buffer}));
	}

	m_File.close();
}

void ShopManager::SaveShop()
{
	m_File.open(m_FileName,std::ios::out);

	m_File<<m_Content.size()<<std::endl;
	for(auto& i: m_Content)
	{
		m_File<<i.first<<" "<<i.second.m_Content.size()<<" ";
		for(auto& j:i.second.m_Content)
		{
			m_File<<j.second.m_Name<<" "<<j.second.m_Size<<" ";
		}
		m_File<<std::endl;
	}

	m_File.close();
}

Shop& ShopManager::GetShop(const std::string& name)
{
	auto iter=m_Content.find(name);
	if(iter==m_Content.end())
		m_ErrorHappenedSignal.emit(L"没有该店铺");
	else
	{
		return iter->second;
	}
}