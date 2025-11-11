#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "AutoKeyPresser.h"
#include "KeyMap.h"
#include <Windows.h>
#include <memory>

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
    void wyslijKlawisze();
    void zaktualizujNazwe();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QTimer> keyTimer;
    std::unique_ptr<QTimer> countdownTimer;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;
    std::unique_ptr<QTimer> foodTimer; // nowy, niezależny timer

    HWND handle = NULL;
    HWND parentHandle = NULL;
    QString windowText = "";
    QString parentWindowText = "";
    bool isSending = false;
    int remainingTime = 0;
    POINT point;

    void ZlapIdOkna();
    bool isButtonPressedGlobal = false;
    bool isButtonPressed = false;
    void aktualizujStanPrzyciskuStart();
    bool sequenceRunning = false;
    void wyslijSekwencje(); // nowa pętla sekwencji
    void wyslijJedzenie();  // slot wysyłający klawisz jedzenia

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};
#endif // MAINWINDOW_H
