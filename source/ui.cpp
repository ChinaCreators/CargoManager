#include "ui.h"

LoginView::LoginView()
{
	setContentAlignment(Wt::AlignmentFlag::Center);
	m_pUserNameNotify=addWidget(std::make_unique<Wt::WText>(L"用户名"));
	m_pUserNameInput=addWidget(std::make_unique<Wt::WTextEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pPasswordNotify=addWidget(std::make_unique<Wt::WText>(L"密码"));
	m_pPasswordInput=addWidget(std::make_unique<Wt::WTextEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pSubmit=addWidget(std::make_unique<Wt::WPushButton>());	
}

MainApplication::MainApplication(const Wt::WEnvironment& env)
	:WApplication(env)
{
	m_pLoginView=root()->addWidget(std::make_unique<LoginView>());
}