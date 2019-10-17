#include "ui.h"

Navigation::Navigation()
{
	m_pActiveTab = nullptr;
	m_pActiveContent = nullptr;
	m_pTab = addWidget(std::make_unique<Wt::WContainerWidget>());
	m_pLayout = m_pTab->setLayout(std::make_unique<Wt::WHBoxLayout>());
}

void Navigation::SetActive(Wt::WWidget* ptab, Wt::WWidget* pcontent)
{
	for (auto i : m_Content)
		i.second->hide();
	for (auto i : m_pTab->children())
		i->setStyleClass(m_DefaultStyleClassesForTab);

	ptab->setStyleClass(m_NewStyleClassesForTab);
	pcontent->show();
}

Wt::WWidget* Navigation::AddTab(const Wt::WString& title, std::unique_ptr<Wt::WWidget>&& ptr, std::function<void(Wt::WWidget*)> func)
{
	m_Content.insert(std::make_pair(title, ptr.get()));

	auto ptab = std::make_unique<Wt::WText>(title);
	ptab->addStyleClass("tab_item");

	auto _ptab = ptab.get();
	auto _pcontent = ptr.get();
	ptab->clicked().connect([=, ptr_ = ptr.get()]() {
		this->SetActive(_ptab, _pcontent);
		func(ptr_);
	});

	if (m_pActiveTab == nullptr && m_pActiveContent == nullptr)
	{
		m_pActiveContent = ptr.get();
		m_pActiveTab = ptab.get();

		m_DefaultStyleClassesForTab = m_pActiveTab->styleClass();
		m_pActiveTab->addStyleClass("active_tab");
		m_NewStyleClassesForTab = m_pActiveTab->styleClass();

		m_pActiveContent->show();
	}
	else
	{
		ptr->hide();
	}

	auto re = ptr.get();
	addWidget(std::move(ptr));
	m_pLayout->addWidget(std::move(ptab));

	return re;
}

LoginView::LoginView(MainApplication& app)
	: m_Application(app)
{
	setContentAlignment(Wt::AlignmentFlag::Center);
	addStyleClass("login_view");

	m_pError = nullptr;
	m_pUserNameNotify = addWidget(std::make_unique<Wt::WText>(L"用户名"));
	m_pUserNameInput = addWidget(std::make_unique<Wt::WLineEdit>());
	addWidget(std::make_unique<Wt::WBreak>());
	m_pPasswordNotify = addWidget(std::make_unique<Wt::WText>(L"密码"));
	m_pPasswordInput = addWidget(std::make_unique<Wt::WLineEdit>());
	m_pPasswordInput->setAttributeValue("type", "password");
	addWidget(std::make_unique<Wt::WBreak>());
	m_pSubmit = addWidget(std::make_unique<Wt::WPushButton>("Sign in"));

	m_pSubmit->clicked().connect(this, &LoginView::Login);
	m_pPasswordInput->enterPressed().connect(this, &LoginView::Login);
	m_Application.m_AccountServer.m_SignUpSignal.connect(this, &LoginView::AfterLogin);
	m_Application.m_AccountServer.m_ErrorHappenedSignal.connect(this, &LoginView::LoginError);
}

void LoginView::Login()
{
	std::string name = m_pUserNameInput->text().toUTF8();
	std::string pwd = m_pPasswordInput->text().toUTF8();
	m_Application.m_AccountServer.SignIn(name, pwd);
}

void LoginView::AfterLogin(const Wt::WString& name)
{
	m_pUserNameNotify->hide();
	m_pPasswordNotify->hide();
	m_pUserNameInput->hide();
	m_pPasswordInput->hide();
	m_pSubmit->hide();
	if (m_pError)
		m_pError->hide();
	m_pUserName = addWidget(std::make_unique<Wt::WText>(L"当前用户:" + name));
}

void LoginView::LoginError(const Wt::WString& error)
{
	if (m_pError)
	{
		m_pError->setText(error);
	}
	else
	{
		m_pError = addWidget(std::make_unique<Wt::WText>(error));
	}
}

bool LoginView::AvoidMultiLogin(const Wt::WString& name)
{
	std::fstream file(name.toUTF8() + ".sgn", std::ios::in);
	int sign = 0;
	file >> sign;
	if (sign == 1)
	{
		LoginError(L"不能同时登陆");
		file.close();
		return false;
	}
	else
	{
		file.close();
		file.open(name.toUTF8() + ".sgn", std::ios::out);
		file << 1;
		file.close();
		return true;
	}
}

