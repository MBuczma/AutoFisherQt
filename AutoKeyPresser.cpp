/**
 * @file AutoKeyPresser.cpp
 * @brief Implementacja klasy AutoKeyPresser.
 *
 * Zawiera funkcje do:
 * - Pobierania uchwytu okna spod kursora
 * - Odczytywania tytułu okna
 * - Wysyłania pojedynczych klawiszy do okna
 */
#include "AutoKeyPresser.h"

#include <QDebug>
#include <KeyMap.h>

AutoKeyPresser::AutoKeyPresser() {}
AutoKeyPresser::~AutoKeyPresser() {}

void AutoKeyPresser::WindowHandleFromPoint(HWND &handle, HWND &parentHandle)
{
    POINT point;
    GetCursorPos(&point); // Pobranie współrzędnych kursora w odniesieniu do ekranu
    handle = WindowFromPoint(point);
    parentHandle = GetAncestor(handle, GA_ROOT);

    if (handle != nullptr) {
        //qDebug() << "X:" << point.x << "Y:" << point.y;
        qDebug() << "[AutoKeyPresser][WindowHandleFromPoint] Handle:" << handle
                 << "- Tekst:" << GetWindowTextFromHandle(handle)
                 << " ParentHandle:" << parentHandle
                 << "- Tekst:" << GetWindowTextFromHandle(parentHandle) << Qt::endl;
    } else {
        qDebug() << "[AutoKeyPresser][WindowHandleFromPoint] Nie udało się pobrać uchwytu okna.";
    }
}

QString AutoKeyPresser::GetWindowTextFromHandle(const HWND hwnd) const
{
    // Bufor na przechowanie tekstu okna
    wchar_t windowText[256]; // Maksymalna długość tekstu to 256 znaków
    int length = GetWindowTextW(hwnd, windowText, sizeof(windowText) / sizeof(wchar_t));

    if (length > 0) {
        // Zamieniamy tekst z wide-char (wchar_t) na QString
        return QString::fromWCharArray(windowText, length);
    } else {
        qDebug()
            << "[AutoKeyPresser][GetWindowTextFromHandle] Nie udało się pobrać tekstu dla uchwytu:"
            << hwnd;
        return QString(); // Zwraca pusty QString
    }
}

void AutoKeyPresser::SendKey(const HWND handle, const QString &key, const QString &name)
{
    const auto &keyMap = KeyMap::getMap();
    QString keyUpper = key.toUpper();

    auto it = keyMap.find(keyUpper);
    if (it != keyMap.end()) {
        WPARAM keyCode = it.value();
        PostMessage(handle, WM_KEYDOWN, keyCode, 0);
        PostMessage(handle, WM_KEYUP, keyCode, 0);
        qDebug() << "[AutoKeyPresser][SendKey] Wysyłam klawisz:" << key << "do uchwytu:" << name;
    } else {
        qDebug() << "[AutoKeyPresser][SendKey] Nieznany klawisz:" << key
                 << "(po przekształceniu:" << keyUpper << ")";
    }
}
