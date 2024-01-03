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
    SettingUpdate();
    
    enum TypeSetting{
        FOLDER_OBSIDIAN,
        REMOTE,
        ISSTART
    };
    /// @brief Метод для сохранения настроек в ini файл
    /// @param Group - секция в которой будет хранится введенная переменная 
    /// @param Key - ключ к переменной
    /// @param Value - значение переменной
    static void SaveSetting(QString Group, QString Key, QString Value);
    /// @brief Метод для сохранения настроек в ini файл - метод специфичный и относится только к текущей задаче
    /// @param Group - секция в которой будет хранится введенная переменная 
    /// @param Key - ключ к переменной
    /// @param Value - значение переменной
    /// @param type - тип вводимой переменной
    static void SaveSetting(QString Group, QString Key, QString Value, TypeSetting type);
    /// @brief Загрузка переменной из ini файла
    /// @param Group - секция в которой хранится переменная 
    /// @param Key - ключ к переменной
    /// @return переменную в строковом формате
    static QString LoadSetting(QString Group, QString Key);
    /// @brief Загрузка переменной из ini файла - метод специфичный и относится только к текущей задаче
    /// @param Group  секция в которой хранится переменная 
    /// @param Key - ключ к переменной
    /// @param type - тип переменной
    /// @return переменную в строковом формате
    static QString LoadSetting(QString Group, QString Key, TypeSetting type);
    /// @brief Создание файла в заданной директории - вызывается один раз
    /// @return 
    static bool Initialization();
    static bool Initialization(TypeSetting type);

};