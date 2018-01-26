#ifndef ADDAGECATEGORY_H
#define ADDAGECATEGORY_H

#include <QDialog>

namespace Ui {
class AddAgeCategory;
}

class AddAgeCategory : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddAgeCategory(QWidget *parent = 0);
    ~AddAgeCategory();

    void setData(const QString &minAge, const QString &maxAge, const QString &age_id);
    void data(QString &minAge, QString &maxAge, QString &age_id);
    void setType(AddType type);
    //void setHours(const QString &hours);
   // void hours(QString &hours);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddAgeCategory *ui;
    AddType m_type;
};

#endif // ADDAGECATEGORY_H
