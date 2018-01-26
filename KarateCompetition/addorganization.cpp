#include "addorganization.h"
#include "ui_addorganization.h"

#include <QMessageBox>

AddOrganization::AddOrganization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrganization),
    m_type(AddOrganization::AddType::ADD_PERSON)
{
    ui->setupUi(this);
}

AddOrganization::~AddOrganization()
{
    delete ui;
}

void AddOrganization::setData(const QString &name, const QString &info, const QString &organization_id)
{
    ui->name->setText(name);
    ui->info->setText(info);
    ui->organization_id->setText(organization_id);
}
void AddOrganization::data(QString &name, QString &info, QString &organization_id)
{
    name = ui->name->text();
    info = ui->info->text();
    organization_id = ui->organization_id->text();
}

void AddOrganization::accept()
{
    bool ok = true;

    if(m_type == AddType::ADD_PERSON)
    {
        ui->organization_id->text().toInt(&ok);
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

void AddOrganization::showEvent(QShowEvent *)
{
    if(m_type == AddType::ADD_PERSON)
    {
        setData("", "", "");
        ui->name->setEnabled(true);
        ui->info->setEnabled(true);
        ui->organization_id->setEnabled(true);
    }
}
void AddOrganization::setType(AddType type)
{
    m_type = type;
}
