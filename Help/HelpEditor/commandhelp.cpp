#include "commandhelp.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QTextDocument>
#include <QTextBlock>
#include <QFont>
#include <QTextCharFormat>
#include <QTextFragment>
#include <QDebug>
#include <QStringList>
#include <commandfolder.h>

CommandHelp::CommandHelp(QTreeWidgetItem *item)
{
    mItem = item;
    mParent = 0;
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
    mParent = 0;
}

bool CommandHelp::generateFile(QString destination, QString functionBase)
{
    QString parsedDescription = parseTextEditHtml(mDescription);
    QString parsedParametres = parseTextEditHtml(mParametres);
    QString parsedExample = parseTextEditHtml(mExamble);
    QFile file(destination+"/"+mName+".html");
    if (mParent)
    {
        functionBase = functionBase.replace("$$FOLDER$$",mParent->name());
        if (mParent->parent())
        {
            functionBase = functionBase.replace("$$PARENTFOLDER$$",mParent->parent()->name());
        }
        else
        {
            functionBase = functionBase.replace("$$PARENTFOLDER$$","");
        }
    }
    else
    {
        functionBase = functionBase.replace("$$FOLDER$$","").replace("$$PARENTFOLDER$$","");
    }

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0,"",QString("Ei voida avata tiedostoa %1").arg(destination+"/"+mName+".html"));
        return false;
    }
    QTextStream out(&file);
    out << functionBase.replace("$$COMMANDNAME$$",mName).replace("$$DESCRIPTION$$",parsedDescription).replace("$$COMMAND$$",mCommand).replace(
               "$$PARAMETRES$$",parsedParametres).replace("$$EXAMPLE$$",parsedExample);
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

QString CommandHelp::parseTextEditHtml(const QString &html)
{
    QString ret;
    QStringList parts1 = html.split("<body");

    QStringList parts2 = parts1.last().split("</body>");
    ret = "<span"+parts2.first()+"</span>";
    return ret;
    /*
    QString ret;
    QTextDocument doc;
    QTextCharFormat format;
    doc.setHtml(html);
    QTextBlock block = doc.begin();
    while (!block.text().isEmpty())
    {

        for (QTextBlock::iterator i = block.begin();i != block.end();i++)
        {
            QTextFragment fragment = i.fragment();
            if (fragment.isValid())
            {
                format = fragment.charFormat();
                ret += "<span style='font-family:\""+format.fontFamily()+"\";";
                if (format.fontWeight() > QFont::Bold) ret += "font-style:bold;";
                if (format.fontItalic()) ret += "font-style:italic;";
                if (format.fontUnderline()) ret += "font-decoration:underline;";
                QColor color = format.foreground().color();

                ret += "color:#"+QString::number(color.red(),16).rightJustified(2,'0')+QString::number(color.green(),16).rightJustified(2,'0')+QString::number(color.blue(),16).rightJustified(2,'0')+";";
                ret += "' >"+block.text()+"</span>";
            }
            else
            {
                qDebug("Invalid fragment");
            }
        }
        ret += "<br/>";
        block = block.next();
    }
    */
    return ret;
}
