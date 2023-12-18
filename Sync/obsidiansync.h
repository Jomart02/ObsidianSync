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
    void saveData();
    void openFolder();
    void onStart();
    void onStop();
    void sync();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    void gitInit();
    void gitPull();
    void gitPush();
    void gitMerge();
    void login();
    void toTray();
    void toAutoStart();
    bool isOS;
    bool isStart;
    QTimer *timer;
    Ui::ObsidianSync *ui;

    QString ObsidianPath;
    QString pathToCommand;
    QString commandGitInit = "git init ";
    QString commandDir = "git -C ";
    QString remoteRef;

    /* Объявляем объект будущей иконки приложения для трея */
    QSystemTrayIcon *trayIcon;
};
