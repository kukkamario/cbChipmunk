#ifndef COMMANDHELP_H
#define COMMANDHELP_H
#include <QString>
#include <QTreeWidgetItem>
#include <QDataStream>
class CommandHelp
{
public:
    CommandHelp(QTreeWidgetItem *item);
    CommandHelp(QDataStream &stream,QTreeWidgetItem *item);

    void setName(const QString &name)
    {
        mName = name;
        mItem->setText(0,name);
    }

    QString name()const{return mName;}
    QString mDescription;
    QString mCommand;
    QString mParametres;
    QString mExamble;
    QTreeWidgetItem *item(){return mItem;}
    bool generateFile(QString destination,QString functionBase);
    void save(QDataStream &stream);
private:
    QTreeWidgetItem *mItem;
    QString mName;
    QString parseTextEditHtml(const QString &html);
};

#endif // COMMANDHELP_H
