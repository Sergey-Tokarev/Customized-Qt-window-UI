#include "transparentmainwindow.h"
#include "ui_transparentmainwindow.h"
#include "customwindow.h"


gui::TransparentMainWindow::TransparentMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::TransparentMainWindow)
{
    m_oldFashionWindowFlags = this->windowFlags();
    m_ui->setupUi(this);
}

gui::TransparentMainWindow::~TransparentMainWindow()
{
    delete m_ui;
}

void gui::TransparentMainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    InitCustomWindow();
}

void gui::TransparentMainWindow::InitCustomWindow()
{
    static bool s_wasShown = false;

    if (!s_wasShown)
    {
        m_ui->verLayoutContentWindow->addWidget(CreateCustomWindow(this));
        s_wasShown = true;
        OnWindowStyleChanged(true);
    }
}

QWidget* gui::TransparentMainWindow::CreateCustomWindow(QWidget* parent)
{
    gui::CustomWindow* customWindow = new gui::CustomWindow(parent);
    QObject::connect(customWindow, SIGNAL(ChangeWindowStyle(bool)), this, SLOT(OnWindowStyleChanged(bool)));

    return customWindow;
}

void gui::TransparentMainWindow::OnWindowStyleChanged(bool isOldStyle)
{
    if(isOldStyle)
    {
        this->setWindowFlags(m_oldFashionWindowFlags);
        m_ui->centralWidget->setContentsMargins(0,0,0,0);
        m_ui->centralWidget->layout()->setContentsMargins(0,0,0,0);
    }
    else // custom style
    {
        this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
        m_ui->centralWidget->setContentsMargins(5,5,5,5);
        m_ui->centralWidget->layout()->setContentsMargins(0,0,0,0);
    }
    emit ShowCaptionPanel(!isOldStyle);
    this->show();
}
