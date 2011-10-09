#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QCloseEvent>
#include "generatedialog.h"
#include <QTextStream>
#include <QStack>
#include "textdialog.h"
#include <QFontComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#define MAGIC_NUM 41221

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mModified = false;
    mIgnoreSignals = false;
    mCurrentHelp = 0;
    mHighlighter = new Highlighter(ui->exampleTextEdit->document());

    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(treePopup(QPoint)));
    connect(ui->treeWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    connect(ui->nameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(ui->exampleTextEdit,SIGNAL(textChanged()),this,SLOT(changed()));
    connect(ui->commandTextEdit,SIGNAL(textChanged()),this,SLOT(changed()));
    connect(ui->descTextEdit,SIGNAL(textChanged()),this,SLOT(changed()));
    connect(ui->parametreTextEdit,SIGNAL(textChanged()),this,SLOT(changed()));
    connect(ui->actionUusi,SIGNAL(triggered()),this,SLOT(newHelp()));
    connect(ui->actionTallenna,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionTallenna_nimell,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionPoistu,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAvaa,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionHtml_sivut,SIGNAL(triggered()),this,SLOT(generate()));
    enableEdit(false);
    mIgnoreChangeSignals = false;
    setTitle("");
    mBaseFolder = 0;
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);




    mBoldButton = ui->mainToolBar->addAction(tr("B"));
    mBoldButton->setCheckable(true);
    mBoldButton->setChecked(false);
    mBoldButton->setToolTip(tr("Lihavointi"));
    mBoldButton->setShortcut(QKeySequence(QKeySequence::Bold));
    connect(mBoldButton,SIGNAL(toggled(bool)),this,SLOT(setFontBold(bool)));

    ui->mainToolBar->addSeparator();

    mItalicButton = ui->mainToolBar->addAction(tr("I"));
    mItalicButton->setCheckable(true);
    mItalicButton->setChecked(false);
    mItalicButton->setToolTip(tr("Kursivointi"));
    mItalicButton->setShortcut(QKeySequence(QKeySequence::Italic));
    connect(mItalicButton,SIGNAL(toggled(bool)),this,SLOT(setFontItalic(bool)));

    ui->mainToolBar->addSeparator();

    mUnderlineButton = ui->mainToolBar->addAction(tr("U"));
    mUnderlineButton->setCheckable(true);
    mUnderlineButton->setChecked(false);
    mUnderlineButton->setToolTip(tr("Alleviivaus"));
    mUnderlineButton->setShortcut(QKeySequence(QKeySequence::Underline));
    connect(mUnderlineButton,SIGNAL(toggled(bool)),this,SLOT(setFontUnderline(bool)));

    ui->mainToolBar->addSeparator();

    mFontCombo = new QFontComboBox(this);
    mFontCombo->setCurrentFont(ui->parametreTextEdit->currentFont());
    connect(mFontCombo,SIGNAL(currentFontChanged(QFont)),this,SLOT(setTextEditFont(QFont)));
    ui->mainToolBar->addWidget(mFontCombo);

    ui->mainToolBar->addSeparator();

    mFontSize = new QDoubleSpinBox(this);
    mFontSize->setSingleStep(1);
    mFontSize->setDecimals(0);
    mFontSize->setMinimum(6);
    mFontSize->setMaximum(72);
    mFontSize->setValue(ui->parametreTextEdit->fontPointSize());
    mFontSize->setToolTip(tr("Tekstin koko"));
    connect(mFontSize,SIGNAL(valueChanged(double)),this,SLOT(setFontPointSize(double)));
    ui->mainToolBar->addWidget(mFontSize);

    connect(ui->descTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(ui->parametreTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentCharFormatChanged(QTextCharFormat)));
    setModified(false);
}

MainWindow::~MainWindow()
{
    clearAll();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (checkIsSaved()) {e->accept();return;}
    e->ignore();
}

void MainWindow::setTitle(const QString &fileName)
{
    this->setWindowTitle("Manual editor - "+fileName+"[*]");
}

void MainWindow::save()
{
    if (!mSavePath.isEmpty())
    {
        saveTo(mSavePath);
        return;
    }

    saveAs();
}

QString MainWindow::saveDialog()
{
    return QFileDialog::getSaveFileName(this,QString(),QString(),tr("Coolbasic ohjetiedosto (*.cbh)"));
}

void MainWindow::saveAs()
{
    QString filePath = saveDialog();
    if (!filePath.isEmpty())
    {
        saveTo(filePath);
    }
}

void MainWindow::nameChanged(QString name)
{
    if (mCurrentHelp)
    {
        mCurrentHelp->setName(name);
        setModified(true);
    }
}

