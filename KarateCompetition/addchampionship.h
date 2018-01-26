#ifndef ADDCHAMPIONSHIP_H
#define ADDCHAMPIONSHIP_H


#include <QDialog>

namespace Ui {
class AddChampionship;
}

class AddChampionship : public QDialog
{
    Q_OBJECT

public:
    enum class AddType
    {
        ADD_PERSON,
        ADD_HOURS
    };

public:
    explicit AddChampionship(QWidget *parent = 0);
    ~AddChampionship();

    void setData(const QString &name, const QString &location, const QString &championship_id,
                 const QString &qualification_score, const QString &quater_finals_score, const QString &semifinals_score, const QString &finale_score);
    void data( QString &name,  QString &location,  QString &championship_id,
               QString &qualification_score,  QString &quater_finals_score,  QString &semifinals_score,  QString &finale_score);
    void setType(AddType type);
    //void setHours(const QString &hours);
   // void hours(QString &hours);

protected:
    void accept() override;
    void showEvent(QShowEvent *);

private:
    Ui::AddChampionship *ui;
    AddType m_type;
};

#endif // ADDCHAMPIONSHIP_H
