#ifndef ADDORGANIZATION_H
#define ADDORGANIZATION_H


#include <QDialog>

namespace Ui {
class AddOrganization;
}

class AddOrganization : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddOrganization(QWidget *parent = 0);
    ~AddOrganization();

    void setData(const QString &name, const QString &info, const QString &organization_id);
    void data(QString &name, QString &info, QString &organization_id);
    void setType(AddType type);
    //void setHours(const QString &hours);
   // void hours(QString &hours);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddOrganization *ui;
    AddType m_type;
};
#endif // ADDORGANIZATION_H
