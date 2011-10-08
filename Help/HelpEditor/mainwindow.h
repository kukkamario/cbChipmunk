#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "commandhelp.h"
#include "commandfolder.h"

typedef QMap<QTreeWidgetItem*,CommandHelp*> CommandHelpMap;
typedef QMap<QTreeWidgetItem*,CommandFolder*> CommandFolderMap;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool saveTo(const QString &path);
    void setModified(bool t);
    bool mModified;
    QString mSavePath;

    CommandHelpMap mCommandHelps;
    CommandFolderMap mCommandFolders;
    void selectCommandHelp(CommandHelp *help);
    void removeCommandHelp(CommandHelp *help);
    void removeCommandFolder(CommandFolder *folder);
    void enableEdit(bool t);

    bool checkIsSaved();

    bool mIgnoreChangeSignals;
    CommandHelp *mCurrentHelp;

    QString saveDialog();
    CommandFolder *mBaseFolder;
    void clearAll();
    void addNewHelp(QTreeWidgetItem *parent);
    void addNewFolder(QTreeWidgetItem *parent);

    void closeEvent(QCloseEvent *e);

    void setTitle(const QString &fileName);
private slots:
    void saveAs();
    void save();
    void open();
    void newHelp();

    void generate();

    void itemClicked(QModelIndex index);
    void changed();
    void nameChanged(QString name);


    void treePopup(QPoint point);
};

#endif // MAINWINDOW_H