void MainWindow::changed()
{
    if (mCurrentHelp)
    {
        setModified(true);
    }
}

void MainWindow::setModified(bool t)
{
    if (mIgnoreChangeSignals) return;
    this->setWindowModified(t);
    mModified = t;
}

void MainWindow::treePopup(QPoint point)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(point);
    if (item != 0)
    {
        if (mCommandHelps.contains(item))
        {
            CommandHelp *help = mCommandHelps[item];
            QMenu menu(this);
            QAction *select = menu.addAction(tr("Valitse"));
            QAction *remove = menu.addAction(tr("Poista"));
            QAction *clicked = menu.exec(ui->treeWidget->mapToGlobal(point),select);
            if (clicked == select)
            {
                selectCommandHelp(help);
            }
            else if (clicked == remove)
            {
                QMessageBox::StandardButton button = QMessageBox::question(this,tr("Haluatko varmasti poistaa ohjeen?"),tr("Haluatko varmasti poistaa ohjeen?\n"),QMessageBox::Yes|QMessageBox::No);
                if (button == QMessageBox::Yes)
                {
                    removeCommandHelp(help);
                }
            }
            return;
        }


        //Ei ollut CommandHelp, joten ilmeisesti kansio...
        CommandFolder *folder = mCommandFolders[item];
        QMenu menu(this);
        QAction *addHelp = menu.addAction(tr("Lisää ohje"));
        QAction *addFolder = menu.addAction(tr("Lisää kansio"));
        QAction *rename = menu.addAction(tr("Nimeä uudelleen"));
        QAction *remove = menu.addAction(tr("Poista"));
        if (folder == mBaseFolder)
        {
            remove->setEnabled(false);
        }
        QAction *clicked = menu.exec(ui->treeWidget->mapToGlobal(point),addHelp);
        if (clicked == rename)
        {
            TextDialog dialog(this);
            dialog.setText(tr("Nimeä kansio"));
            dialog.setText(folder->name());
            if (dialog.exec() == QDialog::Accepted)
            {
                folder->setName(dialog.text());
            }
        }
        else if (clicked == addHelp)
        {
            addNewHelp(item);
        }
        else if (clicked == addFolder)
        {
            addNewFolder(item);
        }
        else if (clicked == remove)
        {
            QMessageBox::StandardButton button = QMessageBox::question(this,tr("Haluatko varmasti poistaa kansion?"),tr("Haluatko varmasti poistaa kansion ja kaiken sen sisällön?\n"),QMessageBox::Yes|QMessageBox::No);
            if (button == QMessageBox::Yes)
            {
                removeCommandFolder(folder);
            }
        }
    }
}


void MainWindow::open()
{
    if (!checkIsSaved()) return;
    QString filePath = QFileDialog::getOpenFileName(this,QString(),QString(),tr("Coolbasic ohjetiedosto (*.cbh)"));
    if (!filePath.isEmpty())
    {
        clearAll();
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,tr("Virhe"),tr("Tiedoston avaus epäonnistui.\n %1").arg(filePath));
            return;
        }
        QDataStream s(&file);
        int magicnum;
        s >> magicnum;
        if (magicnum != MAGIC_NUM)
        {
            QMessageBox::critical(this,tr("Virhe"),tr("Tiedosto on virheellinen.\n %1").arg(filePath));
            return;
        }
        int versionum;
        s >> versionum;

        if (versionum == 1)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            mBaseFolder = new CommandFolder(s,item);
            mCommandFolders[item] = mBaseFolder;
            mBaseFolder->addAllToMaps(mCommandHelps,mCommandFolders);
        }
        else
        {
            QMessageBox::critical(this,tr("Virhe"),tr("Tiedostotyypin versio ei ole tuettu.\n %1").arg(filePath));
            return;
        }
        file.close();

        setModified(false);
        setTitle(filePath);
        mSavePath = filePath;
    }
}

bool MainWindow::saveTo(const QString &path)
{
    if (mCurrentHelp)
    {
        mCurrentHelp->mCommand = ui->commandTextEdit->toPlainText();
        mCurrentHelp->mDescription = ui->descTextEdit->toHtml();
        mCurrentHelp->mExamble = ui->exampleTextEdit->toHtml();
        mCurrentHelp->mParametres = ui->parametreTextEdit->toHtml();
        mCurrentHelp->setName(ui->nameLineEdit->text());
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,tr("Virhe"),tr("Tiedoston avaus epäonnistui.\n %1").arg(path));
        return false;
    }
    QDataStream out(&file);
    out << (int)MAGIC_NUM;
    out << (int)1;
    mBaseFolder->save(out);

    file.close();
    setModified(false);
    setTitle(path);
    mSavePath = path;
    return true;
}







