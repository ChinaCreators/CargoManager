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

	MainApplication& m_Application;

	Wt::WText* m_pUserNameNotify,*m_pPasswordNotify;
	Wt::WLineEdit *m_pUserNameInput,*m_pPasswordInput;
	Wt::WPushButton* m_pSubmit;
};

class ShopView:public Wt::WContainerWidget
{
public:
	ShopView();
private:
};

class MainApplication:public Wt::WApplication
{
public:
	MainApplication(const Wt::WEnvironment& env);
private:
	Wt::WString m_UserName;
	AccountServer m_AccountServer;
	Navigation* m_pNavigation;
};
