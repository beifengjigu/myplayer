#include "getor.h"
#include<iostream>
Getor::Getor(){
    InitDataBase();
    GetStartConfig();
}
//检查database，如果没有创建则创建
void Getor::InitDataBase(){
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        database=QSqlDatabase::database("qt_sql_default_connection");
    }else{
        database=QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Music.db");
        database.setUserName("beifeng");
        database.setPassword("123456");
        if(!database.open()){
            QMessageBox::critical(nullptr,"无法打开数据库文件：Music.db",database.lastError().databaseText());
            exit(-1);
        }
    }
    //共有三张表，一张存放歌单，一张存放歌曲，一张存放歌曲与歌单的关系
    QSqlQuery querylist;
    querylist.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(GETOR_H_LISTDATABASENAME));
    if(querylist.next()){
        if(querylist.value(0).toInt()==0){
            QSqlQuery sql_query;
            sql_query.exec(QString("create table %1 (listid integer primary key autoincrement,name varchar(40),time varchar(25))").arg(GETOR_H_LISTDATABASENAME));
        }
    }

    QSqlQuery querymusic;
    querymusic.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(GETOR_H_MUSICDATABASENAME));
    if(querymusic.next()){
        if(querymusic.value(0).toInt()==0){
            QSqlQuery sql_query;
            sql_query.exec(QString("create table %1 (musicid integer primary key autoincrement,path varchar(40) unique,lyric varchar(40),title varchar(40),artist varchar(40),album varchar(40),duration integer,collect int,playtime int)").arg(GETOR_H_MUSICDATABASENAME));
        }
    }
    QSqlQuery queryrecord;
    queryrecord.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(GETOR_H_RECORDDATABASENAME));
    if(queryrecord.next()){
        if(queryrecord.value(0).toInt()==0){
            QSqlQuery sql_query;
            sql_query.exec(QString("create table %1 (recordid integer primary key autoincrement,listid integer,musicid integer)").arg(GETOR_H_RECORDDATABASENAME));
            sql_query.exec(QString("create unique index record_index on %1(listid,musicid)").arg(GETOR_H_RECORDDATABASENAME));
        }
    }
    QSqlQuery queryhistory;
    queryhistory.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(GETOR_H_HISTORYDATABASENAME));
    if(queryhistory.next()){
        if(queryhistory.value(0).toInt()==0){
            QSqlQuery sql_query;
            sql_query.exec(QString("create table %1 (historyid integer primary key autoincrement,musicid integer,time varchar(25))").arg(GETOR_H_HISTORYDATABASENAME));
         }
    }
}
//获取启动配置
bool Getor::GetStartConfig(){
    QDomDocument doc;
    QFile file(GETOR_H_STARTSETTING);
    if (!file.exists()){
        CreateSetting();
    }//若不存在
    if (!file.open(QIODevice::ReadOnly)){
            return 0;
    }
    if (!doc.setContent(&file)){
        file.close();
        return 0;
    }
    file.close();
    this->islistitem=(doc.elementsByTagName(GETOR_H_ISLISTITEM).at(0).toElement().text()=="1");
    GetAllList();
    std::cout<<"start:"<<this->islistitem<<std::endl;
    if(this->islistitem==false){
        this->listindex=doc.elementsByTagName(GETOR_H_LISTID).at(0).toElement().text().toInt();
    }else{
        this->listindex=GetListIndexById(doc.elementsByTagName(GETOR_H_LISTID).at(0).toElement().text());
    }
    std::cout<<"start:"<<this->listindex<<std::endl;
    GetAllMusic();
    this->musicindex=GetMusicIndexById(doc.elementsByTagName(GETOR_H_MUSICID).at(0).toElement().text());
    this->volume=doc.elementsByTagName(GETOR_H_VOLUME).at(0).toElement().text().toInt();
    return 1;
}
//创建配置文件
bool Getor::CreateSetting(){
    QDomDocument doc;
    QDomProcessingInstruction instrction;

    instrction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instrction);
    QDomElement root=doc.createElement("startsetting");
    doc.appendChild(root);

    QDomElement eleislistitem=doc.createElement(GETOR_H_ISLISTITEM);
    QDomText text=doc.createTextNode("0");
    root.appendChild(eleislistitem);
    eleislistitem.appendChild(text);

    QDomElement eleplaylist=doc.createElement(GETOR_H_LISTID);
    text=doc.createTextNode(QString::number(GETOR_H_ALLMUSIC));
    root.appendChild(eleplaylist);
    eleplaylist.appendChild(text);

    QDomElement eleindex=doc.createElement(GETOR_H_MUSICID);
    text=doc.createTextNode("0");
    root.appendChild(eleindex);
    eleindex.appendChild(text);

    QDomElement elevolume=doc.createElement(GETOR_H_VOLUME);
    text=doc.createTextNode("50");
    root.appendChild(elevolume);
    elevolume.appendChild(text);

    QFile file(GETOR_H_STARTSETTING);
    if(file.open(QFile::WriteOnly|QFile::Text)){
        QTextStream out_stream(&file);
        doc.save(out_stream,4);
        file.close();
        return true;
    }
    return false;
}
//保存配置
bool Getor::StoreConfig(){
    QDomDocument doc;
    QFile file(GETOR_H_STARTSETTING);
    if (!file.open(QIODevice::ReadOnly)){
        return 0;
    }
    if (!doc.setContent(&file)){
        file.close();
        return 0;
    }
    file.close();
    if(this->islistitem==false){
        doc.elementsByTagName(GETOR_H_ISLISTITEM).at(0).firstChild().setNodeValue("0");
    }else{
        doc.elementsByTagName(GETOR_H_ISLISTITEM).at(0).firstChild().setNodeValue("1");
    }
    if(this->islistitem==false){
        doc.elementsByTagName(GETOR_H_LISTID).at(0).firstChild().setNodeValue(QString::number(this->listindex));
    }else{
        doc.elementsByTagName(GETOR_H_LISTID).at(0).firstChild().setNodeValue(this->playlistrecord.at(this->listindex).id);
    }
    if(this->musicindex==-1){
        doc.elementsByTagName(GETOR_H_MUSICID).at(0).firstChild().setNodeValue("0");
    }else{
        doc.elementsByTagName(GETOR_H_MUSICID).at(0).firstChild().setNodeValue(this->musicrecord.at(this->musicindex).id);
    }
    doc.elementsByTagName(GETOR_H_VOLUME).at(0).firstChild().setNodeValue(QString::number(volume));
    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        return 0;
    }//输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
    return 0;
}

