#ifndef GETOR_H
#define GETOR_H
#include <QFile>
#include <QMessageBox>
#include <QVariant>
#include <QDateTime>

#include <QtXml/QtXml>
#include <QtXml/QDomDocument>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QList>

#include <QMediaPlayer>
#include <QMediaMetaData>
#include "mydata.h"
#include "player.h"

#define GETOR_H_STARTSETTING "./startsetting.xml"
#define GETOR_H_ISLISTITEM "islistitem"
#define GETOR_H_LISTID "listid"
#define GETOR_H_VOLUME "volume"
#define GETOR_H_MUSICID "musicid"

#define GETOR_H_ALLMUSIC 1
#define GETOR_H_COLMUSIC 2
#define GETOR_H_HISMUSIC 3

#define GETOR_H_LISTDATABASENAME "PlayList"
#define GETOR_H_MUSICDATABASENAME "Music"
#define GETOR_H_RECORDDATABASENAME "Record"
#define GETOR_H_HISTORYDATABASENAME "History"
class Getor{
private:
    int listindex;
    int musicindex;
    bool islistitem;
    int volume;
    QList<ListRecord> playlistrecord;
    QList<MusicRecord> musicrecord;
    QMediaPlayer player;
public:
    Getor();
    void InitDataBase();
    bool GetStartConfig();
    bool CreateSetting();
    bool StoreConfig();//+

    bool GetIsItem();

    void GetAllList();
    void GetAllMusic();

    int GetListIndexById(QString id);
    int GetMusicIndexById(QString id);

    bool AddList(QString name);
    bool RenameList(int index,QString newname);
    bool DeleteList(int index);

    int AddMusic(QString path);
    bool AddRecord(QString musicid,int index);
    bool DeleteMusic(int index);
    QString GetListName(int index);
    int GetListNum();

    int GetMusicNum();
    MusicRecord GetMusicInfo(int index);
    MusicRecord GetMusicById(QString musicid);
    //不限于当前歌单，数据库全局搜索


    PlayerNode GetMusicNode(int index);//
    int GetMusicIndexByNode(PlayerNode node);//


    int GetNowListIndex();
    int GetNowMusicIndex();


    bool SetNowListIndex(bool isitem,int index);
    bool SetNowMusicIndex(int index);

    int GetVolume();
    void SetVolume(int value);

    bool IsCollected(int index);
    bool CollectChange(int index);

    bool AddHistory(QString id);
    bool AddPlayTime(QString musicid);
};

#endif // GETOR_H
