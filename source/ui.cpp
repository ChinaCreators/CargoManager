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
	for(auto i:m_pTab->children())
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

LoginView::LoginView(MainApplication& app)
	:m_Application(app)
{
	setContentAlignment(Wt::AlignmentFlag::Center);
	setVerticalAlignment(Wt::AlignmentFlag::Center);
	addStyleClass("login_view");

	m_pError=nullptr;
	m_pUserNameNotify=addWidget(std::make_unique<Wt::WText>(L"用户名"));
	m_pUserNameInput=addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pPasswordNotify=addWidget(std::make_unique<Wt::WText>(L"密码"));
	m_pPasswordInput=addWidget(std::make_unique<Wt::WLineEdit>());
	m_pPasswordInput->setAttributeValue("type","password");
	addWidget(std::make_unique<Wt::WBreak>());
	m_pSubmit=addWidget(std::make_unique<Wt::WPushButton>("Sign in"));

	m_pSubmit->clicked().connect(this,&LoginView::Login);
	m_pPasswordInput->enterPressed().connect(this,&LoginView::Login);
	m_Application.m_AccountServer.m_SignUpSignal.connect(this,&LoginView::AfterLogin);
	m_Application.m_AccountServer.m_ErrorHappenedSignal.connect(this,&LoginView::LoginError);
}

void LoginView::Login()
{
	std::string name=m_pUserNameInput->text().toUTF8();
	std::string pwd=m_pPasswordInput->text().toUTF8();
	m_Application.m_AccountServer.SignIn(name,pwd);
}

void LoginView::AfterLogin(const Wt::WString& name)
{
	m_pUserNameNotify->hide();
	m_pPasswordNotify->hide();
	m_pUserNameInput->hide();
	m_pPasswordInput->hide();
	m_pSubmit->hide();
	if(m_pError)
		m_pError->hide();
	m_pUserName=addWidget(std::make_unique<Wt::WText>(L"当前用户:"+name));
}

void LoginView::LoginError(const Wt::WString& error)
{
	if(m_pError)
	{
		m_pError->setText(error);	
	}
	else
	{
		m_pError=addWidget(std::make_unique<Wt::WText>(error));
	}
}

MainApplication::MainApplication(const Wt::WEnvironment& env)
	:WApplication(env)
{
	setTitle("CargoManager");
	useStyleSheet("./resources/main.css");

	m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name){
		m_UserName=name;
		//todo add user name display
	});

	m_pNavigation=root()->addWidget(std::make_unique<Navigation>());

	m_pNavigation->AddTab(L"登录",std::make_unique<LoginView>(*this));
	m_pNavigation->AddTab(L"商铺",std::make_unique<ShopView>(*this));
}

ShopView::ShopView(MainApplication& app)
		:m_Application(app)
{
	m_pError=addWidget(std::make_unique<Wt::WText>(L"请先登录"));
	m_pIndex=nullptr;
	m_Application.m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name){
		m_Content.Init(name.toUTF8());
		m_pError->hide();
		m_pIndex=addWidget(std::make_unique<Wt::WContainerWidget>());
		RefreshIndex();
		// for(auto& i:m_Content.m_Content)
		// {
		// 	auto pindex_item=m_pIndex->addWidget(std::make_unique<Wt::WText>(i.first));
		// 	m_pIndex->addWidget(std::make_unique<Wt::WBreak>());
		// 	auto pcargo_view=addWidget(std::make_unique<Wt::WContainerWidget>());

		// 	pindex_item->clicked().connect([=](){
		// 		m_pIndex->hide();
		// 		pcargo_view->show();
		// 	});

		// 	m_Shops.insert(std::make_pair(i.first,pcargo_view));
		
		// 	pcargo_view->addWidget(std::make_unique<Wt::WPushButton>(L"back"))->clicked().connect([=](){
		// 		pcargo_view->hide();
		// 		m_pIndex->show();
		// 					});	//back button
		// 	pcargo_view->addWidget(std::make_unique<Wt::WText>(i.first));		//shop name
		// 	pcargo_view->addWidget(std::make_unique<Wt::WBreak>());
		// 	for(auto& j:i.second.m_Content)
		// 	{
		// 		pcargo_view->addWidget(std::make_unique<Wt::WText>(j.first));
		// 		pcargo_view->addWidget(std::make_unique<Wt::WText>(std::to_string(j.second.m_Size)));
		// 		pcargo_view->addWidget(std::make_unique<Wt::WBreak>());	
		// 	}
			
		// 	pcargo_view->hide();
		// }
		// auto pnshop=m_pIndex->addWidget(std::make_unique<Wt::WLineEdit>(L"添加店铺"));	
		// m_pIndex->addWidget(std::make_unique<Wt::WPushButton>(L"Submit"))->clicked().connect(
		// [&,this,pnshop](){
		// 	m_Content.m_Content.insert(std::make_pair(pnshop->text().toUTF8(),Shop()));
		// 	//todo refresh
		// }
		// );
	});	
}

void ShopView::RefreshIndex()
{
	auto buffer_ptr=m_pIndex;
	buffer_ptr->hide();
	m_pIndex=addNew<Wt::WContainerWidget>();
	for(auto& i:m_Content.m_Content)
	{
		m_pIndex->addNew<Wt::WText>(i.first);
		m_pIndex->addNew<Wt::WBreak>();
	}

	auto perror=m_pIndex->addNew<Wt::WText>();
	perror->hide();

	m_pIndex->addNew<Wt::WBreak>();

	auto pnline=m_pIndex->addNew<Wt::WLineEdit>();
	pnline->setPlaceholderText("the shop you want to create");
	pnline->enterPressed().connect(
		[&,this](){
			auto iter=this->m_Content.m_Content.find(pnline->text().toUTF8());
			if(iter!=this->m_Content.m_Content.end())
			{
				perror->setText(L"不能重复添加商铺");
				perror->show();
			}
			else
			{
				this->m_Content.m_Content.insert(std::make_pair(pnline->text().toUTF8(),Shop()));
				this->RefreshIndex();
			}
		}
	);

	m_pIndex->addNew<Wt::WBreak>();

	auto pdline=m_pIndex->addNew<Wt::WLineEdit>();
	pdline->setPlaceholderText("the shop you want to delete");
	pdline->enterPressed().connect(
		[&,this](){
			auto iter=this->m_Content.m_Content.find(pnline->text().toUTF8());
			if(iter==this->m_Content.m_Content.end())
			{
				perror->setText(L" 没有该商铺");
				perror->show();
			}
			else
			{
				this->m_Content.m_Content.erase(this->m_Content.m_Content.find(pdline->text().toUTF8()));
				this->RefreshIndex();
			}
		}
	);

	buffer_ptr->children.clear();
}