MainApplication::MainApplication(const Wt::WEnvironment& env)
	: WApplication(env)
{
	setTitle("CargoManager");
	useStyleSheet("./resources/main.css");
	setLocale(Wt::WLocale::currentLocale());

	m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name) {
		m_UserName = name;
		//todo add user name display
	});

	m_pNavigation = root()->addWidget(std::make_unique<Navigation>());

	m_pNavigation->AddTab(L"登录", std::make_unique<LoginView>(*this));
	auto pshop = reinterpret_cast<ShopView*>(m_pNavigation->AddTab(L"商铺", std::make_unique<ShopView>(*this), [](Wt::WWidget* ptr) { reinterpret_cast<ShopView*>(ptr)->Refresh(); }));
	m_pNavigation->AddTab(L"货物", std::make_unique<CargoView>(pshop->m_Content), [](Wt::WWidget* ptr) { reinterpret_cast<CargoView*>(ptr)->Refresh(); });
}

ShopView::ShopView(MainApplication& app)
	: m_Application(app)
{
	m_pError = addWidget(std::make_unique<Wt::WText>(L"请先登录"));
	m_pIndex = nullptr;
	m_Application.m_AccountServer.m_SignUpSignal.connect([this](const Wt::WString& name) {
		m_Content.Init(name.toUTF8());
		m_pError->hide();
		m_pIndex = addWidget(std::make_unique<Wt::WContainerWidget>());
		m_pShop = addWidget(std::make_unique<Wt::WContainerWidget>());
		m_pShop->hide();
		RefreshIndex();
	});
}

bool IsInvalidString(const Wt::WString& str)
{
	return str.empty() || ([&]() -> bool {for(auto i :str.toUTF8()){if (i != ' ')return false;}return true; })() || str.toUTF8().empty() || str.toUTF8().size() == 0;
}

void ShopView::RefreshIndex()
{
	m_pIndex->clear();

	for (auto& i : m_Content.m_Content)
	{
		m_pIndex->addNew<Wt::WText>(i.first)->clicked().connect([&, this]() {
			m_CurrentShop = i.first;
			this->RefreshShop(i.first, i.second.m_Content);
			this->m_pIndex->hide();
			this->m_pShop->show();
		});
		m_pIndex->addNew<Wt::WBreak>();
	}

	auto perror = m_pIndex->addNew<Wt::WText>();
	perror->hide();

	m_pIndex->addNew<Wt::WBreak>();

	auto pnline = m_pIndex->addNew<Wt::WLineEdit>();
	pnline->setPlaceholderText(L"添加店铺");
	auto newfunc = [=]() {
		if (IsInvalidString(pnline->text()))
		{
			perror->setText(L"不能为空");
			perror->show();
			return;
		}
		auto iter = this->m_Content.m_Content.find(pnline->text().toUTF8());
		if (iter != this->m_Content.m_Content.end())
		{
			perror->setText(L"不能重复添加商铺");
			perror->show();
		}
		else
		{
			this->m_Content.m_Content.insert(std::make_pair(pnline->text().toUTF8(), Shop()));
			m_Content.SaveShop();
			this->RefreshIndex();
		}
	};
	pnline->enterPressed().connect(newfunc);
	m_pIndex->addNew<Wt::WPushButton>(L"Submit")->clicked().connect(newfunc);
	m_pIndex->addNew<Wt::WBreak>();

	auto pdline = m_pIndex->addNew<Wt::WLineEdit>();
	pdline->setPlaceholderText(L"删除店铺");
	auto deletefunc = [=]() {
		if (IsInvalidString(pdline->text()))
		{
			perror->setText(L"不能为空");
			perror->show();
			return;
		}
		auto iter = this->m_Content.m_Content.find(pdline->text().toUTF8());
		if (iter == this->m_Content.m_Content.end())
		{
			perror->setText(L" 没有该商铺");
			perror->show();
		}
		else
		{
			this->m_Content.m_Content.erase(iter);
			m_Content.SaveShop();
			this->RefreshIndex();
		}
	};
	pdline->enterPressed().connect(deletefunc);
	m_pIndex->addNew<Wt::WPushButton>(L"Submit")->clicked().connect(deletefunc);
}

