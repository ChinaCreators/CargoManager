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

class Navigation:public Wt::WContainerWidget
{
public:
	void AddTab(const Wt::WString& title,std::unique_ptr<Wt::WWidget>&& ptr);
private:
	std::map<Wt::WString,Wt::WWidget*> m_Content;
	Wt::WContainerWidget* m_Tab;
};

class LoginView:public Wt::WContainerWidget
{
public:
	LoginView();
private:
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