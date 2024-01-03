#include "obsidiansync.h"
#include "./ui_obsidiansync.h"

ObsidianSync::ObsidianSync(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer()),
    ui(new Ui::ObsidianSync)
{
    ui->setupUi(this);
    
    isStart = 0;//По дефолту всегла 0 при запуске 
    isOS = OS;// Получаем тип ОС


    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(ui->pushButton_path, SIGNAL(clicked()), this, SLOT(openFolder()));
    connect(this,SIGNAL(startTimer(int)),timer,SLOT(start(int)));
    //Инициализируем при первом запуске
    SettingUpdate::Initialization(SettingUpdate::TypeSetting::FOLDER_OBSIDIAN);
    SettingUpdate::Initialization(SettingUpdate::TypeSetting::REMOTE);
    //Запущенно ли приложение - если пк был перезагружен
    isStart = SettingUpdate::LoadSetting("Start","bool",SettingUpdate::TypeSetting::ISSTART).toInt();
    //Получаем данные - если была запущена прога 
    if(isStart){
        ObsidianPath = SettingUpdate::LoadSetting("folder","obsidian",SettingUpdate::TypeSetting::FOLDER_OBSIDIAN);
        remoteRef = SettingUpdate::LoadSetting("Remote","ref",SettingUpdate::TypeSetting::REMOTE);
        pathToCommand = "\"" + ObsidianPath + "\"";
    }
    //Так же если была запущена прога до выключения пк - запускаем по новой
    if(isStart){
        onStart();
    }


}
bool ObsidianSync::getStart(){
    return isStart;
}

ObsidianSync::~ObsidianSync(){
    delete ui;
}
void ObsidianSync::onStart(){
    if(!checkingData()) return;

    saveData();//Сохраняем введенные данные 
    gitInit();//Инициализация гита (один раз)
    
    //Запуск таймера для сонхронизации с гитом 
    connect(timer, SIGNAL(timeout()), this, SLOT(sync()));  
    emit startTimer(60000);
    //emit startTimer(300000);
    //программа запущена 
    SettingUpdate::SaveSetting("Start","bool","1",SettingUpdate::TypeSetting::ISSTART);

    //Для вывода в трей для проверки нужно чтобы переменна запуска перезаписывалась
    if(isStart ){
        isStart = 0;
    }

    toTray();//Вывод в трей 
    isStart=1;
    this->hide();//Сокрытие приложения
    toAutoStart();//Добавление в автозапуск
}

bool ObsidianSync::checkingData(){
    if(remoteRef == "") {
        setError("Data error","Error linking to remote repository");
        return false;
    }
    if(ObsidianPath == "") {
        setError("Data error","Folder path error");
        return false;
    }
    return true;
}

void ObsidianSync::setError(QString title, QString error){
      QMessageBox::warning(this, title,error , QMessageBox::Cancel);
}

void ObsidianSync::sync(){
    gitPull();
    gitPush();
}

void ObsidianSync::onStop(){
    disconnect(timer, SIGNAL(timeout()), this, SLOT(sync()));  
    timer->stop();

    if(isOS){
        QString autostartPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + QLatin1String("/autostart");
        QFile autorunFile(autostartPath + QLatin1String(PATHSETTING));
        // // Удаляем файл автозапуска
        if(autorunFile.exists()) autorunFile.remove();
    }
}

void ObsidianSync::openFolder(){
    QString filename= QFileDialog::getExistingDirectory(this,"Choose Folder");
    if (filename.isEmpty())
        return;
    ui->lineEdit_path->setText(filename);
    ObsidianPath = filename;//Сохраняем в переменную выбранную папку ы
}
//Сохраняем все введенные данные
void ObsidianSync::saveData(){
    SettingUpdate::SaveSetting("folder","obsidian",ObsidianPath,SettingUpdate::TypeSetting::FOLDER_OBSIDIAN);
    remoteRef =  ui->lineEdit_branch->text();
    SettingUpdate::SaveSetting("folder","obsidian",remoteRef,SettingUpdate::TypeSetting::REMOTE);
}

