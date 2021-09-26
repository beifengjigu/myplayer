#ifndef PLAYER_H
#define PLAYER_H
#include<QStringList>
#include<QMediaPlayer>
#include<QMediaMetaData>
#include<QUrl>
#include<QIcon>
#include<QImage>
#include<QCoreApplication>
#include<QList>

#include<cstdlib>
#include<time.h>

#include "mydata.h"

#define ONPLAY 1
#define STOP 0
typedef int Status;

typedef struct{
    QString path;
    QString lyric;
    QString musicid;
    bool islistitem;
    QString listid;
}PlayerNode;
class Player{
private:
    int index;
    Status status;
//    QStringList playlist;
    QList<PlayerNode> playlist;
    QMediaPlayer player;
    int sequence;
public:
    Player();
    int GetIndex();//获取当前的偏移量
    qint64 GetFull();//获取歌曲全部时长
    qint64 GetCurrent();//获取歌曲当前时间
    int Play(int offset);//播放
    int Pause();//暂停
    int SetIndex(int newindex);//设置偏移量
    int SetPositon(qint64 pos);//设置歌曲播放进度
    int SetPlayList(QStringList thelist);
    int SetVolume(int value);

    bool RemoveMusic(int theindex);
    QIcon GetIcon();
    QString GetTitle();
    int GetStatus();
    bool IsReady();
//    MusicInfo GetInfo(QString path);

    bool AddMusic(PlayerNode node);
    void ClearList();//清除记录
    PlayerNode GetNode(int theindex);
    int GetMusicNum();
    void SetSequence(int thesequence=0);//0，顺序播放,1随机播放
    int ChangeSequence();
    int GetRand();
};
#endif // PLAYER_H
