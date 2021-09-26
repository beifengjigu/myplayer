#include "player.h"
#include<iostream>
Player::Player(){
    this->index=0;
    this->status=STOP;
}
int Player::Play(int offset){
    if(this->playlist.size()<=0) return 0;
    switch (offset) {
    case 0:
        if(this->status==STOP){
            this->player.play();
            this->status=ONPLAY;
            return 1;
        }else if(this->status==ONPLAY){
            this->player.pause();
            this->status=STOP;
            return 0;
        }else{
            this->status=STOP;
        }
        break;
    case 1:
        if(this->sequence==1) this->index+=this->GetRand();
        else this->index++;
        this->index=this->index%this->playlist.size();
        this->player.setMedia(QUrl::fromLocalFile(this->playlist.at(this->index).path));
        this->player.play();
        this->status=ONPLAY;
        return 1;
    case -1:
        if(this->sequence==1) this->index-=this->GetRand();
        else this->index--;
        if(this->index<0) this->index=this->index+this->playlist.size();
        this->player.setMedia(QUrl::fromLocalFile(this->playlist.at(this->index).path));
        this->player.play();
        this->status=ONPLAY;
        return 1;
    default:
        break;
    }
    return 0;
}
//int Player::SetPlayList(QStringList thelist){
//    this->playlist.clear();
//    this->playlist=thelist;
//    return this->playlist.length();
//}
qint64 Player::GetCurrent(){
    return this->player.position();
}
qint64 Player::GetFull(){
    return this->player.duration();
}
int Player::SetPositon(qint64 pos){
    this->player.setPosition(pos);
    return 1;
}
int Player::Pause(){
    if(this->status==ONPLAY){
        this->player.pause();
        this->status=STOP;
    }
    return 0;
}
int Player::SetVolume(int value){
    this->player.setVolume(value);
    return 0;
}
int Player::SetIndex(int newindex){
    if(this->playlist.length()<=0) return 0;
    this->index=(newindex+this->playlist.length())%this->playlist.length();
    this->player.setMedia(QUrl::fromLocalFile(this->playlist.at(this->index).path));
    this->status=STOP;
    return 0;
}
int Player::GetIndex(){
    return this->index;
}
bool Player::AddMusic(PlayerNode node){
    this->playlist.append(node);
    if(this->playlist.length()>0) return 1;
    else return 0;
}
bool Player::RemoveMusic(int theindex){
    this->playlist.removeAt(theindex);
    return 1;
}
QIcon Player::GetIcon(){
    return QIcon(QPixmap::fromImage(this->player.metaData(QMediaMetaData::CoverArtImage).value<QImage>()));
}
QString Player::GetTitle(){
    return this->player.metaData(QMediaMetaData::Title).toString();
}
int Player::GetStatus(){
    return this->status;
}
bool Player::IsReady(){
    return this->player.isMetaDataAvailable();
}
void Player::ClearList(){
    this->playlist.clear();
    this->index=0;
    this->status=STOP;
}
PlayerNode Player::GetNode(int theindex){
    return this->playlist.at(theindex);
}
int Player::GetMusicNum(){
    return this->playlist.length();
}
void Player::SetSequence(int thesequence){
    this->sequence=thesequence;
}
int Player::ChangeSequence(){
    switch (this->sequence) {
        case 0:
            this->sequence=1;
            return 1;
        case 1:
            this->sequence=0;
            return 0;
        default:
            return 0;
    }
}
int Player::GetRand(){
    srand(time(NULL));
    return rand()%this->playlist.length();
}