void ObsidianSync::toAutoStart(){

    if(isOS){
        // Путь к папке автозапуска
        QString autostartPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + QLatin1String("/autostart");
        /* Проверяем, существует ли директория, в которой будет храниться файл автозапуска.
        * */
        QDir autorunDir(autostartPath);
        if(!autorunDir.exists()){
            // Если не существует, то создаём
            autorunDir.mkpath(autostartPath);
        }
        QFile autorunFile(autostartPath + QLatin1String(PATHSETTING));
        /* Проверяем состояние чекбокса, если отмечен, то добавляем приложения в автозапуск.
        * В противном случае удаляем
        * */

        // Далее проверяем наличие самого файла автозапуска
        if(!autorunFile.exists()){

            /* Далее открываем файл и записываем в него необходимые данные
            * с указанием пути к исполняемому файлу, с помощью QCoreApplication::applicationFilePath()
            * */
            if(autorunFile.open(QFile::WriteOnly)){

                QString autorunContent("[Desktop Entry]\n"
                                    "Type=Application\n"
                                    "Exec=" + QCoreApplication::applicationFilePath() + "\n"
                                    "Hidden=false\n"
                                    "NoDisplay=false\n"
                                    "X-GNOME-Autostart-enabled=true\n"
                                    "Name[en_GB]=AutorunLinux\n"
                                    "Name=AutorunLinux\n"
                                    "Comment[en_GB]=AutorunLinux\n"
                                    "Comment=AutorunLinux\n");
                QTextStream outStream(&autorunFile);
                outStream << autorunContent;
                // Устанавливаем права доступа, в том числе и на исполнение файла, иначе автозапуск не сработает
                autorunFile.setPermissions(QFileDevice::ExeUser|QFileDevice::ExeOwner|QFileDevice::ExeOther|QFileDevice::ExeGroup|
                                    QFileDevice::WriteUser|QFileDevice::ReadUser);
                autorunFile.close();
            }
        }
    }else{

    }
    
}



//===================================== GIT =====================================

void ObsidianSync::gitInit(){
    pathToCommand = "\"" + ObsidianPath + "\"";
    QDir file(ObsidianPath + "/.git/");
    if (!file.exists()) {
        //Инициализация
        QString commandInit = commandGitInit + pathToCommand ;
        const char* command = commandInit.toUtf8().constData();
        system(command);
        //ignore
        QFile file(ObsidianPath + "workspace.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << ".obsidian/";

        QString remote = commandDir + pathToCommand +  " remote add origin " + remoteRef;
        command = remote.toUtf8().constData();
        system(command);
        
        QString conf = commandDir + pathToCommand + " config --worktree credential.helper store";
        command = remote.toUtf8().constData();
        system(command);

        gitPush();
    }

}

void ObsidianSync::gitPull(){

        gitMerge();

        QString pull = commandDir + pathToCommand  + " pull origin master";
        const char* command = pull.toUtf8().constData();
        system(command);
}

void ObsidianSync::gitPush(){

        gitPull();

        QString add = commandDir + pathToCommand  + " add .";
        const char* command = add.toUtf8().constData();
        system(command);

        QString commit = commandDir + pathToCommand  + " commit -m " +" \" sync  " +  QTime::currentTime().toString("hh:mm:ss") + "\"";
        command = commit.toUtf8().constData();
        system(command);

        QString push = commandDir + pathToCommand  + " push origin master";
        command = push.toUtf8().constData();
        system(command);
       // login();
}

void ObsidianSync::login(){
    

}
void ObsidianSync::gitMerge(){

    QString fetch = commandDir + pathToCommand  + " fetch origin";
    const char* command = fetch.toUtf8().constData();
    system(command);


    QString commit = commandDir + pathToCommand  + " merge origin master" ;
    command = commit.toUtf8().constData();
    system(command);

}
//===================================== GIT =====================================

//==================================== TRAY =====================================


void ObsidianSync::toTray(){
    if(isStart) return;
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Tray Program" "\n"
                         "Работа со сворачиванием программы трей");
    /* После чего создаем контекстное меню из двух пунктов*/
    QMenu * menu = new QMenu(this);
    QAction * viewWindow = new QAction("Развернуть окно", this);
    // Выводим значок...
    menu->addAction(viewWindow);
    /* Устанавливаем контекстное меню на иконку
     * и показываем иконку приложения в трее
     * */
    trayIcon->setContextMenu(menu);
    trayIcon->show();
    /* подключаем сигналы нажатий на пункты меню к соответсвующим слотам.
     * Первый пункт меню разворачивает приложение из трея,
     * а второй пункт меню завершает приложение
     * */
    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));

        /* Также подключаем сигнал нажатия на иконку к обработчику
     * данного нажатия
     * */
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

}


/* Метод, который обрабатывает нажатие на иконку приложения в трее
 * */
void ObsidianSync::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason){
        case QSystemTrayIcon::Trigger:
            /* иначе, если окно видимо, то оно скрывается,
            * и наоборот, если скрыто, то разворачивается на экран
            * */
            if(!this->isVisible() && isStart){
                this->show();
            } else {
                this->hide();
            }

        break;
        default:
        break;
    }
}


/* Метод, который обрабатывает событие закрытия окна приложения
 * */
void ObsidianSync::closeEvent(QCloseEvent * event)
{
    /* Если окно видимо, то завершение приложения
     * игнорируется, а окно просто скрывается, что сопровождается
     * соответствующим всплывающим сообщением
     */
    if(this->isVisible() ){
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
 
    }
}

//==================================== TRAY =====================================