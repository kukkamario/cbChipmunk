#include "commandhelp.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>

CommandHelp::CommandHelp(QTreeWidgetItem *item)
{
    mItem = item;
}

CommandHelp::CommandHelp(QDataStream &stream, QTreeWidgetItem *item)
{
    stream >> mName;
    stream >> mDescription;
    stream >> mCommand;
    stream >> mParametres;
    stream >> mExamble;
    mItem = item;
    mItem->setText(0,mName);
}

bool CommandHelp::generateFile(QString destination, QString functionBase)
{
    QFile file(destination+"/"+mName+".html");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0,"",QString("Ei voida avata tiedostoa %1").arg(destination+"/"+mName+".html"));
        return false;
    }
    QTextStream out(&file);
    out << functionBase.replace("$$COMMANDNAME$$",mName).replace("$$DESCRIPTION$$",mDescription).replace("$$COMMAND$$",mCommand).replace(
               "$$PARAMETRES$$",mParametres).replace("$$EXAMPLE$$",mExamble);
    file.close();
    return true;
}

void CommandHelp::save(QDataStream &stream)
{
    stream << mName;
    stream << mDescription;
    stream << mCommand;
    stream << mParametres;
    stream << mExamble;
}
