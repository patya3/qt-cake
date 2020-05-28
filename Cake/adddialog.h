#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();
    QString getName();
    QString getDesc();
    QString getTaste();
    QString getColor();
    int getPrice();

    void setName(QString name);
    void setDesc(QString desc);
    void setTaste(QString taste);
    void setColor(QString color);
    void setPrice(int price);
    bool form_is_valid();


private slots:
    void on_colorButton_clicked();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
