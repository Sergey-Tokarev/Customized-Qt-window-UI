#include <QMouseEvent>
#include <QPropertyAnimation>
#include "switcherbutton.h"
#include "ui_switcherbutton.h"

gui::SwitcherButton::SwitcherButton(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::SwitcherButton),
    m_switchedOn(false),
    m_switcherColor(QColor(255 ,255, 255)),
    m_switchedOnColor(QColor(146 ,220, 92)),
    m_switchedOffColor(QColor(230, 222, 211))
{
    m_ui->setupUi(this);
    InstallEventFilter();
}

gui::SwitcherButton::~SwitcherButton()
{
    delete m_ui;
}

void gui::SwitcherButton::SetColors(QColor switchedOnColor, QColor switchedOffColor)
{
    m_switchedOnColor = switchedOnColor;
    m_switchedOffColor = switchedOffColor;

    Update();
}

void gui::SwitcherButton::SetSize(const QRect& rect)
{
    int normilizedHeight = (rect.height() / 4) * 4;
    if(normilizedHeight * 2 > (rect.width() / 4) * 4)
    {
        normilizedHeight = rect.width() / 2;
    }
    QRect normilizedRect(QPoint(rect.x(), rect.y()), QSize(normilizedHeight * 2, normilizedHeight));

    const int minPaddingSize = 4;
    const qreal paddingPersent = 0.05;
    int paddingSize = qMax(minPaddingSize, static_cast<int>(static_cast<qreal>(normilizedHeight) * paddingPersent));
    int switcherHeight = normilizedHeight - (paddingSize * 2);

    m_offSwitcherGeometry.setRect(paddingSize, paddingSize, switcherHeight, switcherHeight);
    m_onSwitcherGeometry.setRect(3 * paddingSize + switcherHeight, paddingSize, switcherHeight, switcherHeight);

    this->setMinimumSize(normilizedRect.size());

    this->setGeometry(normilizedRect);
    m_ui->switcher->setGeometry(m_switchedOn ? m_onSwitcherGeometry : m_offSwitcherGeometry);

    int leftLabelHeight = m_ui->lblOn->frameSize().height();
    int leftLabelWidth = m_ui->lblOn->frameSize().width();
    int leftLabelX = (normilizedHeight - leftLabelHeight) / 2;
    int leftLabelY = (normilizedHeight - leftLabelWidth) / 2;
    m_ui->lblOn->setGeometry(QRect(leftLabelX, leftLabelY, leftLabelWidth, leftLabelHeight));

    int rightLabelHeight = m_ui->lblOff->frameSize().height();
    int rightLabelWidth = m_ui->lblOff->frameSize().width();
    int rightLabelX = normilizedHeight + (normilizedHeight - rightLabelHeight) / 2;
    int rightLabelY = (normilizedHeight - rightLabelWidth) / 2;
    m_ui->lblOff->setGeometry(QRect(rightLabelX, rightLabelY, rightLabelWidth, rightLabelHeight));

    Update();
}

void gui::SwitcherButton::OnToggled()
{
    Update();
    emit Toggled(m_switchedOn);
}

void gui::SwitcherButton::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
    Init();
}

void gui::SwitcherButton::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    SetSize(this->frameRect());
}

bool gui::SwitcherButton::eventFilter(QObject *target, QEvent *event)
{
    if(target == m_ui->switcher || target == m_ui->lblOff || target == m_ui->lblOn)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::MouseButton::LeftButton)
            {
                m_switchedOn = !m_switchedOn;
                ToggleSwitcher();
            }
        }
    }

    return QFrame::eventFilter(target, event);
}

void gui::SwitcherButton::Init()
{
    static bool s_wasShown = false;

    if (!s_wasShown)
    {
        m_backgroundColor = m_switchedOn ? m_switchedOnColor : m_switchedOffColor;
        SetSize(this->frameRect());
        s_wasShown = true;
    }
}

void gui::SwitcherButton::InstallEventFilter()
{
    m_ui->switcher->installEventFilter(this);
    m_ui->lblOff->installEventFilter(this);
    m_ui->lblOn->installEventFilter(this);
}

void gui::SwitcherButton::ToggleSwitcher()
{
    QPropertyAnimation* switcherAnimation = new QPropertyAnimation(m_ui->switcher, "geometry");
    QObject::connect(switcherAnimation, SIGNAL(finished()), this, SLOT(OnToggled()));

    switcherAnimation->setDuration(1000);
    switcherAnimation->setEasingCurve(QEasingCurve::Linear);
    switcherAnimation->setEndValue(m_switchedOn ? m_onSwitcherGeometry : m_offSwitcherGeometry);
    switcherAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void gui::SwitcherButton::Update()
{
    m_backgroundColor = m_switchedOn ? m_switchedOnColor : m_switchedOffColor;

    QString switcherButtonStyleSheet = QString("QFrame{background-color: rgb(%1,%2,%3); border: 1px solid transparent; border-radius: %4px;}").
            arg(QString::number(m_backgroundColor.red()),
                QString::number(m_backgroundColor.green()),
                QString::number(m_backgroundColor.blue()),
                QString::number(this->frameRect().height() >> 1));

    this->setStyleSheet(switcherButtonStyleSheet);

    QString switcherStyleSheet = QString("QFrame{background-color: rgb(%1,%2,%3); border: 1px solid transparent; border-radius: %4px;}").
            arg(QString::number(m_switcherColor.red()),
                QString::number(m_switcherColor.green()),
                QString::number(m_switcherColor.blue()),
                QString::number(m_ui->switcher->frameRect().height() >> 1));

    m_ui->switcher->setStyleSheet(switcherStyleSheet);
}



