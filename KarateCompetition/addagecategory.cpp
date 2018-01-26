#include "addagecategory.h"
#include "ui_addagecategory.h"
#include <QMessageBox>

AddAgeCategory::AddAgeCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAgeCategory),
    m_type(AddAgeCategory::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddAgeCategory::~AddAgeCategory()
{
    delete ui;
}

void AddAgeCategory::setData(const QString &minAge, const QString &maxAge, const QString &age_id)
{
    ui->minAge->setText(minAge);
    ui->maxAge->setText(maxAge);
    ui->age_id->setText(age_id);
}
void AddAgeCategory::data(QString &minAge, QString &maxAge, QString &age_id)
{
    minAge = ui->minAge->text();
    maxAge = ui->maxAge->text();
    age_id = ui->age_id->text();
}

void AddAgeCategory::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->age_id->text().toInt(&ok);
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

void AddAgeCategory::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "", "");
        ui->age_id->setEnabled(true);
        ui->minAge->setEnabled(true);
        ui->maxAge->setEnabled(true);
    }
}
void AddAgeCategory::setType(AddType type)
{
    m_type = type;
}
