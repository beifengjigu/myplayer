#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressDialog>
#include <QInputDialog>
#include <QButtonGroup>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include "getor.h"
#include "myqss.h"
#include "player.h"
#include "dialog.h"
#include "lyric.h"


#define MAXVALUE 6000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_allRaBtn_toggled(bool checked);

    void on_colRaBtn_toggled(bool checked);

    void on_hisRaBtn_toggled(bool checked);

    void on_playRaBtn_toggled(bool checked);

    void on_action_triggered();

    void on_addListBtn_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void rename_list();

    void delete_list();

    void collect_music();

    void delete_music();

    void add_record();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_timer_out();

    void on_previousBtn_clicked();

    void on_playBtn_clicked();

    void on_nextBtn_clicked();

    void on_volumeSlider_sliderMoved(int position);

    void on_processSlider_sliderPressed();

    void on_processSlider_sliderReleased();

    void on_processSlider_sliderMoved(int position);

    void on_musicIcon_clicked();

    void on_cdIcon_clicked();

    void on_sequenceBtn_clicked();

private:
    Ui::MainWindow *ui;
    Getor getor;
    Player player;
    Lyric lyric;
    QMenu *listwidgetmenu;
    QMenu *musictablemenu;
    QTimer timer;
    Dialog *select_dialog;
    QStandardItemModel musictextmodel;

    void Init_List_Widget();
    void Set_List_Item();
    void Init_Group_Button();

    void Init_Music_Table();
    void Set_Music_Item();

    void Init_Player_Item();
    void Set_Player_Ele();//设置歌曲相关控件

    QString formatTime(int ms);
};
#endif // MAINWINDOW_H
