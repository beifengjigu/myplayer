#ifndef MYDATA_H
#define MYDATA_H
#include<QString>
typedef struct{
    QString id;
    QString name;
    QString time;
}ListRecord;
typedef struct{
    QString id;
    QString path;
    QString lyric;
    QString title;
    QString artist;
    QString album;
    qint64 fulltime;
    bool collect;
    int playtime;
}MusicRecord;
#endif // MYDATA_H
