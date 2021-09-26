#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon/musicplayer.svg"));

    ui->groupBox->setStyleSheet("QRadioButton {padding:5px 8px 5px 8px;border-radius:4px;}QRadioButton::checked {background-color: rgb(199, 236, 238);QRadioButton::hover {\
    background-color: rgb(170, 255, 255);}}");
    this->Init_Group_Button();
    this->Init_List_Widget();
    this->Init_Music_Table();
    this->Init_Player_Item();
}
//初始化groupbutton的逻辑控制
void MainWindow::Init_Group_Button(){
    std::cout<<"if:"<<this->getor.GetNowListIndex()<<std::endl;
    if(this->getor.GetIsItem()==false){
        switch (this->getor.GetNowListIndex()) {
            case GETOR_H_ALLMUSIC:
                ui->allRaBtn->setChecked(true);
                break;
            case GETOR_H_COLMUSIC:
                ui->colRaBtn->setChecked(true);
                break;
            case GETOR_H_HISMUSIC:
                ui->hisRaBtn->setChecked(true);
                break;
        }
    }
}

//初始化ListWidget的外观
void MainWindow::Init_List_Widget(){
    this->listwidgetmenu=new QMenu(this);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QAction *renameaction=new QAction(u8"重命名");
    connect(renameaction,&QAction::triggered,this,&MainWindow::rename_list);
    QAction *deleteaction=new QAction(u8"删除");
    connect(deleteaction,&QAction::triggered,this,&MainWindow::delete_list);
    this->listwidgetmenu->addAction(renameaction);
    this->listwidgetmenu->addAction(deleteaction);

    this->Set_List_Item();

}
//设置ListWidget的Item的内容
void MainWindow::Set_List_Item(){
    int i=0;
    while(i<getor.GetListNum()){
        ui->listWidget->addItem(getor.GetListName(i));
        i++;
    }
    if(this->getor.GetIsItem()==true)   ui->listWidget->setCurrentRow(this->getor.GetNowListIndex());
}
//新建歌单
void MainWindow::on_addListBtn_clicked()
{
    bool result;
    QString input=QInputDialog::getText(this,u8"新建歌单","请输入歌单名字:",QLineEdit::Normal,"新建歌单",&result);
    if(result==true){
        this->getor.AddList(input);
        ui->listWidget->addItem(input);
    }
}
//重命名歌单
void MainWindow::rename_list(){
    int itemrow=ui->listWidget->currentRow();
    bool result;
    QString input=QInputDialog::getText(this,u8"重命名歌单","请输入歌单名字:",QLineEdit::Normal,ui->listWidget->currentItem()->text(),&result);
    if(result==true){
        if(!getor.RenameList(itemrow,input)){
            QMessageBox::critical(nullptr,"重命名失败","");
        }else{
            ui->listWidget->currentItem()->setText(input);
        }
    }
}
//删除歌单
void MainWindow::delete_list(){
    int itemrow=ui->listWidget->currentRow();
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "是否删除?", QMessageBox::Yes|QMessageBox::No);
    if (btn == QMessageBox::Yes) {
        if(!this->getor.DeleteList(itemrow)){
            QMessageBox::critical(nullptr,"删除失败","");
        }else{
            ui->listWidget->takeItem(itemrow);
        }
    }
}
//歌单点击更新tableView
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    ui->playRaBtn->setAutoExclusive(false);
    ui->playRaBtn->setChecked(false);
    ui->playRaBtn->setAutoExclusive(true);
    ui->allRaBtn->setAutoExclusive(false);
    ui->allRaBtn->setChecked(false);
    ui->allRaBtn->setAutoExclusive(true);
    ui->colRaBtn->setAutoExclusive(false);
    ui->colRaBtn->setChecked(false);
    ui->colRaBtn->setAutoExclusive(true);
    ui->hisRaBtn->setAutoExclusive(false);
    ui->hisRaBtn->setChecked(false);
    ui->hisRaBtn->setAutoExclusive(true);
    this->getor.SetNowListIndex(true,index.row());
    this->Set_Music_Item();
}
//歌单右键菜单显示
void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    if(ui->listWidget->itemAt(pos)==Q_NULLPTR){ //||ui->listWidget->currentRow()==0
        return ;
    }
    this->listwidgetmenu->exec(QCursor::pos());
}

