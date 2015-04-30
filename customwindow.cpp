#include <QIcon>
#include <QMenu>
#include <QMouseEvent>
#include "customwindow.h"
#include "ui_customwindow.h"
#include "switcherbutton.h"

gui::CustomWindow::CustomWindow(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CustomWindow),
    m_parentWindow(parent),
    m_isOldStyle(true),
    m_leftButtonPressed(false)
{
    m_ui->setupUi(this);

    CreateActions();
    UpdateRestoreMaximizeButtonIcon();
    InstallEventFilter();
    Init();

    m_ui->widgetCaptionPanel->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(m_ui->widgetCaptionPanel, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this,                     SLOT(ShowCaptionContextMenu(const QPoint&)));

    QObject::connect(parent,                   SIGNAL(ShowCaptionPanel(bool)),
                     this,                     SLOT(OnShowCaptionPanel(bool)));

    QObject::connect(this,                     SIGNAL(RestoreWindow()),
                     parent,                   SLOT(showNormal()));

    QObject::connect(this,                     SIGNAL(MinimizeWindow()),
                     parent,                   SLOT(showMinimized()));

    QObject::connect(this,                     SIGNAL(MaximizeWindow()),
                     parent,                   SLOT(showMaximized()));

    QObject::connect(this,                     SIGNAL(CloseWindow()),
                     parent,                   SLOT(close()));
}

gui::CustomWindow::~CustomWindow()
{
    delete m_ui;
}

void gui::CustomWindow::Init()
{
    QSpacerItem* vertSpacer1 = new QSpacerItem(20, 100, QSizePolicy::Fixed, QSizePolicy::Expanding);
    QSpacerItem* horizSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout* vLayout = new QVBoxLayout(m_ui->widgetWindowContent);
    vLayout->addSpacerItem(vertSpacer1);

    QHBoxLayout* hLayout1 = new QHBoxLayout(m_ui->widgetWindowContent);
    hLayout1->addSpacerItem(horizSpacer);

    SwitcherButton* switcher = new SwitcherButton(m_ui->widgetWindowContent);
    QObject::connect(switcher, SIGNAL(Toggled(bool)), this, SLOT(OnSwitcherToggled(bool)));

    hLayout1->addWidget(switcher);
    hLayout1->addSpacerItem(horizSpacer);

    vLayout->addLayout(hLayout1);

    vLayout->addSpacerItem(vertSpacer1);

    QHBoxLayout* hLayout = new QHBoxLayout(m_ui->widgetWindowContent);

    hLayout->addSpacerItem(horizSpacer);
    QLabel* label = new QLabel("USE SLIDER TO SWITCH WINDOW STATES", m_ui->widgetWindowContent);
    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    label->setFont(font);
    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), QColor(214,214,214));
    label->setPalette(palette);

    hLayout->addWidget(label);
    hLayout->addSpacerItem(horizSpacer);

    vLayout->addLayout(hLayout);

    vLayout->addSpacerItem(vertSpacer1);

    SetLayout(vLayout);

    switcher->SetSize(QRect(50,50, 600, 300));
}

QLayout* gui::CustomWindow::GetContentLayout()
{
    return m_ui->widgetWindowContent->layout();
}

void gui::CustomWindow::SetLayout(QLayout* layout)
{
    m_ui->widgetWindowContent->setLayout(layout);
}

void gui::CustomWindow::OnShowCaptionPanel(bool show)
{
    m_ui->widgetCaptionPanel->setVisible(show);
}

void gui::CustomWindow::OnSwitcherToggled(bool switchedOn)
{
    emit(ChangeWindowStyle(!switchedOn));
    UpdateRestoreMaximizeButtonIcon();
}

void gui::CustomWindow::on_btnRestoreMaximize_clicked()
{
    if(m_parentWindow != NULL)
    {
        if(m_parentWindow->isMaximized())
        {
            emit RestoreWindow();
        }
        else
        {
            emit MaximizeWindow();
        }
    }

    UpdateRestoreMaximizeButtonIcon();
}

