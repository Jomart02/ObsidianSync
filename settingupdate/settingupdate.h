#pragma once
#include <QSettings>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QString>
#include <string>
#include <vector>
#include <QWidget>


class SettingUpdate
{
public:

    enum TypeSetting{
        FOLDER_OBSIDIAN,
        REMOTE,
        ISSTART
    };

    static void SaveSetting(QString Group, QString Key, QString Value);
    static void SaveSetting(QString Group, QString Key, QString Value, TypeSetting type);
    static QString LoadSetting(QString Group, QString Key);
    static QString LoadSetting(QString Group, QString Key, TypeSetting type);
    static bool Initialization();
    static bool Initialization(TypeSetting type);
    SettingUpdate();
};