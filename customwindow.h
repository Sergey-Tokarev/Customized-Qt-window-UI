#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QWidget>
#include <QAction>

namespace Ui {
class CustomWindow;
}

namespace gui
{
    class CustomWindow : public QWidget
    {
        Q_OBJECT

    public:
        explicit CustomWindow(QWidget *parent = 0);
        ~CustomWindow();

        QLayout* GetContentLayout();
        void SetLayout(QLayout* layout);

    signals:
        void ChangeWindowStyle(bool isOldStyle);
        void RestoreWindow();
        void MinimizeWindow();
        void MaximizeWindow();
        void CloseWindow();

    public slots:
        void OnShowCaptionPanel(bool show);

    private slots:
        void OnSwitcherToggled(bool switchedOn);
        void on_btnRestoreMaximize_clicked();
        void ShowCaptionContextMenu(const QPoint& pos);
        void ShowSystemContextMenu(const QPoint& globalPos);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        void Init();
        void CreateActions();
        void UpdateRestoreMaximizeButtonIcon();
        void InstallEventFilter();
        void GetCaptionContextMenu(QMenu& contextMenu);

    private:
        Ui::CustomWindow* m_ui;
        QWidget* m_parentWindow;
        bool m_isOldStyle;
        bool m_leftButtonPressed;
        QPoint m_leftButtonStartPos;
        QRect m_parentWindowGeometry;

        QAction* m_restoreAction;
        QAction* m_minimizeAction;
        QAction* m_maximizeAction;
        QAction* m_closeAction;
    };
}


#endif // CUSTOMWINDOW_H
