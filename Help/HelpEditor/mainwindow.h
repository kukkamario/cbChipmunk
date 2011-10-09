#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "commandhelp.h"
#include "commandfolder.h"
#include <QTextCharFormat>
#include "highlighter.h"
class QFontComboBox;
class QDoubleSpinBox;
class QPushButton;
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

    QFontComboBox *mFontCombo;
    QAction *mBoldButton;
    QAction *mItalicButton;
    QAction *mUnderlineButton;
    QDoubleSpinBox *mFontSize;
    void fontChanged(const QFont &font);
    Highlighter *mHighlighter;
    bool mIgnoreSignals;
private slots:
    void saveAs();
    void save();
    void open();
    void newHelp();

    void generate();

    void setFontBold(bool t);
    void setFontItalic(bool t);
    void setFontUnderline(bool t);
    void setTextEditFont(QFont font);

    void itemClicked(QModelIndex index);
    void changed();
    void nameChanged(QString name);


    void treePopup(QPoint point);

    void currentCharFormatChanged(QTextCharFormat format);
    void setFontPointSize(double s);
};

#endif // MAINWINDOW_H
