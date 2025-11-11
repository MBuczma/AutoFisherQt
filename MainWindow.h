#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AutoKeyPresser.h"
#include "KeyMap.h"
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    bool start();
    bool stop();

    void zaktualizujNazwe();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QTimer> keyTimer;
    std::unique_ptr<QTimer> countdownTimer;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;

    HWND handle = NULL;
    HWND parentHandle = NULL;
    QString windowText = "";
    QString parentWindowText = "";
    bool isSending = false;
    int remainingTime = 0;

    void ZlapIdOkna();
    bool isButtonPressedGlobal = false;
    bool isButtonPressed = false;
    void aktualizujStanPrzyciskuStart();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