void MainWindow::removeCommandFolder(CommandFolder *folder)
{
    selectCommandHelp(0);
    mCommandHelps.remove(folder->item());
    folder->removeAndDeleteAll(mCommandHelps,mCommandFolders);
    delete folder->item();
    delete folder;
}

void MainWindow::removeCommandHelp(CommandHelp *help)
{
    selectCommandHelp(0);
    mCommandHelps.remove(help->item());
    delete help->item();
    delete help;
}

void MainWindow::selectCommandHelp(CommandHelp *help)
{
    mIgnoreChangeSignals = true;
    if (mCurrentHelp)
    {

        //Tallennetaan vanha
        mCurrentHelp->mCommand = ui->commandTextEdit->toPlainText();
        mCurrentHelp->mDescription = ui->descTextEdit->toHtml();
        mCurrentHelp->mExamble = ui->exampleTextEdit->toHtml();
        mCurrentHelp->mParametres = ui->parametreTextEdit->toHtml();
        mCurrentHelp->setName(ui->nameLineEdit->text());
    }
    mCurrentHelp = help;
    if (help)
    {
        enableEdit(true);
        ui->commandTextEdit->setPlainText(mCurrentHelp->mCommand);
        ui->descTextEdit->setHtml(mCurrentHelp->mDescription);
        ui->exampleTextEdit->setHtml(mCurrentHelp->mExamble);
        ui->parametreTextEdit->setHtml(mCurrentHelp->mParametres);
        ui->nameLineEdit->setText(mCurrentHelp->name());
    }
    else
    {
        enableEdit(false);
        ui->commandTextEdit->setPlainText("");
        ui->descTextEdit->setPlainText("");
        ui->exampleTextEdit->setPlainText("");
        ui->parametreTextEdit->setPlainText("");
        ui->nameLineEdit->setText("");
    }
    mIgnoreChangeSignals = false;
}

void MainWindow::itemClicked(QModelIndex index)
{
    QTreeWidgetItem *item = (QTreeWidgetItem*)index.internalPointer();
    if (mCommandHelps.contains(item))
    {
        selectCommandHelp(mCommandHelps[item]);
    }
}

void MainWindow::enableEdit(bool t)
{
    ui->commandTextEdit->setEnabled(t);
    ui->descTextEdit->setEnabled(t);
    ui->exampleTextEdit->setEnabled(t);
    ui->nameLineEdit->setEnabled(t);
    ui->parametreTextEdit->setEnabled(t);
}


/**
  * Tarkastaa, että kaikki on tallennettu ja jos ei niin kysyy halutaanko tallentaa.
  *
  * @return Palauttaa voidaanko jatkaa ja tuhota edelliset tiedot
  */
