#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemdialog.h"
#include "addorganization.h"
#include "addagecategory.h"
#include "addchampionship.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSettings>
#include <QActionGroup>
#include <QItemSelectionModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_tableActions = new QActionGroup(this);
    m_tableActions->addAction(ui->actionTables);
    m_tableActions->addAction(ui->actionOrganizations);
    m_tableActions->addAction(ui->actionChampionship);
    m_tableActions->addAction(ui->actionAge);
    m_tableActions->addAction(ui->actionJoin_Table);
    connect(m_tableActions, &QActionGroup::triggered, this, &MainWindow::onTableActionsTriggered);
    connect(ui->actionAdd_Item, &QAction::triggered, this, &MainWindow::onAddItem);
    connect(ui->actionAdd_Organization, &QAction::triggered, this, &MainWindow::onAddOrganization);
    connect(ui->actionAdd_Championship, &QAction::triggered, this, &MainWindow::onAddChampionship);
    connect(ui->actionAdd_Age_Category, &QAction::triggered, this, &MainWindow::onAddAgeCategory);
    m_addItemDialog = new AddItemDialog(this);
    m_addOrganization = new AddOrganization(this);
    m_addAgeCategory = new AddAgeCategory(this);
    m_addChampionship = new AddChampionship(this);
    QString hostName = "baasu.db.elephantsql.com";
    QString databaseName = "xuiqwkse";
    QString userName = "xuiqwkse";
    QString password = "Ikc7qWDsaBB3S_4n78YUJsfKWBu99VFn";
    readSettings(hostName, databaseName, userName, password);
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(hostName);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(userName);
    m_db.setPassword(password);
    bool ok = m_db.open();
    if(!ok)
    {
        ui->statusBar->showMessage(tr("Database not connected!"));
        cout<<m_db.lastError().databaseText().toStdString();
        cout<< m_db.lastError().driverText().toStdString();
    }
    else
    {
        ui->statusBar->showMessage(tr("Database connected!"));
    }
    setupModel();
    ui->tablePersons->setModel(m_personsModel);
    ui->tableOrganization->setModel(m_organizationModel);
    ui->tableChampionship->setModel(m_championshipModel);
    ui->tableAge->setModel(m_ageModel);
    ui->tableWorkingHours->setModel(m_workingHoursModel);
    ui->tableWorkingHoursJoinPersons->setModel(m_workingHoursJoinPersonsModel);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::onRefreshDB);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings(QString &hostName, QString &databaseName, QString &userName, QString &password)
{
    qDebug() << "App dir path " << qApp->applicationDirPath();
    QSettings s(qApp->applicationDirPath() + "/DBTests.conf", QSettings::IniFormat);
    hostName = s.value("HOSTNAME").toString();
    databaseName = s.value("DATABASENAME").toString();
    userName = s.value("USERNAME").toString();
    password = s.value("PASSWORD").toString();
    qDebug() << "Settings file name " << s.fileName();
}

void MainWindow::onAddItem()
{
    m_addItemDialog->setType(AddItemDialog::AddType::ADD_PERSON);
    int r = m_addItemDialog->exec();
    if(r == QDialog::Accepted)
    {
        QString firstName;
        QString lastName;
        QString id;
        QString age;
        QString weight;
        QString experience;
        m_addItemDialog->data(firstName, lastName, id, age, weight, experience);
        insertQuery(id, firstName, lastName, age, weight, experience);
    }
}

void MainWindow::onAddOrganization()
{
    m_addOrganization->setType(AddOrganization::AddType::ADD_PERSON);
    int r = m_addOrganization->exec();
    if(r == QDialog::Accepted)
    {
        QString name;
        QString info;
        QString organization_id;
        m_addOrganization->data(name, info, organization_id);
        insertQuery(name, info, organization_id);
    }
}
void MainWindow::onAddAgeCategory()
{
    m_addAgeCategory->setType(AddAgeCategory::AddType::ADD_PERSON);
    int r = m_addAgeCategory->exec();
    if(r == QDialog::Accepted)
    {
        QString minAge;
        QString maxAge;
        QString age_id;
        m_addAgeCategory->data(minAge, maxAge, age_id);
        insertQuery1(minAge, maxAge, age_id);
    }
}

void MainWindow::onAddChampionship()
{
    m_addChampionship->setType(AddChampionship::AddType::ADD_PERSON);
    int r = m_addChampionship->exec();
    if(r == QDialog::Accepted)
    {
        QString name;
        QString location;
        QString championship_id;
        QString qualification_score;
        QString quater_finals_score;
        QString semifinals_score;
        QString finale_score;
        m_addChampionship->data(name,  location,  championship_id,
                                qualification_score, quater_finals_score,
                                semifinals_score, finale_score);
        insertQuery(name,  location,  championship_id,
                    qualification_score, quater_finals_score,
                    semifinals_score, finale_score);
    }
}

