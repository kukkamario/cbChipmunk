#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::setText(const QString &string)
{
    ui->lineEdit->setText(string);
}

QString TextDialog::text()const
{
    return ui->lineEdit->text();
}

void TextDialog::setInfo(const QString &info)
{
    ui->label->setText(info);
}

QString TextDialog::info()const
{
    return ui->label->text();
}
