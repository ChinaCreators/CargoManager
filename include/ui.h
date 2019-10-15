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

class LoginView:public Wt::WContainerWidget
{
public:
	LoginView();
private:
	Wt::WText* m_pUserNameNotify,*m_pPasswordNotify;
	Wt::WLineEdit *m_pUserNameInput,*m_pPasswordInput;
	Wt::WPushButton* m_pSubmit;
};

class TopLabel:public Wt::WContainerWidget
{
public:
	TopLabel(const Wt::WString& user_name);
	void UpdateUser();
private:
	Wt::WHBoxLayout* m_pLayout;
	Wt::WText* m_pUserName;
	const Wt::WString& m_UserName;
};

class MainApplication:public Wt::WApplication
{
public:
	MainApplication(const Wt::WEnvironment& env);
private:
	Wt::WString m_UserName;
	AccountServer m_AccountServer;

	TopLabel* m_pTopLabel;
	LoginView* m_pLoginView;
};