void MainWindow::onRefreshDB()
{
    selectQuery();
}

void MainWindow::setupModel()
{
    // Adauga Participanti
    ui->tablePersons->setSelectionMode(QAbstractItemView::SingleSelection);
    m_personsModel = new QSqlTableModel(this, m_db);
    m_personsModel->setTable("Participanti");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_personsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_personsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_personsModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m_personsModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m_personsModel->select();


    // Adauga oraganizatii
    ui->tableOrganization->setSelectionMode(QAbstractItemView::SingleSelection);
    m_organizationModel = new QSqlTableModel(this, m_db);
    m_organizationModel->setTable("organization");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_organizationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_organizationModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_organizationModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_organizationModel->setHeaderData(2, Qt::Horizontal, tr("Info"));
    m_organizationModel->select();

    // Adauga categorii varsta
    ui->tableAge->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ageModel = new QSqlTableModel(this, m_db);
    m_ageModel->setTable("age");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_ageModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_ageModel->setHeaderData(0, Qt::Horizontal, tr("Min Age"));
    m_ageModel->setHeaderData(1, Qt::Horizontal, tr("Max Age"));
    m_ageModel->setHeaderData(2, Qt::Horizontal, tr("Age Id"));
    m_ageModel->select();

    // Adauga campionat
    ui->tableChampionship->setSelectionMode(QAbstractItemView::SingleSelection);
    m_championshipModel = new QSqlTableModel(this, m_db);
    m_championshipModel->setTable("championship");
//  m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_championshipModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_championshipModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_championshipModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_championshipModel->setHeaderData(2, Qt::Horizontal, tr("Location"));
    m_championshipModel->setHeaderData(3, Qt::Horizontal, tr("Qualificuation Score"));
    m_championshipModel->setHeaderData(4, Qt::Horizontal, tr("Quaterfinal Score"));
    m_championshipModel->setHeaderData(5, Qt::Horizontal, tr("Semifinal Score"));
    m_championshipModel->setHeaderData(6, Qt::Horizontal, tr("Final Score"));
    m_championshipModel->select();

    m_workingHoursModel = new QSqlRelationalTableModel(this, m_db);
    m_workingHoursModel->setTable("WorkingHours");
    m_workingHoursModel->setRelation(0, QSqlRelation("Persons", "id", "lastname"));
    m_workingHoursModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_workingHoursModel->setHeaderData(0, Qt::Horizontal, tr("Person"));
    m_workingHoursModel->setHeaderData(1, Qt::Horizontal, tr("Hours"));
    m_workingHoursModel->select();

    m_workingHoursJoinPersonsModel = new QSqlQueryModel(this);
    QSqlQuery query("SELECT workinghours.id, persons.firstname, persons.lastname, workinghours.hours \
                       FROM workinghours INNER JOIN persons ON workinghours.id=persons.id");
    m_workingHoursJoinPersonsModel->setQuery(query);
    m_workingHoursJoinPersonsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    m_workingHoursJoinPersonsModel->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m_workingHoursJoinPersonsModel->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m_workingHoursJoinPersonsModel->setHeaderData(3, Qt::Horizontal, tr("Hours"));
}

