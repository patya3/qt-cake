#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "adddialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    // tablazat es a JSON fajl oszlopainak megnevezese
    this->table_columns << "name" << "desc" << "taste" << "dominant_color" << "price";
    ui->setupUi(this);

    // tablazat szethuzasa teljes szelessegre
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // tablazat fejlecenek beallitasa
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Desc"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Taste"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Color"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Price"));

    // tablazat feltoltese ertekekkel egy default JSON fajbol    
    this->json_objects = read_json(QDir::currentPath() + "/../resources/cakes.json");
    set_table_rows(this->json_objects, this->table_columns);

    // context menu beallitas, az ablak inicializalasakor
    init_context_menu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* PRIVATE METODUSOK */

/* JSON file beolvasas es konvertalasa QJsonArray tipussa. */
QJsonArray MainWindow::read_json(QString filename)
{
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    return d.array();
}

/* Json file irasa QJsonArray-ben tarolt adatokkal. */
void MainWindow::write_json(QString filename, QJsonArray data)
{
    if (!filename.isEmpty()) {
        QFile jsonFile(filename);
        if (!jsonFile.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), jsonFile.errorString());
            return;
        }
        jsonFile.write(QJsonDocument(data).toJson());
        jsonFile.close();
    }
}

/* QTableWidget feltoltese a JSON fajlbol szarmazo adatokkal. */
void MainWindow::set_table_rows(QJsonArray objects, QStringList fields)
{
    int numrows = objects.count();
    int numcols = fields.count();
    ui->tableWidget->setRowCount(numrows);
    for (int r = 0; r < numrows; r++) {
        for (int c = 0; c < numcols; c++) {
            QJsonValue field_value = objects.at(r).toObject()[fields.at(c)];
            if (field_value.isBool() || field_value.isDouble() || field_value.isNull()) {
                ui->tableWidget->setItem(r,c, new QTableWidgetItem(QString::number(field_value.toInt())));
            } else if (field_value.isString()) {
                ui->tableWidget->setItem(r,c, new QTableWidgetItem(field_value.toString()));
            }
        }
    }
}

/* A tablazat Context Menu-jenek letrehozasa,
 * es az esemenykezelok hozzakapcsolasa az esemenyekhez. */
void MainWindow::init_context_menu()
{
    menu = new QMenu(this);
    QAction* actionDelete = new QAction(tr("Delete"), this);
    QAction* actionEdit = new QAction(tr("Edit"), this);
    menu->addAction(actionDelete);
    menu->addAction(actionEdit);

    connect(actionDelete, &QAction::triggered, this, &MainWindow::on_delete);
    connect(actionEdit, &QAction::triggered, this, &MainWindow::on_edit);
}

/* QJsonObject letrehozas a megadott parameterkbol. */
QJsonObject MainWindow::create_json_object(QString name, QString desc, QString taste, QString color, int price)
{
    QJsonObject new_item;
    new_item.insert("name", QJsonValue::fromVariant(name));
    new_item.insert("desc", QJsonValue::fromVariant(desc));
    new_item.insert("taste", QJsonValue::fromVariant(taste));
    new_item.insert("dominant_color", QJsonValue::fromVariant(color));
    new_item.insert("price", QJsonValue::fromVariant(price));
    return new_item;
}

/* ESEMENYKEZELOK */

/* Fajl beolvasas kezelese, csak JSON fajlok engedelyezese. */
void MainWindow::on_actionRead_cakes_from_file_triggered()
{
    m_filename = QFileDialog::getOpenFileName(
                    this,
                    tr("Select the json file you want to load!"),
                    QDir::currentPath(),
                    tr("Json Files (*.json)")
                ) ;
    if (!m_filename.isEmpty())
    {
         this->json_objects = this->read_json(m_filename);
    }
    set_table_rows(this->json_objects, this->table_columns);
}

/* Context Menu megjelenitese jobb gomb kattintaskor.
 * Kivalasztott sutemeny indexenek eltarolasa. */
void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    this->selected_row = ui->tableWidget->indexAt(pos).row();
    menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

