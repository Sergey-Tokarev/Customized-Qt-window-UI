#ifndef TRANSPARENTMAINWINDOW_H
#define TRANSPARENTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class TransparentMainWindow;
}

namespace gui
{
    class TransparentMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit TransparentMainWindow(QWidget *parent = 0);
        ~TransparentMainWindow();

    protected:
        virtual void showEvent(QShowEvent* event);

    private:
        void InitCustomWindow();
        QWidget* CreateCustomWindow(QWidget* parent = 0);

    signals:
        void ShowCaptionPanel(bool show);

    public slots:
        void OnWindowStyleChanged(bool isOldStyle);

    private:
        Ui::TransparentMainWindow* m_ui;
        Qt::WindowFlags m_oldFashionWindowFlags;
    };
}

#endif // TRANSPARENTMAINWINDOW_H
