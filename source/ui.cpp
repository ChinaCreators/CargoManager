#include "ui.h"

Navigation::Navigation()
{
	m_pActiveTab=nullptr;
	m_pActiveContent=nullptr;
	m_pTab=addWidget(std::make_unique<Wt::WContainerWidget>());
	m_pLayout=m_pTab->setLayout(std::make_unique<Wt::WHBoxLayout>());
}

void Navigation::SetActive(Wt::WWidget* ptab,Wt::WWidget* pcontent)
{
	for(auto i:m_Content)
		i.second->hide();
	for(auto i:m_pLayout->widget()->children())
		i->setStyleClass(m_DefaultStyleClassesForTab);

	ptab->setStyleClass(m_NewStyleClassesForTab);
	pcontent->show();
}

void Navigation::AddTab(const Wt::WString& title,std::unique_ptr<Wt::WWidget>&& ptr)
{
	m_Content.insert(std::make_pair(title,ptr.get()));
	
	auto ptab=std::make_unique<Wt::WText>(title);
	ptab->addStyleClass("tab_item");

	auto _ptab=ptab.get();
	auto _pcontent=ptr.get();
	ptab->clicked().connect([=](){
		this->SetActive(_ptab,_pcontent);
	});

	if(m_pActiveTab==nullptr&&m_pActiveContent==nullptr)
	{
		m_pActiveContent=ptr.get();
		m_pActiveTab=ptab.get();

		m_DefaultStyleClassesForTab=m_pActiveTab->styleClass();
		m_pActiveTab->addStyleClass("active_tab");
		m_NewStyleClassesForTab=m_pActiveTab->styleClass();

		m_pActiveContent->show();
	}
	else
	{
		ptr->hide();
	}

	addWidget(std::move(ptr));
	m_pLayout->addWidget(std::move(ptab));
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

	m_pNavigation=root()->addWidget(std::make_unique<Navigation>());

	m_pNavigation->AddTab(L"登录",std::make_unique<LoginView>());
	m_pNavigation->AddTab(L"商铺",std::make_unique<ShopView>());
}

ShopView::ShopView()
{

}