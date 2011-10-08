#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

#include <QDialog>

namespace Ui {
    class GenerateDialog;
}

class GenerateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateDialog(QWidget *parent = 0);
    ~GenerateDialog();
    QString functionBase();
    QString folderBase();
    QString destinationFolder();
private:
    Ui::GenerateDialog *ui;

private slots:
    void createPressed();
    void cancelPressed();
    void functionBasePressed();
    void folderBasePressed();
    void destinationFolderSelect();
};


#endif // GENERATEDIALOG_H