bool MainWindow::checkIsSaved()
{
    if (mModified)
    {
        QMessageBox::StandardButton pressed = QMessageBox::question(this,tr("Tiedostossa on tallentamattomia muutoksia"),tr("Haluatko tallentaa muutokset? \nKaikki tallentamattomat muutokset menetetään."),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (pressed == QMessageBox::Yes)
        {
            QString path = saveDialog();
            if (path.isEmpty())
            {
                //Painettiin Cancel
                return false;
            }

            //Tallennetaan
            return saveTo(path);
        }
        if (pressed == QMessageBox::No) return true;
        if (pressed == QMessageBox::Cancel) return false;
    }
    return true;
}

void MainWindow::newHelp()
{
    if (!checkIsSaved()) return;
    clearAll();
    enableEdit(true);
    //Pohjakansio
    QTreeWidgetItem *folder = new QTreeWidgetItem(ui->treeWidget);
    folder->setText(0,tr("Ohjeet"));
    CommandFolder *cfolder = new CommandFolder(tr("Ohjeet"),folder);
    mCommandFolders[folder] = cfolder;
    mBaseFolder = cfolder;

    //Ensimmäinen ohje
    QTreeWidgetItem *startHelp = new QTreeWidgetItem(folder);
    CommandHelp *chelp = new CommandHelp(startHelp);
    chelp->setName(tr("Funktio"));
    mCommandHelps[startHelp] = chelp;
    ui->nameLineEdit->setText(chelp->name());
    mCurrentHelp = chelp;
    setTitle("unsaved");
    setModified(true);
    folder->setExpanded(true);
}

void MainWindow::clearAll()
{
    for (CommandHelpMap::iterator i = mCommandHelps.begin();i != mCommandHelps.end(); i++)
    {
        delete i.value();
    }
    mCommandHelps.clear();

    for (CommandFolderMap::iterator i = mCommandFolders.begin();i != mCommandFolders.end(); i++)
    {
        delete i.value();
    }
    mCommandFolders.clear();

    enableEdit(false);
    mIgnoreChangeSignals = true;
    mCurrentHelp = 0;
    ui->commandTextEdit->setPlainText("");
    ui->descTextEdit->setPlainText("");
    ui->exampleTextEdit->setPlainText("");
    ui->parametreTextEdit->setPlainText("");
    ui->nameLineEdit->setText("");
    ui->treeWidget->clear();
    mIgnoreChangeSignals = false;
    setModified(true);
}

void MainWindow::addNewHelp(QTreeWidgetItem *parent)
{
    Q_ASSERT(mCommandFolders.contains(parent));
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    CommandHelp *help = new CommandHelp(item);
    help->setName(tr("Funktio"));
    setModified(true);
    mCommandHelps[item] = help;
    parent->setExpanded(true);
    mCommandFolders[parent]->mHelps.append(help);
    help->setParent(mCommandFolders[parent]);
}

void MainWindow::addNewFolder(QTreeWidgetItem *parent)
{
    Q_ASSERT(mCommandFolders.contains(parent));
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    CommandFolder *folder = new CommandFolder(tr("Uusi kansio"),item);
    setModified(true);
    mCommandFolders[item] = folder;
    parent->setExpanded(true);
    mCommandFolders[parent]->mFolders.append(folder);
    folder->setParent(mCommandFolders[parent]);
}

void MainWindow::generate()
{
    if (mBaseFolder == 0) return;
    GenerateDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QFile functionBaseFile(dialog.functionBase());
        if (!functionBaseFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,tr("Tiedostoa ei voida avata"),tr("Tiedostoa %1 ei voida avata.").arg(dialog.functionBase()));
            return;
        }
        QString functionBase;
        QTextStream in(&functionBaseFile);
        QString line = in.readLine();
        while (!line.isNull()) {
            functionBase += line + "\n";
            line = in.readLine();
        }
        functionBaseFile.close();

        QFile folderBaseFile(dialog.folderBase());
        if (!folderBaseFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,tr("Tiedostoa ei voida avata"),tr("Tiedostoa %1 ei voida avata.").arg(dialog.folderBase()));
            return;
        }
        QString folderBase;
        QTextStream in2(&folderBaseFile);
        line = in2.readLine();
        while (!line.isNull()) {
            folderBase += line + "\n";
            line = in2.readLine();
        }
        folderBaseFile.close();

        folderBase = folderBase.replace("$$MANUALNAME$$",mBaseFolder->name()).replace("$$FUNCSMALLDESC$$","");
        functionBase = functionBase.replace("$$MANUALNAME$$",mBaseFolder->name());

        mBaseFolder->generateFiles(dialog.destinationFolder(),folderBase,functionBase);

    }
}

void MainWindow::setFontBold(bool t)
{
    if (mIgnoreSignals) return;
    if (t)
    {
        ui->descTextEdit->setFontWeight(QFont::Bold);
        ui->parametreTextEdit->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->descTextEdit->setFontWeight(QFont::Normal);
        ui->parametreTextEdit->setFontWeight(QFont::Normal);
    }
}

void MainWindow::setFontItalic(bool t)
{
    if (mIgnoreSignals) return;
    ui->descTextEdit->setFontItalic(t);
    ui->parametreTextEdit->setFontItalic(t);
}

void MainWindow::setFontUnderline(bool t)
{
    if (mIgnoreSignals) return;
    ui->descTextEdit->setFontUnderline(t);
    ui->parametreTextEdit->setFontUnderline(t);
}

void MainWindow::setTextEditFont(QFont font)
{
    if (mIgnoreSignals) return;
    ui->descTextEdit->setCurrentFont(font);
    ui->parametreTextEdit->setCurrentFont(font);
}


void MainWindow::currentCharFormatChanged(QTextCharFormat format)
{
    fontChanged(format.font());
}

void MainWindow::fontChanged(const QFont &font)
{
    mIgnoreSignals = true;
    mFontCombo->setCurrentIndex(mFontCombo->findText(QFontInfo(font).family()));
    mFontSize->setValue(font.pointSize());
    mBoldButton->setChecked(font.bold());
    mItalicButton->setChecked(font.italic());
    mUnderlineButton->setChecked(font.underline());
    mIgnoreSignals = false;
}

void MainWindow::setFontPointSize(double s)
{
    ui->descTextEdit->setFontPointSize(s);
    ui->parametreTextEdit->setFontPointSize(s);
}
