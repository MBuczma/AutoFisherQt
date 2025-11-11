#include "MainWindow.h"
#include <QMouseEvent>
#include <QTimer>
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

    // --- INICJALIZACJE: najpierw twórz, potem łącz ---
    autoKeyPresser = std::make_unique<AutoKeyPresser>();
    keyTimer = std::make_unique<QTimer>(this);
    countdownTimer = std::make_unique<QTimer>(this);
    foodTimer = std::make_unique<QTimer>(this);

    connect(ui->pushButton_Start, &QPushButton::clicked, this, &MainWindow::start);
    connect(ui->pushButton_Stop, &QPushButton::clicked, this, &MainWindow::stop);
    connect(ui->pushButton_PobierzID, &QPushButton::clicked, this, &MainWindow::ZlapIdOkna);

    connect(keyTimer.get(), &QTimer::timeout, this, &MainWindow::wyslijKlawisze);
    connect(foodTimer.get(), &QTimer::timeout, this, &MainWindow::wyslijJedzenie);

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

void MainWindow::wyslijKlawisze()
{
    if (sequenceRunning || handle == nullptr)
        return;
    sequenceRunning = true;

    const QString title = ui->groupBox_gra->title();
    const QString wedka = ui->comboBox_Wedka->currentText();
    const QString skillAtak = ui->comboBox_Skill_atak->currentText();

    // 1) klawisz z combo Wędka
    if (!wedka.isEmpty())
        autoKeyPresser->SendKey(handle, wedka, title);

    // 2) czeka 500 ms → LPM
    QTimer::singleShot(500, this, [this, title] {
        autoKeyPresser->SendLeftClick(handle, 0); // 0 bo opóźnienie już było

        // 3) czeka 500 ms → Space
        QTimer::singleShot(500, this, [this, title] {
            autoKeyPresser->SendKey(handle, "Space", title);

            // 4) czeka 100 ms → klawisz z combo SkillAtak
            QTimer::singleShot(100, this, [this, title] {
                const QString skillAtak = ui->comboBox_Skill_atak->currentText();
                if (!skillAtak.isEmpty())
                    autoKeyPresser->SendKey(handle, skillAtak, title);

                // 5) czeka 500 ms i kończy
                QTimer::singleShot(500, this, [this] { sequenceRunning = false; });
            });
        });
    });
}

bool MainWindow::start()
{
    QString klawiszWedka = ui->comboBox_Wedka->currentText();
    if (isSending == false) {
        if (klawiszWedka.isEmpty() == false && handle != nullptr) {
            isSending = true;
            aktualizujStanPrzyciskuStart();

            // 1) natychmiast wyślij "Jedzenie"
            wyslijJedzenie();

            // 2) uruchom niezależny timer dla jedzenia
            int foodIntervalMs = ui->spinBox_Czas_ryby->value()
                                 * 1000; // PODMIEŃ na spinBox dla jedzenia, jeśli masz
            foodTimer->start(qMax(100, foodIntervalMs)); // bezpieczne minimum

            // 3) reszta jak dotąd: pętla łowienia
            wyslijSekwencje();

            std::thread([] { Beep(1000, 100); }).detach();

            int interval = qMax(2000, ui->spinBox_Czas_ryby->value() * 1000);
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
    if (isSending == true) {
        isSending = false;
        aktualizujStanPrzyciskuStart();
        if (foodTimer)
            foodTimer->stop(); // <--- stop niezależnego timera
        std::thread([] { Beep(500, 100); }).detach();
        return true;
    } else {
        return false;
    }
}

void MainWindow::wyslijSekwencje()
{
    if (!isSending || sequenceRunning || handle == nullptr)
        return;
    sequenceRunning = true;

    const QString tytul = ui->groupBox_gra->title();
    const auto sendKeyIfAny = [&](const QString &key) {
        if (!isSending || key.isEmpty() || handle == nullptr)
            return;
        autoKeyPresser->SendKey(handle, key, tytul);
    };

    // 1) Wędka
    const QString wedka = ui->comboBox_Wedka->currentText();
    sendKeyIfAny(wedka);

    // 2) +100 ms: LPM
    QTimer::singleShot(100, this, [this, tytul] {
        if (!isSending || handle == nullptr) {
            sequenceRunning = false;
            return;
        }
        autoKeyPresser->SendLeftClickPost(handle, 0);

        // 3) +500 ms: Space
        QTimer::singleShot(500, this, [this, tytul] {
            if (!isSending || handle == nullptr) {
                sequenceRunning = false;
                return;
            }
            autoKeyPresser->SendKey(handle, "Space", tytul);

            // 4) +100 ms: SkillAtak
            QTimer::singleShot(100, this, [this, tytul] {
                if (!isSending || handle == nullptr) {
                    sequenceRunning = false;
                    return;
                }
                const QString skillAtak = ui->comboBox_Skill_atak->currentText();
                if (!skillAtak.isEmpty())
                    autoKeyPresser->SendKey(handle, skillAtak, tytul);

                // 5) +200 ms: SkillŚwiatło
                QTimer::singleShot(500, this, [this, tytul] {
                    if (!isSending || handle == nullptr) {
                        sequenceRunning = false;
                        return;
                    }
                    const QString swiatlo = ui->comboBox_Swiatlo->currentText();
                    if (!swiatlo.isEmpty())
                        autoKeyPresser->SendKey(handle, swiatlo, tytul);

                    // 6) +200 ms: SkillDodatkowy
                    QTimer::singleShot(500, this, [this, tytul] {
                        if (!isSending || handle == nullptr) {
                            sequenceRunning = false;
                            return;
                        }
                        const QString extra = ui->comboBox_Skill_dodatkowy->currentText();
                        if (!extra.isEmpty())
                            autoKeyPresser->SendKey(handle, extra, tytul);

                        // 7) +100 ms: pętla od nowa
                        QTimer::singleShot(100, this, [this] {
                            sequenceRunning = false;
                            if (isSending) {
                                QTimer::singleShot(0, this, &MainWindow::wyslijSekwencje);
                            }
                        });
                    });
                });
            });
        });
    });
}

void MainWindow::wyslijJedzenie()
{
    if (!isSending || handle == nullptr)
        return;
    const QString title = ui->groupBox_gra->title();
    const QString foodKey = ui->comboBox_Jedzenie->currentText();
    if (!foodKey.isEmpty())
        autoKeyPresser->SendKey(handle, foodKey, title);
}
