#pragma once
#include <QWidget>
#include <QFileDialog>
#include <QProcess>
#include <QTimer>
#include "settingupdate.h"
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QStyle>
#include <QMenu>
#include <QTextStream>
#include <QStandardPaths>
#include <QMessageBox>


#ifdef Q_OS_WIN32
        // QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        // settings.setValue(APPLICATION_NAME, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        // settings.sync();
        #define PATHSETTING "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
        #define OS 0;
#else
        #define PATHSETTING "/AutorunLinux.desktop"
        #define OS 1;

#endif


QT_BEGIN_NAMESPACE
namespace Ui { class ObsidianSync; }
QT_END_NAMESPACE

class ObsidianSync : public QWidget
{
    Q_OBJECT
signals:
	void stopTimer();
	void startTimer(int msec);
public:
    ObsidianSync(QWidget *parent = nullptr);
    ~ObsidianSync();
    bool getStart();
protected:
    /* Виртуальная функция родительского класса в нашем классе
     * переопределяется для изменения поведения приложения,
     *  чтобы оно сворачивалось в трей, когда мы этого хотим
     */
    void closeEvent(QCloseEvent * event);
private slots:
    /// @brief сохранение настроек
    void saveData();
    /// @brief Открытие выбора папки 
    void openFolder();
    /// @brief нажатие кнопки старт 
    void onStart();
    /// @brief Нажатие кнопки стоп 
    void onStop();
    /// @brief Синхронизация программы
    void sync();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    /// @brief Инициализация гита
    void gitInit();
    /// @brief pull гита
    void gitPull();
    /// @brief push гита
    void gitPush();
    /// @brief merge гита
    void gitMerge();
    /// @brief Логин в гите - не нужен так как идет автологин 
    void login();
    /// @brief Вывод программы в трей 
    void toTray();
    /// @brief Добавление программы в автозапуск 
    void toAutoStart();
    /// @brief Провнрка введенных данных 
    /// @return t/f если не было исключений
    bool checkingData();
    void setError(QString title, QString error);
    /// @brief Тип ос 0 - винда, 1 - линукс
    bool isOS;
    /// @brief Запущенна ли прога 
    bool isStart;
    QTimer *timer;
    Ui::ObsidianSync *ui;


    /// Дефолтные программы 

    QString ObsidianPath;
    QString pathToCommand;
    QString commandGitInit = "git init ";
    QString commandDir = "git -C ";
    QString remoteRef;

    /* Объявляем объект будущей иконки приложения для трея */
    QSystemTrayIcon *trayIcon;
};
