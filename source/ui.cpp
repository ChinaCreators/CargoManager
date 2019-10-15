#include "ui.h"

void Navigation::AddTab(const Wt::WString& title,std::unique_ptr<Wt::WWidget>&& ptr)
{
	m_Tab=addWidget(std::make_unique<Wt::WContainerWidget>());
	m_Content.insert(std::make_pair(title,ptr.get()));
	addWidget(std::move(ptr));
	auto ptab=std::make_unique<Wt::WText>(title);
	ptab->addStyleClass("tab_item");
	m_Tab->addWidget(std::move(ptab));
}

LoginView::LoginView()
{
	setContentAlignment(Wt::AlignmentFlag::Center);
	m_pUserNameNotify=addWidget(std::make_unique<Wt::WText>(L"用户名"));
	m_pUserNameInput=addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pPasswordNotify=addWidget(std::make_unique<Wt::WText>(L"密码"));
	m_pPasswordInput=addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pSubmit=addWidget(std::make_unique<Wt::WPushButton>("Sign in"));
}

MainApplication::MainApplication(const Wt::WEnvironment& env)
	:WApplication(env)
{
	setTitle("CargoManager");
	useStyleSheet("main.css");

	m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name){
		m_UserName=name;
		//todo add user name display
	});
}

ShopView::ShopView()
{

}