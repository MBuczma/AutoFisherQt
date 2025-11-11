#include <QApplication>
#include <QStyleFactory>
#include "MainWindow.h"

void ustawStyl(QApplication &application);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ustawStyl(app);
    MainWindow w;
    w.show();
    return app.exec();
}

void ustawStyl(QApplication &application)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = application.palette();
    application.setStyleSheet("QPushButton:hover { background-color: #808080; }"
                              "QComboBox:hover { background-color: #808080; }");
    application.setPalette(palette);
}