/* Sutemeny torlesenel kitorlom azt a tablzatbol, es a json_objects QJsonArray-bol. */
void MainWindow::on_delete()
{
    this->json_objects.removeAt(this->selected_row);
    ui->tableWidget->removeRow(this->selected_row);
}

/* Sutemeny szerkesztesenel az AddDialog-nak a mezoibe beallitom
 * a szerkeszteni kivant sutemeny ertekeit,
 * majd miutan a felhasznalo szerkesztette azt es a Save gombra kattintott
 * frissitem a tablazt megfelelo sorat, es a json_objects QJsonArray-t is. */
void MainWindow::on_edit()
{
    AddDialog editDlg;
    editDlg.setWindowTitle("Edit Cake");
    editDlg.setName(ui->tableWidget->item(selected_row, 0)->text());
    editDlg.setDesc(ui->tableWidget->item(selected_row, 1)->text());
    editDlg.setTaste(ui->tableWidget->item(selected_row, 2)->text());
    editDlg.setColor(ui->tableWidget->item(selected_row, 3)->text());
    editDlg.setPrice(ui->tableWidget->item(selected_row, 4)->text().toInt());
    if (editDlg.exec() == QDialog::Accepted) {
        if (!editDlg.form_is_valid()) {
            QMessageBox::warning(this, tr("All fields are required"), tr("Cannot save changes!"));
            return;
        }
        ui->tableWidget->item(selected_row, 0)->setText(editDlg.getName());
        ui->tableWidget->item(selected_row, 1)->setText(editDlg.getDesc());
        ui->tableWidget->item(selected_row, 2)->setText(editDlg.getTaste());
        ui->tableWidget->item(selected_row, 3)->setText(editDlg.getColor());
        ui->tableWidget->item(selected_row, 4)->setText(QString::number(editDlg.getPrice()));
        this->json_objects.replace(
            selected_row,
            create_json_object(
                editDlg.getName(),
                editDlg.getDesc(),
                editDlg.getTaste(),
                editDlg.getColor(),
                editDlg.getPrice()
             )
        );
    }
}

/* A Menubar-ban levo Add action-re kattintva, megjelenik az AddDialog,
 * majd az adatokat kitoltve es a Save gombra kattintva,
 * az uj element a tablazathoz adom, valamint hozzadom a json_objects QJsonArray-hez. */
void MainWindow::on_actionAdd_Cake_triggered()
{
    AddDialog addDlg;
    if (addDlg.exec() == QDialog::Accepted) {
        if (!addDlg.form_is_valid()) {
            QMessageBox::warning(this, tr("All fields are required"), tr("Cannot save changes!"));
            return;
        }
        int index = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(index);
        ui->tableWidget->setItem(index, 0, new QTableWidgetItem(addDlg.getName()));
        ui->tableWidget->setItem(index, 1, new QTableWidgetItem(addDlg.getDesc()));
        ui->tableWidget->setItem(index, 2, new QTableWidgetItem(addDlg.getTaste()));
        ui->tableWidget->setItem(index, 3, new QTableWidgetItem(addDlg.getColor()));
        ui->tableWidget->setItem(index, 4, new QTableWidgetItem(QString::number(addDlg.getPrice())));

        this->json_objects.append(
            create_json_object(
                addDlg.getName(),
                addDlg.getDesc(),
                addDlg.getTaste(),
                addDlg.getColor(),
                addDlg.getPrice()
             )
        );
    }
}

/* File/Save Action-re kattintva elmenti a valtoztatasokat a resources/cakes.json fajlba.*/
void MainWindow::on_actionSave_triggered()
{
    write_json(QDir::currentPath() + "/../resources/cakes.json", this->json_objects);
}

/* File/Save As... Action-re kattintve megjelenik egy QFileDialog,
 * es a szerkesztett tetszoleges helyre el lehet menteni. */
void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                    this,
                    tr("Save data to a new file!"),
                    "/../resources/cakes.json",
                    tr("Json Files (*.json)")
                ) ;
   if (filename.isEmpty())
       return;
   else {
       write_json(filename, this->json_objects);
   }
}
