#include "lyric.h"
#include <iostream>
Lyric::Lyric(){
    this->line=0;
    this->nowindex=0;
}
bool Lyric::SetLyricPath(QString path){
    QFile file(path);
    this->lyriclist.clear();
    const QRegExp regexp("\\[(\\d+):(\\d+)\\](.*)");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray strline = file.readLine();
            if(regexp.indexIn(strline,0) != -1) {
                lyricline line;
                line.time=(regexp.cap(1).toInt()*60+regexp.cap(2).toDouble())*1000;
                line.text=regexp.cap(3);
                lyriclist.append(line);
            }
        }
        file.close();
    }
    return true;
}
QStringList Lyric::GetLyric(){
    QStringList thelist;
    int i=0;
    while(i<this->lyriclist.length()){
        thelist.append(this->lyriclist.at(i).text);
        i++;
    }
    return thelist;
}
QString Lyric::GetLyric(int index){
    if(this->nowindex+index<0||this->lyriclist.length()<=0||(this->nowindex+index>(this->lyriclist.length()-1))) return "";
    return this->lyriclist.at(this->nowindex+index).text;
}
void Lyric::SetLine(int num){
    this->line=num;
}
void Lyric::SetNowTime(qint64 time){
    if(time<0||this->lyriclist.length()<=0) return;
    int i=0;
    while(i<this->lyriclist.length()-1){
        if(time>=this->lyriclist.at(i).time&&time<this->lyriclist.at(i+1).time){
            this->nowindex=i;
            return;
        }
        i++;
    }
    this->nowindex=this->lyriclist.length()-1;

}
