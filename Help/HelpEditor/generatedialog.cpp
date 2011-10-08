#include "generatedialog.h"
#include "ui_generatedialog.h"
#include <QFileDialog>


GenerateDialog::GenerateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateDialog)
{
    ui->setupUi(this);
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelPressed()));
    connect(ui->create,SIGNAL(clicked()),this,SLOT(createPressed()));
    connect(ui->folderBaseOpen,SIGNAL(clicked()),this,SLOT(folderBasePressed()));
    connect(ui->functionBaseOpen,SIGNAL(clicked()),this,SLOT(functionBasePressed()));
    connect(ui->destinationSelect,SIGNAL(clicked()),this,SLOT(destinationFolderSelect()));
}

GenerateDialog::~GenerateDialog()
{
    delete ui;
}

void GenerateDialog::createPressed()
{
    done(QDialog::Accepted);
}

void GenerateDialog::cancelPressed()
{
    done(QDialog::Rejected);
}

QString GenerateDialog::functionBase()
{
    return ui->functionBase->text();
}

QString GenerateDialog::folderBase()
{
    return ui->folderBase->text();
}
QString GenerateDialog::destinationFolder()
{
    return ui->destination->text();
}

void GenerateDialog::folderBasePressed()
{
    QString file = QFileDialog::getOpenFileName(this,"","",tr("Kaikki tiedosto (*.*)"));
    if (!file.isEmpty()) ui->folderBase->setText(file);
}

void GenerateDialog::functionBasePressed()
{
    QString file = QFileDialog::getOpenFileName(this,"","",tr("Kaikki tiedosto (*.*)"));
    if (!file.isEmpty()) ui->functionBase->setText(file);
}

void GenerateDialog::destinationFolderSelect()
{
    QString dir = QFileDialog::getExistingDirectory(this,"","");
    if (!dir.isEmpty()) ui->destination->setText(dir);
}

