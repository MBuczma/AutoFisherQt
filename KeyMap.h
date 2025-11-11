#pragma once

#include <QHash>
#include <QList>
#include <QPair>
#include <QString>
#include <Windows.h>

class KeyMap
{
public:
    static const QList<QPair<QString, WPARAM>> &getOrderedList();
    static const QHash<QString, WPARAM> &getMap();
    static QString getKeyText(WPARAM vkCode);
};
