#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QList>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void Reset();
    void SetItem(QStringList listname);
    int GetSelectIndex();
    ~Dialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    int btnnum;
    Ui::Dialog *ui;
    QList<QPushButton *> btnlist;
};

#endif // DIALOG_H
