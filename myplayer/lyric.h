#ifndef LYRIC_H
#define LYRIC_H
#include<QString>
#include<QStringList>
#include<QFile>
typedef struct{
    qint64 time;
    QString text;
} lyricline;
class Lyric{
private:
    int line;
    int nowindex;
    QList<lyricline> lyriclist;
public:
    Lyric();
    bool SetLyricPath(QString path);
    QString GetLyric(int index);//获取当前
    QStringList GetLyric();
    void SetLine(int num);//设置要显示几行

    bool SetLineNum(int num);
    void SetNowTime(qint64 nowtime);
    int GetNowIndex();
    qint64 GetTime(int index);
};

#endif // LYRIC_H
