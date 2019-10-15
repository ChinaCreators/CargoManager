#pragma once
#include "back_end.h"
#include <Wt/WApplication.h>
#include <Wt/WPanel.h>
#include <Wt/WTextEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>

class LoginView:public Wt::WContainerWidget
{
public:
	LoginView();
private:
	Wt::WText* m_pUserNameNotify,*m_pPasswordNotify;
	Wt::WTextEdit *m_pUserNameInput,*m_pPasswordInput;
	Wt::WPushButton* m_pSubmit;
};

class MainApplication:public Wt::WApplication
{
public:
	MainApplication(const Wt::WEnvironment& env);
private:
	std::string m_UserName;

	LoginView* m_pLoginView;
};