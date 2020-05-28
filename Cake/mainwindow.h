#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>

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
    void on_actionRead_cakes_from_file_triggered();
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
    void on_delete();
    void on_edit();
    void on_actionAdd_Cake_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QString m_filename;             // filebol olvasasnal hasznalt valtozo
    QMenu* menu;                    // tablazathoz soraihoz tartozo Context menu
    QStringList table_columns;      // tablazatnak, illetve inkabb a JSON filenak milyen fieldjei vannak egy objecten belul
    QJsonArray json_objects;        // JSON fajlbol beolvasott JSON objektumok tombje
    int selected_row;               // Context Menu hivasakor melyik index-u oszlopon volt a kurzor

    QJsonArray read_json(QString filename);
    void set_table_rows(QJsonArray json_objects, QStringList fields);
    void init_context_menu();
    QJsonObject create_json_object(QString name, QString desc, QString taste, QString color, int price);
    void write_json(QString filename, QJsonArray data);
};
#endif // MAINWINDOW_H