void MainWindow::insertQuery(const QString &id, const QString &firstName, const QString &lastName,
                             const QString &age, const QString &weight, const QString &experience)
{
    QSqlField idField("participant_id", QVariant::Int);
    QSqlField firstNameField("first_name", QVariant::String);
    QSqlField lastNameField("last_name", QVariant::String);
    QSqlField ageField("age", QVariant::Int);
    QSqlField weightField("weight", QVariant::Int);
    QSqlField experienceField("experience", QVariant::String);
    idField.setValue(id);
    firstNameField.setValue(firstName);
    lastNameField.setValue(lastName);
    ageField.setValue(age);
    weightField.setValue(weight);
    experienceField.setValue(experience);
    QSqlRecord record;
    record.append(idField);
    record.append(firstNameField);
    record.append(lastNameField);
    record.append(ageField);
    record.append(weightField);
    record.append(experienceField);
    m_personsModel->insertRecord(-1, record);
    if(!m_personsModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::insertQuery(const QString &name, const QString &info, const QString &organization_id)
{
    QSqlField idField("organization_id", QVariant::Int);
    QSqlField nameField("name", QVariant::String);
    QSqlField infoField("info", QVariant::String);
    idField.setValue(organization_id);
    nameField.setValue(name);
    infoField.setValue(info);
    QSqlRecord record;
    record.append(idField);
    record.append(nameField);
    record.append(infoField);
    m_organizationModel->insertRecord(-1, record);
    if(!m_organizationModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::insertQuery1(const QString &minAge, const QString &maxAge, const QString &age_id)
{
    QSqlField idField("age_id", QVariant::Int);
    QSqlField minAgeField("min_bound", QVariant::Int);
    QSqlField maxAgeField("max_bound", QVariant::Int);
    idField.setValue(age_id);
    minAgeField.setValue(minAge);
    maxAgeField.setValue(maxAge);
    QSqlRecord record;
    record.append(idField);
    record.append(minAgeField);
    record.append(maxAgeField);
    m_ageModel->insertRecord(-1, record);
    if(!m_ageModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::insertQuery(const QString &name, const QString &location, const QString &championship_id,
                             const QString &qualification_score, const QString &quater_finals_score,
                             const QString &semifinals_score, const QString &finale_score)
{
    QSqlField idField("championship_id", QVariant::Int);
    QSqlField nameField("name", QVariant::String);
    QSqlField locationField("location", QVariant::String);
    QSqlField qualificationScoreField("qualification_score", QVariant::Int);
    QSqlField quaterfinalsScoreField("quater_finals_score", QVariant::Int);
    QSqlField semifinalsScoreField("semifinals_score", QVariant::Int);
    QSqlField finaleScoreField("finale_score", QVariant::Int);
    idField.setValue(championship_id);
    nameField.setValue(name);
    locationField.setValue(location);
    qualificationScoreField.setValue(qualification_score);
    quaterfinalsScoreField.setValue(quater_finals_score);
    semifinalsScoreField.setValue(semifinals_score);
    finaleScoreField.setValue(finale_score);
    QSqlRecord record;
    record.append(idField);
    record.append(nameField);
    record.append(locationField);
    record.append(qualificationScoreField);
    record.append(quaterfinalsScoreField);
    record.append(semifinalsScoreField);
    record.append(finaleScoreField);
    m_championshipModel->insertRecord(-1, record);
    if(!m_championshipModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::insertQuery(const QString &id, const QString &hours)
{
    QSqlField idField("id", QVariant::Int);
    QSqlField hoursField("hours", QVariant::Int);
    idField.setValue(id);
    hoursField.setValue(hours);
    QSqlRecord record;
    record.append(idField);
    record.append(hoursField);
    m_workingHoursModel->insertRecord(-1, record);
    if(!m_workingHoursModel->submitAll())
    {
        ui->statusBar->showMessage(tr("Values not submitted to remote database!"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Values submitted to remote database."));
    }
}

void MainWindow::selectQuery()
{
    m_personsModel->select();
    m_workingHoursModel->select();
    QSqlQuery query("SELECT workinghours.id, persons.firstname, persons.lastname, workinghours.hours \
                       FROM workinghours INNER JOIN persons ON workinghours.id=persons.id");
    m_workingHoursJoinPersonsModel->setQuery(query);
}

void MainWindow::onTableActionsTriggered(QAction *action)
{
    if(action == ui->actionTables)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tablePersons);
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(action == ui->actionOrganizations)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tableWorkingHours);
        ui->stackedWidget->setCurrentIndex(1);
        ui->tableOrganization->selectionModel()->clearSelection();
    }

    if(action == ui->actionJoin_Table)
    {
        ui->stackedWidget->setCurrentIndex(5);
        ui->tablePersons->selectionModel()->clearSelection();
    }

    if(action == ui->actionChampionship)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tableWorkingHours);
        ui->stackedWidget->setCurrentIndex(4);
        ui->tableChampionship->selectionModel()->clearSelection();
    }
    if(action == ui->actionAge)
    {
        //ui->stackedWidget->setCurrentWidget(ui->tableWorkingHours);
        ui->stackedWidget->setCurrentIndex(2);
        ui->tableAge->selectionModel()->clearSelection();
    }
}

void MainWindow::onAddHours()
{
    QItemSelectionModel *selModel = ui->tablePersons->selectionModel();
    QModelIndexList selIndexes = selModel->selectedIndexes();
    if(selIndexes.count() == 0)
    {
        return;
    }
    QModelIndex index = selIndexes[0];
    int row = index.row();
    QString id = m_personsModel->itemData(index.sibling(row, 0))[Qt::EditRole].toString();
    QString firstName = m_personsModel->itemData(index.sibling(row, 1))[Qt::EditRole].toString();
    QString lastName = m_personsModel->itemData(index.sibling(row, 2))[Qt::EditRole].toString();
    QString age = m_personsModel->itemData(index.sibling(row, 3))[Qt::EditRole].toString();
    QString weight = m_personsModel->itemData(index.sibling(row, 4))[Qt::EditRole].toString();
    QString experience = m_personsModel->itemData(index.sibling(row, 5))[Qt::EditRole].toString();

    m_addItemDialog->setType(AddItemDialog::AddType::ADD_HOURS);
    m_addItemDialog->setData(firstName, lastName, id, age, weight, experience);
    int r = m_addItemDialog->exec();
    if(r == QDialog::Rejected)
    {
        return;
    }
//    QString hours;
//    m_addItemDialog->hours(hours);
//    insertQuery(id, hours);
}