void gui::CustomWindow::UpdateRestoreMaximizeButtonIcon()
{
    if(m_parentWindow != NULL)
    {
        QString iconPath = m_parentWindow->isMaximized() ? ":/Resources/Images/restore.png" :
                                                           ":/Resources/Images/maximize.png";
        m_ui->btnRestoreMaximize->setIcon(QIcon(iconPath));
    }
}

void gui::CustomWindow::InstallEventFilter()
{
    m_ui->widgetCaptionPanel->installEventFilter(this);
    m_ui->lblSystemMenu->installEventFilter(this);
}

void gui::CustomWindow::GetCaptionContextMenu(QMenu& contextMenu)
{
    contextMenu.clear();

    contextMenu.addAction(m_restoreAction);
    contextMenu.addAction(m_minimizeAction);
    contextMenu.addAction(m_maximizeAction);
    contextMenu.addAction(m_closeAction);
    contextMenu.insertSeparator(m_closeAction);

    if(m_parentWindow != NULL)
    {
        m_restoreAction->setEnabled(m_parentWindow->isMaximized());
        m_maximizeAction->setEnabled(!m_parentWindow->isMaximized());
    }
}

void gui::CustomWindow::CreateActions()
{
    m_restoreAction = new QAction(QIcon(":/Resources/Images/restore.png"), tr("Restore"), this);
    QObject::connect(m_restoreAction, SIGNAL(triggered()), this, SLOT(on_btnRestoreMaximize_clicked()));

    m_minimizeAction = new QAction(QIcon(":/Resources/Images/minimize.png"), tr("Minimize"), this);
    QObject::connect(m_minimizeAction, SIGNAL(triggered()), this, SIGNAL(MinimizeWindow()));
    QObject::connect(m_ui->btnMinimize, SIGNAL(clicked()), m_minimizeAction, SIGNAL(triggered()));

    m_maximizeAction = new QAction(QIcon(":/Resources/Images/maximize.png"), tr("Maximize"), this);
    QObject::connect(m_maximizeAction, SIGNAL(triggered()), this, SLOT(on_btnRestoreMaximize_clicked()));

    m_closeAction = new QAction(QIcon(":/Resources/Images/close.png"), tr("Close"), this);
    QObject::connect(m_closeAction, SIGNAL(triggered()), this, SIGNAL(CloseWindow()));
    QObject::connect(m_ui->btnClose, SIGNAL(clicked()), m_closeAction, SIGNAL(triggered()));
}

void gui::CustomWindow::ShowCaptionContextMenu(const QPoint& pos)
{
    QPoint globalPos = m_ui->widgetCaptionPanel->mapToGlobal(pos);
    ShowSystemContextMenu(globalPos);
}

void gui::CustomWindow::ShowSystemContextMenu(const QPoint& globalPos)
{
    QMenu contextMenu;

    GetCaptionContextMenu(contextMenu);
    contextMenu.exec(globalPos);
}

bool gui::CustomWindow::eventFilter(QObject* target, QEvent* event)
{
    if (target == m_ui->widgetCaptionPanel)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::MouseButton::LeftButton)
            {
                m_parentWindowGeometry = m_parentWindow->geometry();
                m_leftButtonStartPos = mouseEvent->globalPos();
                m_leftButtonPressed = true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::MouseButton::LeftButton)
            {
                m_leftButtonPressed = false;
            }
        }
        else if (event->type() == QEvent::MouseMove && m_leftButtonPressed == true)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint deltaPos = mouseEvent->globalPos() - m_leftButtonStartPos;

            m_parentWindow->setGeometry(m_parentWindowGeometry.x() + deltaPos.x(),
                                        m_parentWindowGeometry.y() + deltaPos.y(),
                                        m_parentWindowGeometry.width(),
                                        m_parentWindowGeometry.height());
        }
    }

    else if(target == m_ui->lblSystemMenu)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::MouseButton::LeftButton)
            {
                ShowSystemContextMenu(mouseEvent->globalPos());
            }
        }
    }

    return QWidget::eventFilter(target, event);
}