void ShopView::RefreshShop(const Wt::WString& shop_name, std::map<std::string, Cargo>& cargos)
{
	m_pShop->clear();

	m_pShop->addNew<Wt::WPushButton>(L"Back")->clicked().connect([this]() {
		this->RefreshIndex();
		m_pShop->hide();
		m_pIndex->show();
	});

	m_pShop->addNew<Wt::WText>(shop_name);
	m_pShop->addNew<Wt::WBreak>();

	for (auto& i : cargos)
	{
		m_pShop->addNew<Wt::WText>(i.first);
		m_pShop->addNew<Wt::WText>(std::to_string(i.second.m_Size));
		if (i.second.m_Size < 5)
			m_pShop->addNew<Wt::WText>(L"需要进货");
		m_pShop->addNew<Wt::WBreak>();
	}

	auto perror = m_pShop->addNew<Wt::WText>();
	perror->hide();
	m_pShop->addNew<Wt::WBreak>();

	auto pnline = m_pShop->addNew<Wt::WLineEdit>();
	pnline->setPlaceholderText(L"想添加的货物类型");
	auto pnbutton = m_pShop->addNew<Wt::WPushButton>(L"Submit");

	auto newfunc = [=, &cargos]() {
		if (IsInvalidString(pnline->text()))
		{
			perror->setText(L"不能为空");
			perror->show();
			return;
		}
		auto iter = cargos.find(pnline->text().toUTF8());
		if (iter != cargos.end())
		{
			perror->setText(L"该种货物已存在");
			perror->show();
		}
		else
		{
			cargos.insert(std::make_pair(pnline->text().toUTF8(), Cargo{pnline->text().toUTF8(), 0}));
			m_Content.SaveShop();
			this->RefreshShop(shop_name, cargos);
		}
	};

	pnline->enterPressed().connect(newfunc);
	pnbutton->clicked().connect(newfunc);

	m_pShop->addNew<Wt::WBreak>();
	auto pdline = m_pShop->addNew<Wt::WLineEdit>();
	pdline->setPlaceholderText(L"想删除的货物类型");
	auto pdbutton = m_pShop->addNew<Wt::WPushButton>(L"Submit");

	auto deletefunc = [=, &cargos]() {
		if (IsInvalidString(pdline->text()))
		{
			perror->setText(L"不能为空");
			perror->show();
			return;
		}
		auto iter = cargos.find(pdline->text().toUTF8());
		if (iter == cargos.end())
		{
			perror->setText(L"该种货物不存在");
			perror->show();
		}
		else
		{
			cargos.erase(iter);
			m_Content.SaveShop();
			this->RefreshShop(shop_name, cargos);
		}
	};
	pdline->enterPressed().connect(deletefunc);
	pdbutton->clicked().connect(deletefunc);
}

bool IsInvalidNumber(const Wt::WString& str)
{
	for (auto i : str.toUTF8())
	{
		if (i > '9' || i < '0')
			return true;
	}
	return false;
}

CargoView::CargoView(ShopManager& manager)
	: m_ShopManager(manager)
{
	m_IsInit = false;
	m_pError = addNew<Wt::WText>(L"请先登录");
	m_ShopManager.m_InitSingal.connect([this]() {
		m_IsInit = true;
		m_pError->hide();
		m_pShop = addNew<Wt::WComboBox>();
		for (auto& i : m_ShopManager.m_Content)
		{
			m_pShop->addItem(i.first);
		}

		m_pAdd = addNew<Wt::WPushButton>(L"添加");
		m_pSubmit = addNew<Wt::WPushButton>(L"提交");
		m_pContent = addNew<Wt::WContainerWidget>();

		m_pShop->activated().connect([this]() { this->m_pContent->clear(); });

		m_pAdd->clicked().connect([this]() {
			if (!IsInvalidString(m_pShop->valueText()))
			{
				std::string shop_name = m_pShop->valueText().toUTF8();
				auto iter = this->m_ShopManager.m_Content.find(shop_name);
				auto pcargo = m_pContent->addNew<Wt::WComboBox>();
				for (auto& i : iter->second.m_Content)
				{
					pcargo->addItem(i.first);
				}
				auto pinput = m_pContent->addNew<Wt::WLineEdit>();
				pinput->setPlaceholderText(L"输入正负数以进出货物");
				m_pContent->addNew<Wt::WBreak>();
			}
		});

		m_pSubmit->clicked().connect([this]() {
			std::vector<Wt::WWidget*> list = m_pContent->children();
			for (int i = 0; i < list.size(); i += 3)
			{
				auto shop_name = m_pShop->currentText();
				auto cargo_name = reinterpret_cast<Wt::WComboBox*>(list[i])->currentText();
				int cargo_change = 0;
				if (!IsInvalidString(reinterpret_cast<Wt::WLineEdit*>(list[i + 1])->text()) && !IsInvalidNumber(reinterpret_cast<Wt::WLineEdit*>(list[i + 1])->text()))
					cargo_change = std::stoi(reinterpret_cast<Wt::WLineEdit*>(list[i + 1])->text().toUTF8());
				if (!IsInvalidString(shop_name) && !IsInvalidString(cargo_name))
					m_ShopManager.m_Content[shop_name.toUTF8()].m_Content[cargo_name.toUTF8()].m_Size += cargo_change;
			}
			m_pContent->clear();
		});
	});
}

void CargoView::Refresh()
{
	if (m_IsInit)
	{
		m_pShop->clear();
		m_pContent->clear();
		for (auto& i : m_ShopManager.m_Content)
		{
			m_pShop->addItem(i.first);
		}
	}
}

void ShopView::Refresh()
{
	if (m_pIndex && m_pShop)
	{
		if (m_pIndex->isHidden())
			RefreshShop(m_CurrentShop, m_Content.m_Content[m_CurrentShop.toUTF8()].m_Content);
		else
			RefreshIndex();
	}
}