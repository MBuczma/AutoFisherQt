/**
 * @file AutoKeyPresser.h
 * @brief Deklaracja klasy AutoKeyPresser.
 *
 * Klasa odpowiedzialna za obsługę automatycznego wysyłania klawiszy
 * do wybranego okna systemowego. Umożliwia również pobranie uchwytu okna
 * spod kursora i odczytanie jego tytułu.
 */
#pragma once

#include <QString>
#include <Windows.h>

class AutoKeyPresser
{
public:
    explicit AutoKeyPresser();
    ~AutoKeyPresser();

    // Pobiera uchwyt okna na podstawie pozycji kursora
    void WindowHandleFromPoint(HWND &handle, HWND &parentHandle);

    // Pobiera nazwę okna z uchwytu
    QString GetWindowTextFromHandle(const HWND hwnd) const;

    // Wysyła klawisz do podanego okna
    void SendKey(const HWND handle, const QString &key, const QString &name);
};
