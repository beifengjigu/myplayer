#ifndef LYRIC_H
#define LYRIC_H
#include<QString>
#include<QStringList>
#include<QList>
#include<QFile>
typedef struct{
    qint64 time;
    QString text;
} lyricline;
class Lyric{
private:
    int line;
    QList<lyricline> lyriclist;
public:

};

#endif // LYRIC_H
