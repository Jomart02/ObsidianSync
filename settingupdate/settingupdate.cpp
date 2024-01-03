#include "settingupdate.h"

///Пути к файлам
///Надо прописать по другому 

#define PATH QDir::homePath()+"/syncObsidian/folder.ini"
#define PATHREMOTE QDir::homePath()+"/syncObsidian/remote.ini"
#define PATHSTART QDir::homePath()+"/syncObsidian/start.ini"

SettingUpdate::SettingUpdate()
{

}
bool SettingUpdate::Initialization(){
    QString filename = PATH;
    QFile file(filename);;
    if (!file.exists()){
        file.open(QFile::WriteOnly | QFile::Text);
        file.close();
    }else return false;
    return true;
}

bool SettingUpdate::Initialization(TypeSetting type){
    QString filename;
    switch(type){
        case TypeSetting::FOLDER_OBSIDIAN:
            filename = PATH;

        break;
        case TypeSetting::REMOTE:
            filename = PATHREMOTE;
        break;
        case TypeSetting::ISSTART:
            filename = PATHSTART;
        break;
        default:
        break;
    }

    QFile file(filename);;
    if (!file.exists()){
        file.open(QFile::WriteOnly | QFile::Text);
        file.close();
    }else return false;
    return true;
}

QString SettingUpdate::LoadSetting(QString Group, QString Key, TypeSetting type){

    QString filename;
    switch(type){
        case TypeSetting::FOLDER_OBSIDIAN:
            filename = PATH;
        break;
        case TypeSetting::REMOTE:
            filename = PATHREMOTE;
        break;
        case TypeSetting::ISSTART:
            filename = PATHSTART;
        break;
        default:
        break;
    }

    QString Value;
    QSettings sett(filename, QSettings::IniFormat);

    sett.beginGroup(Group);
    Value = sett.value(Key).toString();
    sett.endGroup();
    return Value;
}


QString SettingUpdate::LoadSetting(QString Group, QString Key){
    QString Value;
    QSettings sett(PATH, QSettings::IniFormat);

    sett.beginGroup(Group);
    Value = sett.value(Key).toString();
    sett.endGroup();
    return Value;
}


void SettingUpdate::SaveSetting(QString Group, QString Key, QString Value, TypeSetting type){
    
    QString filename;
    switch(type){
        case TypeSetting::FOLDER_OBSIDIAN:
            filename = PATH;
        break;
        case TypeSetting::REMOTE:
            filename = PATHREMOTE;
        break;
        case TypeSetting::ISSTART:
            filename = PATHSTART;
        break;
        default:
        break;
    }
   
    QSettings sett(filename, QSettings::IniFormat);

    sett.beginGroup(Group);
    sett.setValue(Key, Value);
    sett.endGroup();
}

void SettingUpdate::SaveSetting(QString Group, QString Key, QString Value){
    QSettings sett(PATH, QSettings::IniFormat);

    sett.beginGroup(Group);
    sett.setValue(Key, Value);
    sett.endGroup();
}
