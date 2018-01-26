#include "addchampionship.h"
#include "ui_addchampionship.h"
#include <QMessageBox>

AddChampionship::AddChampionship(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChampionship),
    m_type(AddChampionship::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddChampionship::~AddChampionship()
{
    delete ui;
}

void AddChampionship::setData(const QString &name, const QString &location, const QString &championship_id,
                              const QString &qualification_score, const QString &quater_finals_score,
                              const QString &semifinals_score, const QString &finale_score)
{

    ui->name->setText(name);
    ui->location->setText(location);
    ui->championship_id->setText(championship_id);
    ui->qualification_score->setText(qualification_score);
    ui->quater_finals_score->setText(quater_finals_score);
    ui->semifinals_score->setText(semifinals_score);
    ui->finale_score->setText(finale_score);
}
void AddChampionship::data(QString &name,  QString &location,  QString &championship_id,
                           QString &qualification_score,  QString &quater_finals_score,
                           QString &semifinals_score,  QString &finale_score)
{
    name = ui->name->text();
    location = ui->location->text();
    championship_id = ui->championship_id->text();
    qualification_score = ui->qualification_score->text();
    quater_finals_score = ui->quater_finals_score->text();
    semifinals_score = ui->semifinals_score->text();
    finale_score = ui->finale_score->text();
}

void AddChampionship::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->championship_id->text().toInt(&ok);
        if(!ok)
        {
            QMessageBox::warning(this, tr("Add Item Error"), tr("Id should be a number"));
        }
        else
        {
            QDialog::accept();
        }
    }
}

void AddChampionship::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "", "", "", "", "","");
        ui->name->setEnabled(true);
        ui->location->setEnabled(true);
        ui->championship_id->setEnabled(true);
        ui->qualification_score->setEnabled(true);
        ui->quater_finals_score->setEnabled(true);
        ui->semifinals_score->setEnabled(true);
        ui->finale_score->setEnabled(true);
    }
}
void AddChampionship::setType(AddType type)
{
    m_type = type;
}
