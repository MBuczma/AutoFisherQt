#include "MainWindow.h"
#include <QMouseEvent>
#include "QTimer"
#include "ui_MainWindow.h"
#include <thread>

static constexpr WPARAM VK_MOUSE_X1_SYN = 0x1001;
static constexpr WPARAM VK_MOUSE_X2_SYN = 0x1002;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isButtonPressed(false)
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &MainWindow::start);
    connect(ui->pushButton_Stop, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->pushButton_PobierzID, &QPushButton::clicked, this, &MainWindow::ZlapIdOkna);

    // Dodaj wszystkie klawisze z mapy KeyMap
    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_Wedka->addItem(pair.first);
    }
    ui->comboBox_Wedka->setCurrentText("Z");

    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_Skill_atak->addItem(pair.first);
    }
    ui->comboBox_Skill_atak->setCurrentText("F9");

    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_Jedzenie->addItem(pair.first);
    }
    ui->comboBox_Jedzenie->setCurrentText("F10");

    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_Swiatlo->addItem(pair.first);
    }
    ui->comboBox_Swiatlo->setCurrentText("2");

    for (const auto &pair : KeyMap::getOrderedList()) {
        ui->comboBox_Skill_dodatkowy->addItem(pair.first);
    }
    ui->comboBox_Skill_dodatkowy->setCurrentText("F4");

    ui->spinBox_Czas_ryby->setValue(48);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ZlapIdOkna()
{
    qDebug() << "[GroupBoxControl] ZlapIdOkna() zostało wywołane.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed == true) {
        isButtonPressed = false;
        autoKeyPresser->WindowHandleFromPoint(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        ui->pushButton_PobierzID->setDown(false);
        aktualizujStanPrzyciskuStart();
    }
    QWidget::mouseReleaseEvent(event); // Dodano wywołanie funkcji bazowej
}

void MainWindow::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    parentWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);

    if (windowText == parentWindowText) {
        ui->groupBox_gra->setTitle(parentWindowText);
    } else {
        ui->groupBox_gra->setTitle(parentWindowText + " " + windowText);
    }
}

void MainWindow::aktualizujStanPrzyciskuStart()
{
    // TODO: implementacja
}

bool MainWindow::start()
{
    QString klawiszWedka = ui->comboBox_Wedka->currentText();
    if (isSending == false) {
        if (klawiszWedka.isEmpty() == false && handle != nullptr) {
            qDebug() << "[GroupBoxControl] wysylanieStart() przycisk =" << klawiszWedka
                     << " uchwyt:" << windowText << "-" << parentWindowText;
            isSending = true;
            aktualizujStanPrzyciskuStart();
            //wyslijKlawisz();
            std::thread([] { Beep(1000, 100); }).detach(); // 1000 Hz przez 100 ms

            int interval = 100;
            remainingTime = interval;

            keyTimer->start(interval);
            countdownTimer->start(100);
            return true;
        }
    } else {
        qDebug() << "[GroupBoxControl][wysylanieStart] Juz wysyła";
    }
    return false;
}

bool MainWindow::stop()
{
    // TODO: implementacja
    return true;
}
