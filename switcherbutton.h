#ifndef SWITCHERBUTTON_H
#define SWITCHERBUTTON_H

#include <QWidget>
#include <QFrame>

namespace Ui {
    class SwitcherButton;
}

namespace gui
{
    class SwitcherButton : public QFrame
    {
        Q_OBJECT

    public:
        explicit SwitcherButton(QWidget *parent = 0);
        ~SwitcherButton();

        void SetColors(QColor switchedOnColor, QColor switchedOffColor);
        void SetSize(const QRect& rect);

    signals:
        void Toggled(bool switchedOn);

    private slots:
        void OnToggled();

    protected:
        virtual void showEvent(QShowEvent* event);
        virtual void resizeEvent(QResizeEvent * event);
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        void Update();
        void Init();
        void InstallEventFilter();
        void ToggleSwitcher();

    private:
        Ui::SwitcherButton* m_ui;
        bool m_switchedOn;
        QColor m_switcherColor;
        QColor m_switchedOnColor;
        QColor m_switchedOffColor;
        QColor m_backgroundColor;
        QRect m_offSwitcherGeometry;
        QRect m_onSwitcherGeometry;
    };
}

#endif // SWITCHERBUTTON_H
