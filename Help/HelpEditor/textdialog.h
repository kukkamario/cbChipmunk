#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
    class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QWidget *parent = 0);
    void setInfo(const QString &info);
    QString info()const;
    void setText(const QString &string);
    QString text()const;
    ~TextDialog();

private:
    Ui::TextDialog *ui;
private slots:
};

#endif // TEXTDIALOG_H
