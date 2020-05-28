#include "adddialog.h"
#include "ui_adddialog.h"

#include <QColorDialog>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

QString AddDialog::getName()
{
    return ui->nameLineEdit->text();
}
QString AddDialog::getDesc()
{
    return ui->descriptionLineEdit->text();
}
QString AddDialog::getTaste()
{
    return ui->tasteLineEdit->text();
}
QString AddDialog::getColor()
{
    return ui->colorLineEdit->text();
}
int AddDialog::getPrice()
{
    return ui->priceSpinBox->value();
}
void AddDialog::setName(QString name)
{
    ui->nameLineEdit->setText(name);
}
void AddDialog::setDesc(QString desc)
{
    ui->descriptionLineEdit->setText(desc);
}
void AddDialog::setTaste(QString taste)
{
    ui->tasteLineEdit->setText(taste);
}
void AddDialog::setColor(QString color)
{
    ui->colorLineEdit->setText(color);
    QString background_color = "background-color: "+color;
    ui->colorButton->setStyleSheet(background_color);
}
void AddDialog::setPrice(int price)
{
    ui->priceSpinBox->setValue(price);
}

/* Szin kivalasztasanal beallitja a szomszedos mezot a szin hexa ertekere,
 * es a mellette levo gombot a szinre allitja. */
void AddDialog::on_colorButton_clicked()
{
    QColorDialog dlg ;
    if (dlg.exec() == QDialog::Accepted) {
        ui->colorLineEdit->setText(dlg.selectedColor().name());
        QString background_color = "background-color: rgb(%1, %2, %3);";
        ui->colorButton->setStyleSheet(background_color.arg(dlg.selectedColor().red()).arg(dlg.selectedColor().green()).arg(dlg.selectedColor().blue()));
    }
}

/* Ellenorzi, hogy van e ures mezo a dialog form-jában. */
bool AddDialog::form_is_valid()
{
    if (this->getName().isEmpty()
        || this->getDesc().isEmpty()
        || this->getTaste().isEmpty()
        || this->getColor().isEmpty()
        || this->getPrice() == 0) {
        return false;
    }
    return true;
}
