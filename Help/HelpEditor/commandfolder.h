#ifndef COMMANDFOLDER_H
#define COMMANDFOLDER_H
#include "commandhelp.h"

class CommandFolder
{
public:
    typedef QMap<QTreeWidgetItem*,CommandHelp*> CommandHelpMap;
    typedef QMap<QTreeWidgetItem*,CommandFolder*> CommandFolderMap;
    CommandFolder(const QString &name,QTreeWidgetItem *item);
    CommandFolder(QDataStream &stream,QTreeWidgetItem *item);
    void add(CommandHelp *help);
    QString name()const{return mName;}
    QTreeWidgetItem *item()const{return mItem;}
    QList<CommandHelp*> mHelps;
    QList<CommandFolder*> mFolders;
    void setName(const QString &name);
    bool generateFiles(QString destination,QString folderBase,QString functionBase);
    void save(QDataStream &stream);
    void removeAndDeleteAll(CommandHelpMap &mCommandHelps,CommandFolderMap &mCommandFolders);
    void addAllToMaps(CommandHelpMap &mCommandHelps,CommandFolderMap &mCommandFolders);
private:
    QString mName;
    QTreeWidgetItem *mItem;
};

#endif // COMMANDFOLDER_H
