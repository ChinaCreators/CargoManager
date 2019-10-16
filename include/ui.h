#pragma once
#include "back_end.h"
#include <Wt/WApplication.h>
#include <Wt/WPanel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WTabWidget.h>
#include <Wt/WMenuItem.h>

class MainApplication;

class Navigation:public Wt::WContainerWidget
{
public:
	Navigation();

	void AddTab(const Wt::WString& title,std::unique_ptr<Wt::WWidget>&& ptr);
private:
	void SetActive(Wt::WWidget* ptab,Wt::WWidget* pcontent);

	std::map<Wt::WString,Wt::WWidget*> m_Content;
	Wt::WContainerWidget* m_pTab;
	Wt::WHBoxLayout* m_pLayout;

	WWidget* m_pActiveTab;
	WWidget* m_pActiveContent;

	Wt::WString m_DefaultStyleClassesForTab,m_NewStyleClassesForTab;
};

class LoginView:public Wt::WContainerWidget
{
public:
	LoginView(MainApplication& app);
private:
	void Login();
	void AfterLogin(const Wt::WString& name);
	void LoginError(const Wt::WString& error);

	MainApplication& m_Application;

	Wt::WText* m_pUserNameNotify,*m_pPasswordNotify;
	Wt::WLineEdit *m_pUserNameInput,*m_pPasswordInput;
	Wt::WPushButton* m_pSubmit;

	Wt::WText* m_pError;
	Wt::WText* m_pUserName;
};

class ShopView:public Wt::WContainerWidget
{
public:
	ShopView(MainApplication& app);
private:
	ShopManager m_Content;

	MainApplication& m_Application;

	Wt::WContainerWidget* m_pIndex;
	std::map<Wt::WString,Wt::WContainerWidget*> m_Shops;
	Wt::WText* m_pError;
};

class CargoView:public Wt::WContainerWidget
{
public:
	CargoView(std::map<std::string,Cargo>& content);
private:
};

class MainApplication:public Wt::WApplication
{
public:
	MainApplication(const Wt::WEnvironment& env);
	AccountServer m_AccountServer;

	friend LoginView;
private:
	Wt::WString m_UserName;
	Navigation* m_pNavigation;
};
