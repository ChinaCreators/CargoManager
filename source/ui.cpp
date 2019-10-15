#include "ui.h"

LoginView::LoginView()
{
	setContentAlignment(Wt::AlignmentFlag::Center);
	m_pUserNameNotify=addWidget(std::make_unique<Wt::WText>(L"用户名"));
	m_pUserNameInput=addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pPasswordNotify=addWidget(std::make_unique<Wt::WText>(L"密码"));
	m_pPasswordInput=addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pSubmit=addWidget(std::make_unique<Wt::WPushButton>());
}

MainApplication::MainApplication(const Wt::WEnvironment& env)
	:WApplication(env)
{
	m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name){
		m_UserName=name;
		m_pTopLabel->UpdateUser();	
	});

	m_pTopLabel=root()->addWidget(std::make_unique<TopLabel>());
	m_pLoginView=root()->addWidget(std::make_unique<LoginView>());
}

TopLabel::TopLabel(const Wt::WString& user_name)
	:m_UserName(user_name)
{
	m_pLayout=setLayout(std::make_unique<Wt::WHBoxLayout>());
	
	m_pUserName=m_pLayout->addWidget(std::make_unique<Wt::WText>());

}

void TopLabel::UpdateUser()
{
	m_pUserName->setText(m_UserName);
}