//判断是歌单还是菜单的id
bool Getor::GetIsItem(){
    return this->islistitem;
}

//获取所有歌单
void Getor::GetAllList(){
    this->playlistrecord.clear();
    QSqlQuery query;
    query.exec(QString("select * from %1").arg(GETOR_H_LISTDATABASENAME));
    while(query.next()){
        ListRecord record;
        record.id=query.value(0).toString();
        record.name=query.value(1).toString();
        record.time=query.value(2).toString();
        this->playlistrecord.append(record);
    }
}

//通过id获取歌单的下标
int Getor::GetListIndexById(QString id){
    int i=0;
    while(i<this->playlistrecord.length()){
        if(this->playlistrecord.at(i).id==id){
            return i;
        }
        i++;
    }
    return -1;
}

//添加歌单
bool Getor::AddList(QString name){
    QDateTime datetime=QDateTime::currentDateTime();
    QSqlQuery query;
    QString query_string=QString("insert into %1 values(null,?,?)").arg(GETOR_H_LISTDATABASENAME);
    query.prepare(query_string);
    query.addBindValue(name);
    query.addBindValue(datetime.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    if(query.lastError().type()==QSqlError::NoError){
        GetAllList();//想要获得最新的记录比较麻烦,考虑到歌单内容不至于太多，直接重新读取一遍歌单列表
        return 1;
    }else{
        return 0;
    }
}
//重命名歌单
bool Getor::RenameList(int index,QString newname){
    QSqlQuery query;
    QString query_string=QString("update %1 set name=? where listid=?").arg(GETOR_H_LISTDATABASENAME);
    query.prepare(query_string);
    query.addBindValue(newname);
    query.addBindValue(this->playlistrecord.at(index).id);
    query.exec();
    if(query.lastError().type()!=QSqlError::NoError){
        return false;
    }else{
        return true;
    }
}
//删除歌单
bool Getor::DeleteList(int index){
    QSqlQuery query;
    QString query_string=QString("delete from %1 where listid=?").arg(GETOR_H_LISTDATABASENAME);
    query.prepare(query_string);
    query.addBindValue(this->playlistrecord.at(index).id);
    query.exec();
    if(query.lastError().type()==QSqlError::NoError){
        QSqlQuery query_delete_record;
        QString query_delete_record_str=QString("delete from %1 where listid=?").arg(GETOR_H_RECORDDATABASENAME);
        query_delete_record.prepare(query_delete_record_str);
        query_delete_record.addBindValue(this->playlistrecord.at(index).id);
        query_delete_record.exec();
        if(query_delete_record.lastError().type()==QSqlError::NoError){
            this->playlistrecord.removeAt(index);
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}
//获取list数量
int Getor::GetListNum(){
    return this->playlistrecord.length();
}
//通过下标获取歌单名称
QString Getor::GetListName(int index){
    return this->playlistrecord.at(index).name;
}
//获取listindex
int Getor::GetNowListIndex(){
    return this->listindex;
}
//设置listindex和islistitem
bool Getor::SetNowListIndex(bool isitem,int index){
    this->islistitem=isitem;
    this->listindex=index;
    this->GetAllMusic();
    return true;
}

//通过islistitem 和 listindex获取相应的歌曲
void Getor::GetAllMusic(){
    this->musicrecord.clear();
    if(this->listindex==-1) return ;
    QSqlQuery query;
    if(this->islistitem==false){
        switch (this->listindex) {
            case GETOR_H_ALLMUSIC:
                query.exec(QString("select * from %1 order by musicid desc").arg(GETOR_H_MUSICDATABASENAME));
                break;
            case GETOR_H_COLMUSIC:
                query.exec(QString("select * from %1 where collect=1 order by musicid desc").arg(GETOR_H_MUSICDATABASENAME));
                break;
            case GETOR_H_HISMUSIC:
                query.exec(QString("select * from %1 inner join %2 on %1.musicid=%2.musicid order by historyid desc").arg(GETOR_H_MUSICDATABASENAME).arg(GETOR_H_HISTORYDATABASENAME));
            break;
        }
    }else{
        QString query_string=QString("select * from %1 where musicid in (select musicid from %2 where listid=?) order by musicid desc").arg(GETOR_H_MUSICDATABASENAME).arg(GETOR_H_RECORDDATABASENAME);
        query.prepare(query_string);
        query.addBindValue(this->playlistrecord.at(this->listindex).id);
        query.exec();
    }
    int i=0;//+
    while(query.next()){
        MusicRecord record;
        record.id=query.value(0).toString();
        record.path=query.value(1).toString();
        record.lyric=query.value(2).toString();
        record.title=query.value(3).toString();
        record.artist=query.value(4).toString();
        record.album=query.value(5).toString();
        record.fulltime=query.value(6).toInt();
        record.collect=query.value(7).toBool();
        record.playtime=query.value(8).toInt();
        this->musicrecord.append(record);
        i++;
    }
}
//通过id获取歌曲的下标
int Getor::GetMusicIndexById(QString id){
    int i=0;
    while(i<this->musicrecord.length()){
        if(this->musicrecord.at(i).id==id){
            return i;
        }
        i++;
    }
    return -1;
}
//添加歌曲
int Getor::AddMusic(QString path){
    //必须先检查url，Music中不能重复
    //必须先检查在歌单中是否已存在该歌曲 record
    this->player.setMedia(QUrl::fromLocalFile(path));
    while(!this->player.isMetaDataAvailable()){
        QCoreApplication::processEvents();
    }
    while(player.duration()==0){
        QCoreApplication::processEvents();
    }//不知为何QMediaMetaData::Duration始终为0，只能死马当活马医了
    QSqlQuery query;
    QString query_string=QString("insert into %1 values(null,?,?,?,?,?,?,0,0)").arg(GETOR_H_MUSICDATABASENAME);
    query.prepare(query_string);
    query.addBindValue(path);
    query.addBindValue(path.replace(".mp3",".lrc"));
    query.addBindValue(player.metaData(QMediaMetaData::Title).toString());
    query.addBindValue(player.metaData(QMediaMetaData::ContributingArtist).toStringList().join(","));
    query.addBindValue(player.metaData(QMediaMetaData::AlbumTitle).toString());
    query.addBindValue(player.duration());
    query.exec();
    if(query.lastError().type()!=QSqlError::NoError){
        return -1;
    }else{
        QSqlQuery query_new;
        query_new.exec(QString("select musicid from %1 limit((select count(*) from %2)-1),1").arg(GETOR_H_MUSICDATABASENAME).arg(GETOR_H_MUSICDATABASENAME));
        query_new.next();
        return query_new.value(0).toInt();
    }
}
bool Getor::AddRecord(QString musicid, int index){
    QSqlQuery query;
    QString query_sql=QString("insert into %1 values(null,?,?)").arg(GETOR_H_RECORDDATABASENAME);
    query.prepare(query_sql);
    query.addBindValue(this->playlistrecord.at(index).id);
    query.addBindValue(musicid);
    query.exec();
    if(query.lastError().type()==QSqlError::NoError){
        return true;
    }else return false;
}
//删除歌曲
bool Getor::DeleteMusic(int index){
    QSqlQuery query;
    QString query_sql;
    if(this->islistitem==true){
        query_sql=QString("delete from %1 where listid=? and musicid=?").arg(GETOR_H_RECORDDATABASENAME);
        query.prepare(query_sql);
        query.addBindValue(this->playlistrecord.at(this->listindex).id);
        query.addBindValue(this->musicrecord.at(index).id);
        query.exec();
    }else{
        if(this->listindex==GETOR_H_ALLMUSIC){
            query_sql=QString("delete from %1 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
            query.prepare(query_sql);
            query.addBindValue(this->musicrecord.at(index).id);
            query.exec();
            QString query_delete_record=QString("delete from %1 where musicid=?").arg(GETOR_H_RECORDDATABASENAME);
            query.prepare(query_delete_record);
            query.addBindValue(this->musicrecord.at(index).id);
            query.exec();
        }else if(this->listindex==GETOR_H_COLMUSIC){
            query_sql=QString("update %1 set collect=0 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
            query.prepare(query_sql);
            query.addBindValue(this->musicrecord.at(index).id);
            query.exec();
        }else if(this->listindex==GETOR_H_HISMUSIC){

        }
    }
    if(query.lastError().type()==QSqlError::NoError){
        this->musicrecord.removeAt(index);
        return true;
    }else return false;
}
//设置歌曲下标
bool Getor::SetNowMusicIndex(int index){
    if(this->musicrecord.length()<=index) return false;
    else this->musicindex=index;
    return true;
}
//获取当前歌曲index
int Getor::GetNowMusicIndex(){
//    if(this->this->musicindex==-1)
    return this->musicindex;
}
//获取歌曲数量
int Getor::GetMusicNum(){
    return this->musicrecord.length();
}
//获取歌曲信息
MusicRecord Getor::GetMusicInfo(int index){
    return this->musicrecord.at(index);
    //else return this->musicrecord.at(index);
}
PlayerNode Getor::GetMusicNode(int index){
    PlayerNode node;
    node.path=this->musicrecord.at(index).path;
    node.lyric=this->musicrecord.at(index).lyric;
    node.musicid=this->musicrecord.at(index).id;
    node.islistitem=this->islistitem;
    if(this->islistitem==true){
        node.listid=this->playlistrecord.at(this->listindex).id;
    }else{
        node.listid=QString::number(this->listindex);
    }
    return node;
}
int Getor::GetMusicIndexByNode(PlayerNode node){
    if(this->islistitem==false&&node.islistitem==false){
        if(this->listindex==node.listid.toInt()){
            return this->GetMusicIndexById(node.musicid);
        }else return -1;
    }else if(this->islistitem==true&&node.islistitem==true){
        if(this->playlistrecord.at(this->listindex).id==node.listid){
            return this->GetMusicIndexById(node.musicid);
        }else return -1;
    }else return -1;
//    if(this->islistitem==node.islistitem&&this->playlistrecord.at(this->listindex).id==node.listid){

//    }else return -1;
}
//获取音量
int Getor::GetVolume(){
    return this->volume;
}
//设置音量
void Getor::SetVolume(int value){
    this->volume=value;
}
bool Getor::IsCollected(int index){
    return this->musicrecord.at(index).collect;
}
bool Getor::CollectChange(int index){
    QSqlQuery query;
    QString query_sql;
    if(this->musicrecord.at(index).collect==true){
        query_sql=QString("update %1 set collect=0 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
        query.prepare(query_sql);
        query.addBindValue(this->musicrecord.at(index).id);
        query.exec();
        GetAllMusic();
        return false;
    }else{
        query_sql=QString("update %1 set collect=1 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
        query.prepare(query_sql);
        query.addBindValue(this->musicrecord.at(index).id);
        query.exec();
        GetAllMusic();
        return true;
    }
}
bool Getor::AddHistory(QString id){
    QDateTime datetime=QDateTime::currentDateTime();
    QSqlQuery query;
    QString query_sql=QString("insert into %1 values(null,?,?)").arg(GETOR_H_HISTORYDATABASENAME);
    query.prepare(query_sql);
    query.addBindValue(id);
    query.addBindValue(datetime.toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    if(query.lastError().type()==QSqlError::NoError){
        return this->AddPlayTime(id);
    }else return false;
}
bool Getor::AddPlayTime(QString musicid){
    QSqlQuery query;
    QString query_sql=QString("update %1 set playtime=playtime+1 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
    query.prepare(query_sql);
    query.addBindValue(musicid);
    query.exec();
    if(query.lastError().type()==QSqlError::NoError){
        return true;
    }else return false;
}
MusicRecord Getor::GetMusicById(QString musicid){
    QSqlQuery query;
    QString query_sql=QString("select * from %1 where musicid=?").arg(GETOR_H_MUSICDATABASENAME);
    query.prepare(query_sql);
    query.addBindValue(musicid);
    query.exec();
    query.next();
    MusicRecord record;
    if(query.lastError().type()==QSqlError::NoError){
        record.id=query.value(0).toString();
        record.path=query.value(1).toString();
        record.lyric=query.value(2).toString();
        record.title=query.value(3).toString();
        record.artist=query.value(4).toString();
        record.album=query.value(5).toString();
        record.fulltime=query.value(6).toInt();
        record.collect=query.value(7).toBool();
        record.playtime=query.value(8).toInt();
    }else{
        record.id="null";
        record.path="未知路径";
        record.lyric="";
        record.title="未知标题";
        record.artist="未知艺术家";
        record.album="未知专辑";
        record.fulltime=0;
        record.collect=false;
        record.playtime=0;
    }
    return record;
}