void MainWindow::Init_Music_Table(){
    this->musictablemenu=new QMenu(this);
    QAction *likeaction=new QAction(u8"我喜欢");
    QAction *addaction=new QAction(u8"收藏到");
    QAction *removeaction=new QAction(u8"从当前列表删除");
    QAction *infoaction=new QAction(u8"详细信息");

    this->musictablemenu->addAction(likeaction);
    connect(likeaction,&QAction::triggered,this,&MainWindow::collect_music);
    this->musictablemenu->addAction(removeaction);
    connect(removeaction,&QAction::triggered,this,&MainWindow::delete_music);
    this->musictablemenu->addAction(addaction);
    connect(addaction,&QAction::triggered,this,&MainWindow::add_record);
    this->musictablemenu->addAction(infoaction);

    this->select_dialog=new Dialog(this);

    //歌曲列表的外观
    ui->tableView->setShowGrid(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->horizontalHeader()->setStyleSheet(HeaderStyle);
    ui->tableView->verticalHeader()->setStyleSheet(HeaderStyle);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableView->setModel(&this->musictextmodel);

    this->Set_Music_Item();

    ui->tableView->selectRow(this->getor.GetNowMusicIndex());

//    this->LoadUrlToPlayer();
//    this->player.SetIndex(this->getor->GetNowMusicIndex());
//    this->Init_Play_Items();
}
void MainWindow::Set_Music_Item(){
    this->musictextmodel.clear();
    this->musictextmodel.setHorizontalHeaderLabels(QStringList()<<"操作"<<"歌曲名"<<"歌手"<<"专辑"<<"时长");
    int i=0;
    while(i<this->getor.GetMusicNum()){
        MusicRecord info=this->getor.GetMusicInfo(i);
        QList<QStandardItem *> itemlist;
        QStandardItem *item=new QStandardItem();
        if(info.collect==false){
            item->setIcon(QIcon(":/icon/heart.svg"));
        }else{
            item->setIcon(QIcon(":/icon/love.svg"));
        }
        itemlist.append(item);
        item=new QStandardItem(info.title);
        itemlist.append(item);
        item=new QStandardItem(info.artist);
        itemlist.append(item);
        item=new QStandardItem(info.album);
        itemlist.append(item);
        item=new QStandardItem(this->formatTime(info.fulltime));
        itemlist.append(item);
        this->musictextmodel.appendRow(itemlist);
        i++;
    }

}
void MainWindow::collect_music(){
    int row=ui->tableView->currentIndex().row();
    this->getor.CollectChange(row);
    this->Set_Music_Item();
}
void MainWindow::delete_music(){
    int row=this->ui->tableView->currentIndex().row();
    this->getor.DeleteMusic(row);
    this->musictextmodel.removeRow(row);
}
void MainWindow::add_record(){
    int i=0;
    this->select_dialog->Reset();
    QStringList listname;
    while(i<this->getor.GetListNum()){
        listname.append(this->getor.GetListName(i));
        i++;
    }
    this->select_dialog->SetItem(listname);
    int ret=this->select_dialog->exec();
    if(ret==QDialog::Accepted){
        this->getor.AddRecord(this->getor.GetMusicInfo(ui->tableView->currentIndex().row()).id,this->select_dialog->GetSelectIndex());
    }
}
QString MainWindow::formatTime(int ms)
{
    int ss = 1000;
    int mi = ss * 60;
    int hh = mi * 60;
    int dd = hh * 24;

    long day = ms / dd;
    long hour = (ms - day * dd) / hh;
    long minute = (ms - day * dd - hour * hh) / mi;
    long second = (ms - day * dd - hour * hh - minute * mi) / ss;
//    long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

    QString hou = QString::number(hour,10);
    QString min = QString::number(minute,10);
    QString sec = QString::number(second,10);
//    QString msec = QString::number(milliSecond,10);

    //qDebug() << "minute:" << min << "second" << sec << "ms" << msec <<endl;
    if(hour!=0) return hou + ":" + min + ":" + sec ;
    else return min + ":" + sec ;
}
MainWindow::~MainWindow()
{
    this->getor.StoreConfig();
    delete ui;
}



void MainWindow::on_allRaBtn_toggled(bool checked)
{
   if(checked==true){
       ui->listWidget->setCurrentRow(-1);
       this->getor.SetNowListIndex(false,GETOR_H_ALLMUSIC);
       this->Set_Music_Item();
       if(this->player.GetMusicNum()>0) ui->tableView->selectRow(this->getor.GetMusicIndexByNode(player.GetNode(this->player.GetIndex())));
   }
}

void MainWindow::on_colRaBtn_toggled(bool checked)
{
    if(checked==true){
        ui->listWidget->setCurrentRow(-1);
        this->getor.SetNowListIndex(false,GETOR_H_COLMUSIC);
        this->Set_Music_Item();
        if(this->player.GetMusicNum()>0) ui->tableView->selectRow(this->getor.GetMusicIndexByNode(player.GetNode(this->player.GetIndex())));
    }
}

void MainWindow::on_hisRaBtn_toggled(bool checked)
{
    if(checked==true){
        ui->listWidget->setCurrentRow(-1);
        this->getor.SetNowListIndex(false,GETOR_H_HISMUSIC);
        this->Set_Music_Item();
        if(this->player.GetMusicNum()>0) ui->tableView->selectRow(this->getor.GetMusicIndexByNode(player.GetNode(this->player.GetIndex())));
    }
}

void MainWindow::on_playRaBtn_toggled(bool checked)
{
    if(checked==true){
        ui->listWidget->setCurrentRow(-1);
        this->musictextmodel.clear();
        this->musictextmodel.setHorizontalHeaderLabels(QStringList()<<"操作"<<"歌曲名"<<"歌手"<<"专辑"<<"时长");
        int i=0;
        while(i<this->player.GetMusicNum()){
            PlayerNode node=this->player.GetNode(i);
            QList<QStandardItem *> itemlist;
            QStandardItem *item=new QStandardItem();
            MusicRecord info=this->getor.GetMusicById(node.musicid);
            if(info.collect==false){
                item->setIcon(QIcon(":/icon/heart.svg"));
            }else{
                item->setIcon(QIcon(":/icon/love.svg"));
            }
            itemlist.append(item);
            item=new QStandardItem(info.title);
            itemlist.append(item);
            item=new QStandardItem(info.artist);
            itemlist.append(item);
            item=new QStandardItem(info.album);
            itemlist.append(item);
            item=new QStandardItem(this->formatTime(info.fulltime));
            itemlist.append(item);
            this->musictextmodel.appendRow(itemlist);
            i++;
        }
    }

//    this->getor.SetNowListIndex(false,GETOR_H_HISMUSIC);
}

//添加歌曲
void MainWindow::on_action_triggered()
{
    QStringList list=QFileDialog::getOpenFileNames(this,
                                                       "选择歌曲",
                                                       "/home",
                                                       "Music (*.mp3)");
    int i=0;
    QProgressDialog processdialog(nullptr,"添加进度",0,list.length());
    processdialog.show();
    if(this->getor.GetIsItem()==false){

    }
    while(i<list.length()){
        this->getor.AddMusic(list.at(i));
        i++;
        processdialog.setValue(i);
    }
    this->getor.GetAllMusic();
    this->Set_Music_Item();
}



void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tableView->rowAt(pos.y())==-1){//如果点击的地方没有元素，则不显示右键菜单
        return ;
    }

    this->musictablemenu->exec(QCursor::pos());
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    this->player.ClearList();
    int i=0;
    while(i<this->getor.GetMusicNum()){
        PlayerNode node=this->getor.GetMusicNode(i);
        this->player.AddMusic(node);
        i++;
    }
    this->player.SetIndex(index.row());
    this->player.Play(0);
    this->Set_Player_Ele();
}
void MainWindow::Init_Player_Item(){
    ui->processSlider->setRange(0,MAXVALUE);
    this->timer.setInterval(60);
    connect(&this->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
    ui->volumeSlider->setValue(this->getor.GetVolume());
    this->player.SetVolume(this->getor.GetVolume());
    int i=0;
    while(i<this->getor.GetMusicNum()){
        PlayerNode node=this->getor.GetMusicNode(i);
        this->player.AddMusic(node);
        i++;
    }
    this->player.SetIndex(0);
    this->Set_Player_Ele();
    this->player.SetSequence();
}
void MainWindow::Set_Player_Ele(){
    if(this->player.GetMusicNum()==0) return ;
    if(this->player.GetStatus()==1){
        ui->playBtn->setStyleSheet("border-image: url(:/icon/pause.svg);");
        this->timer.start();
    }else{
        ui->playBtn->setStyleSheet("border-image: url(:/icon/play.svg);");
        this->timer.stop();
    }
    while(!this->player.IsReady()){
        QCoreApplication::processEvents();
    }
    ui->tableView->selectRow(this->getor.GetMusicIndexByNode(player.GetNode(this->player.GetIndex())));
    QIcon icon=this->player.GetIcon();
    if(icon.isNull()){
         ui->musicIcon->setIcon(QIcon(":/icon/music.svg"));
         ui->cdIcon->setIcon(QIcon(":/icon/music.svg"));
    }else{
         ui->musicIcon->setIcon(icon);
         ui->cdIcon->setIcon(icon);
    }
    ui->musicIcon->setIconSize(QSize(60,60));
    ui->cdIcon->setIconSize(QSize(400,400));
        //setPixmap(QPixmap::fromImage(QImage(":/icon/music.svg").scaled(60,60)));

    this->lyric.SetLyricPath(this->player.GetNode(this->player.GetIndex()).lyric);

    ui->musicTitle->setText(this->player.GetTitle());
    ui->cdTitle->setText(this->player.GetTitle());

    ui->currentLab->setText(this->formatTime((int)this->player.GetCurrent()));
    ui->durationLab->setText(this->formatTime((int)this->player.GetFull()));
    this->getor.SetNowMusicIndex(ui->tableView->currentIndex().row());

    MusicRecord record=this->getor.GetMusicById(this->player.GetNode(this->player.GetIndex()).musicid);
    ui->cdArtistTextLab->setText(record.artist);
    ui->cdAlbumITextLab->setText(record.album);

    this->getor.AddHistory(this->player.GetNode(this->player.GetIndex()).musicid);

}
void MainWindow::on_timer_out(){
    ui->currentLab->setText(this->formatTime((int)this->player.GetCurrent()));
    ui->durationLab->setText(this->formatTime((int)this->player.GetFull()));
    this->lyric.SetNowTime(this->player.GetCurrent());
    ui->label_0->setText(this->lyric.GetLyric(-3));
    ui->label_1->setText(this->lyric.GetLyric(-2));
    ui->label_2->setText(this->lyric.GetLyric(-1));
    ui->label_3->setText(this->lyric.GetLyric(0));
    ui->label_4->setText(this->lyric.GetLyric(1));
    ui->label_5->setText(this->lyric.GetLyric(2));
    ui->label_6->setText(this->lyric.GetLyric(3));
    if(this->player.GetCurrent()==this->player.GetFull()&&this->player.GetFull()!=0){
        this->player.Play(1);
        this->getor.AddHistory(this->player.GetNode(this->player.GetIndex()).musicid);
        this->Set_Player_Ele();
    }
    if(this->player.GetFull()>0) ui->processSlider->setValue(this->player.GetCurrent()*MAXVALUE/this->player.GetFull());
}

void MainWindow::on_previousBtn_clicked()
{
    this->player.Play(-1);
    this->Set_Player_Ele();
}

void MainWindow::on_playBtn_clicked()
{
    this->player.Play(0);
    this->Set_Player_Ele();
}


void MainWindow::on_nextBtn_clicked()
{
    this->player.Play(1);
    this->Set_Player_Ele();
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    this->player.SetVolume(position);
    this->getor.SetVolume(position);
}

void MainWindow::on_processSlider_sliderPressed()
{
    if(this->player.GetStatus()==ONPLAY)  this->player.Pause();
}

void MainWindow::on_processSlider_sliderReleased()
{
    this->player.Play(0);
    this->Set_Player_Ele();
}

void MainWindow::on_processSlider_sliderMoved(int position)
{
    this->player.SetPositon(position*this->player.GetFull()/MAXVALUE);
}

void MainWindow::on_musicIcon_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_cdIcon_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_sequenceBtn_clicked()
{
    switch (this->player.ChangeSequence()) {
        case 0:
         ui->sequenceBtn->setStyleSheet("border-image: url(:/icon/arrow-right.svg);");
         break;
        case 1:
         ui->sequenceBtn->setStyleSheet("border-image: url(:/icon/shuffle.svg);");
         break;
    }
}
