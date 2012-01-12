#include "commandfolder.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QDir>
#include <qalgorithms.h>

CommandFolder::CommandFolder(const QString &name, QTreeWidgetItem *item)
{
    mName = name;
    mItem = item;
    mItem->setText(0,name);
    mParent = 0;
}

CommandFolder::CommandFolder(QDataStream &stream, QTreeWidgetItem *item)
{
    mParent = 0;
    mItem = item;
    stream >> mName;
    mItem->setText(0,mName);
    int helps;
    stream >> helps;
    for (int i = 0; i != helps; i++)
    {
        QTreeWidgetItem *chItem = new QTreeWidgetItem(item);
        CommandHelp *ch = new CommandHelp(stream,chItem);
        ch->setParent(this);
        mHelps.append(ch);
    }
    int folders;
    stream >> folders;
    for (int i = 0; i != folders; i++)
    {
        QTreeWidgetItem *fItem = new QTreeWidgetItem(item);
        CommandFolder *f = new CommandFolder(stream,fItem);
        f->setParent(this);
        mFolders.append(f);
    }
}

void CommandFolder::addAllToMaps(CommandHelpMap &mCommandHelps, CommandFolderMap &mCommandFolders)
{
    for (QList<CommandHelp*>::iterator i = mHelps.begin();i != mHelps.end();i++)
    {
        mCommandHelps[(*i)->item()] = *i;
    }

    for (QList<CommandFolder*>::iterator i = mFolders.begin();i != mFolders.end();i++)
    {
        mCommandFolders[(*i)->item()] = *i;
        (*i)->addAllToMaps(mCommandHelps,mCommandFolders);
    }
}

void CommandFolder::removeAndDeleteAll(CommandHelpMap &mCommandHelps, CommandFolderMap &mCommandFolders)
{
    for (QList<CommandHelp*>::iterator i = mHelps.begin();i != mHelps.end();i++)
    {
        mCommandHelps.remove((*i)->item());
        delete (*i)->item();
        delete *i;
    }

    for (QList<CommandFolder*>::iterator i = mFolders.begin();i != mFolders.end();i++)
    {
        mCommandFolders.remove((*i)->item());
        (*i)->removeAndDeleteAll(mCommandHelps,mCommandFolders);
        delete (*i)->item();
        delete *i;
    }
}

void CommandFolder::setName(const QString &name)
{
    mName = name;
    mItem->setText(0,name);
}

void CommandFolder::add(CommandHelp *help)
{
    mHelps.append(help);
}

void CommandFolder::save(QDataStream &stream)
{
    stream << mName;
    stream << (int)mHelps.count();
    for (QList<CommandHelp*>::iterator i = mHelps.begin();i != mHelps.end();i++)
    {
        (*i)->save(stream);
    }

    stream << (int)mFolders.count();
    for (QList<CommandFolder*>::iterator i = mFolders.begin();i != mFolders.end();i++)
    {
        (*i)->save(stream);
    }
}

bool commandFolderLessThan(const CommandFolder *c1,const CommandFolder *c2)
{
    return c1->name().toLower() < c2->name().toLower();
}

bool commandHelpLessThan(const CommandHelp *c1,const CommandHelp *c2)
{
    return c1->name().toLower() < c2->name().toLower();
}


bool CommandFolder::generateFiles(QString destination, QString folderBase, QString functionBase)
{
    qSort(mFolders.begin(),mFolders.end(),&commandFolderLessThan);
    qSort(mHelps.begin(),mHelps.end(),&commandHelpLessThan);

    CommandFolder *cur = this->parent();
    int stackDepth = 0;
    QString rootReplaceStr;
    while (cur)
    {
        rootReplaceStr += "../";
        stackDepth++;
        cur = cur->parent();
    }
    if (!rootReplaceStr.isEmpty())
    {
        rootReplaceStr.remove(rootReplaceStr.length()-4,4);
    }
    else
    {
        rootReplaceStr = ".";
    }


    QString ownFBase = folderBase;
    ownFBase.replace(QString("$$FOLDERNAME$$"),mName).replace("$$ROOT$$",rootReplaceStr);;
    QDir dir(destination);
    if (!dir.mkpath(destination+"/"+mName))
    {
        QMessageBox::critical(0,"",QString("Ei voida luoda kansiota %1").arg(destination+"/"+mName));
    }
    QFile file(destination+"/"+mName+"/index.html");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0,"",QString("Ei voida avata tiedostoa %1").arg(destination+"/"+mName+".html"));
        return false;
    }
    QTextStream out(&file);
    QStringList parts1 = ownFBase.split("$$FOLDERBEGIN$$");

    if (parts1.count() > 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Useita $$FOLDERBEGIN$$:jä."));
        return false;
    }
    if (parts1.count() < 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Ei $$FOLDERBEGIN$$:ä"));
    }

    QStringList parts2 = parts1.last().split("$$FOLDEREND$$");

    if (parts2.count() > 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Useita $$FOLDEREND$$:jä."));
        return false;
    }
    if (parts2.count() < 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Ei $$FOLDEREND$$:ä"));
    }

    out << parts1.first();
    for (QList<CommandFolder*>::iterator i = mFolders.begin();i != mFolders.end();i++)
    {
        QString childFolderText = parts2.first();
        out << childFolderText.replace("$$CHILDFOLDERNAME$$",(*i)->name()).replace("$$CHILDFOLDERPATH$$",(*i)->name()+"/index.html");
    }

    QStringList parts3 = parts2.last().split("$$FUNCTIONBEGIN$$");
    if (parts3.count() > 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Useita $$FUNCTIONBEGIN$$:jä."));
        return false;
    }
    if (parts3.count() < 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Ei $$FUNCTIONBEGIN$$:ä"));
    }

    out << parts3.first();

    QStringList parts4 = parts3.last().split("$$FUNCTIONEND$$");

    if (parts4.count() > 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Useita $$FUNCTIONEND$$:jä."));
        return false;
    }
    if (parts4.count() < 2)
    {
        QMessageBox::critical(0,"",QString("Kansion sivupohjan syntaksi on viallinen. Ei $$FUNCTIONEND$$:ä"));
    }


    for (QList<CommandHelp*>::iterator i = mHelps.begin();i != mHelps.end();i++)
    {
        QString functionText = parts4.first();
        out << functionText.replace("$$FUNCTIONPATH$$",(*i)->name()+".html").replace("$$FUNCTIONNAME$$",(*i)->name());
    }

    for (QList<CommandHelp*>::iterator i = mHelps.begin();i != mHelps.end();i++)
    {
        if (!(*i)->generateFile(destination+QString("/")+mName,functionBase)) return false;
    }

    for (QList<CommandFolder*>::iterator i = mFolders.begin();i != mFolders.end();i++)
    {
        if (!(*i)->generateFiles(destination+QString("/")+mName,folderBase,functionBase)) return false;
    }

    out << parts4.last();
    file.close();
    return true;
}
