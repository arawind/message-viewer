#include "messageviewer.h"
#include <QMenuBar>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include "csvparser.h"
#include "vcfparser.h"
#include "fbparser.h"
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QProgressDialog>
#include <QListView>
#include <QByteArray>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDebug>


messageViewer::messageViewer(QWidget *parent)
    : QWidget(parent)
{
    QMenuBar *menubar;
    menubar = new QMenuBar(this);
    QMenu *fileMenu;
    //QPushButton *openButton;
    //openButton = new QPushButton("Import CSV");
    QVBoxLayout *vert;
    QVBoxLayout *sideVert;
    vert = new QVBoxLayout();
    sideVert = new QVBoxLayout();
    QHBoxLayout *horiz;
    horiz = new QHBoxLayout();
    vert->addWidget(menubar);
    //vert->addWidget(openButton);
    sideList = new QListView(this);
    mainList = new QListView(this);
    horiz->addWidget(sideList);
    horiz->addWidget(mainList);
    vert->addLayout(horiz);
    setLayout(vert);
    fileMenu = new QMenu("Messages",menubar);
    QAction *import = fileMenu->addAction("Import Messages");
    QAction *fbImport = fileMenu->addAction("Import FB messages");
    QAction *contacts = fileMenu->addAction("Import Contacts");
    QAction *cleanContact = fileMenu->addAction("Cleanup Contacts");
    QAction *clearDB = fileMenu->addAction("Clear DB");


    menubar->addMenu(fileMenu);
    //connect(openButton, SIGNAL(clicked()), SLOT(csvImport()));
    connect(import, SIGNAL(triggered()), SLOT(csvImport()));
    connect(contacts, SIGNAL(triggered()), SLOT(contactImport()));
    connect(cleanContact, SIGNAL(triggered()), SLOT(cleanContacts()));
    connect(fbImport, SIGNAL(triggered()), SLOT(fbImporter()));
    connect(clearDB, SIGNAL(triggered()), SLOT(dbDelete()));
    connect(sideList, SIGNAL(clicked(QModelIndex)), SLOT(updateMessages(QModelIndex)));

    if(dbOpen())
        setListViews();


}

messageViewer::~messageViewer()
{
    
}

void messageViewer::csvImport(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV", QDir::homePath(), "CSV (*.csv)");
    QString temp;
    if(fileName.isEmpty()){
        return;
    }

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)){
        return;
    }

    QTextStream in(&file);
    temp = in.readAll();
    CSVparser parser(temp);
    parsedMessages = new QStandardItemModel();
    parsedMessages = parser.parse();
    //qDebug()<<parsedMessages->columnCount()<<endl;
    parsedMessages = cleanup(parsedMessages);
    //qDebug()<<parsedMessages->columnCount()<<endl;

    //qDebug()<<parsedMessages->item(0,3)->text();
    //qDebug()<<parsedMessages->item(4,2)->text();

    bool res = dbOpen();

    if(res){
        QSqlQuery qry;
        res = qry.exec("CREATE TABLE IF NOT EXISTS messages(mID integer PRIMARY KEY AUTOINCREMENT, mState integer, mTime TEXT, mPersonID TEXT, mText TEXT, UNIQUE (mState, mTime, mPersonID, mText));");

        if(!res){
            qDebug()<<qry.lastError();
        }
        else{
            //CSV import
            QProgressDialog* progress = new QProgressDialog("Importing from CSV", "Abort",0, parsedMessages->rowCount(), this);
            progress->setWindowModality(Qt::WindowModal);
            progress->show();
            dbase.transaction();
            for(int i=0; i<parsedMessages->rowCount(); i++){
                QStandardItem *state = parsedMessages->item(i,1);

                QStandardItem *time = parsedMessages->item(i,5);

                QStandardItem *personID;
                if(state->text()=="2")
                    personID = parsedMessages->item(i,2);
                else
                    personID = parsedMessages->item(i,3);

                QStandardItem *text = parsedMessages->item(i,7);
                //QString uid = (state->text() + time->text() + personID->text() + text->text()).toUtf8().toBase64();

                //QString query = QString("INSERT INTO messages (mState, mTime, mPersonID, mText) VALUES (%1, '%2', '%3', '%4');").arg(state->text(), time->text(), personID->text(), text->text().toUtf8().toBase64());
                //QString query = QString("%1 %2 %3 %4").arg(state->text(), ,"","");
                //qDebug()<<query;
                //query
                //qDebug()<<query;
                //qDebug()<<state->text();//<<time->text()<<personID->text();
                /*qry.prepare("INSERT INTO messages (mState, mTime, mPersonID, mText)"
                            "VALUES (?, ?, ?, ?)");
                qDebug()<<qry.boundValues().count();

                qry.addBindValue(state->text().toInt());
                qry.addBindValue(time->text());
                qry.addBindValue(personID->text());
                qry.addBindValue(text->text());
                qDebug()<<qry.boundValues().count();*/
                //qDebug()<<query;
                qry.prepare("INSERT OR IGNORE INTO messages (mState, mTime, mPersonID, mText)"
                            "VALUES (?, ?, ?, ?);");
                qry.addBindValue(state->text().toInt());
                qry.addBindValue(time->text());
                qry.addBindValue(personID->text());
                qry.addBindValue(text->text());

                res = qry.exec();

                if(res){
                    progress->setValue(i);
                    continue;

                }
                else if(progress->wasCanceled()){
                    progress->close();
                    break;
                }
                else{
                    qDebug()<<qry.lastError();
                    break;
                }
            }
            progress->setValue(parsedMessages->rowCount());
            if(!dbase.commit())
                qDebug()<<qry.lastError();
            else{
                contactsData = new QStandardItemModel(this);
                QStandardItem *parentItem = contactsData->invisibleRootItem();
                //QSqlQuery qry;
                qry.prepare("SELECT DISTINCT mPersonID FROM messages;");
                if(!qry.exec())
                    return;
                else{
                    while(qry.next()){
                        QStandardItem *item = new QStandardItem(qry.value("mPersonID").toString());
                        parentItem->appendRow(item);
                    }
                    cleanContacts();
                    sideList->setModel(contactsData);
                }
            }
        }

        dbase.close();

    }

}

QStandardItemModel* messageViewer::cleanup(QStandardItemModel *model){
    QList<QStandardItem*> time = model->takeColumn(5);
    QMutableListIterator<QStandardItem*> iterator(time);
    while(iterator.hasNext()){
        QStandardItem* timeItem = iterator.next();
        QDateTime normalised = QDateTime::fromString(timeItem->text(), "yyyy.MM.dd HH:mm");
        // timeItem = yyyy.mm.dd HH:mm;
        timeItem->setText(QString::number(normalised.toMSecsSinceEpoch()/1000));
        iterator.setValue(timeItem);
    }
    //model->removeColumn(5);
    //model->insertColumn(5);
    model->insertColumn(5, time);

    QList<QStandardItem*> state = model->takeColumn(1);
    QMutableListIterator<QStandardItem*> siterator(state);
    while(siterator.hasNext()){
        QStandardItem* stateItem = siterator.next();
        if(stateItem->text().contains("SENT"))
            stateItem->setText("1");
        else
            stateItem->setText("2"); //RECEIVED
        siterator.setValue(stateItem);
    }
    //model->removeColumn(1);
    //model->insertColumn(1);
    model->insertColumn(1, state);

    /*QList<QStandardItem*> text = model->takeColumn(7);
    QMutableListIterator<QStandardItem*> titerator(text);
    while(titerator.hasNext()){
        QStandardItem* textItem = titerator.next();
        QString final = textItem->text().replace("'","\\'");
        final = final.replace("\"","\\\"");
        final =
    }
    //model->removeColumn(1);
    //model->insertColumn(1);
    model->insertColumn(7, text);*/

    qDebug()<<model->columnCount();
    return model;
}

bool messageViewer::dbOpen(){
    if(!dbase.isValid()){
        dbase = QSqlDatabase::addDatabase("QSQLITE");
        dbase.setHostName("localhost");

        QCoreApplication::setOrganizationName("arawind");
        QCoreApplication::setApplicationName("message-viewer");
        QStringList path = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        QDir dir = QDir::root();
        if(dir.mkpath(path.first()))
            dbase.setDatabaseName(path.first().append(QDir::separator()).append("my.db.sqlite"));

        qDebug()<<path.first().append(QDir::separator()).append("my.db.sqlite");
    }
    if(!dbase.open()){
        qDebug()<<"error "<<dbase.lastError();
        return false;
    }
    return true;
}

void messageViewer::setListViews(){
    if(!dbase.isOpen())
        return;
    QSqlQuery qry;
    qry.prepare("SELECT name, lastMssgTime, numMessages FROM contacts WHERE numMessages > 0 ORDER BY lastMssgTime DESC;");
    if(!qry.exec())
        return;
    contactsData = new QStandardItemModel(this);
    QStandardItem *parent = contactsData->invisibleRootItem();
    while(qry.next()){
        QStandardItem *item = new QStandardItem(qry.value("name").toString());
        item->setData(qry.value("lastMssgTime"), Qt::UserRole+1);
        item->setData(qry.value("numMessages"), Qt::UserRole+2);
        parent->appendRow(item);
    }
    sideList->setModel(contactsData);
    dbase.close();
}

void messageViewer::cleanContacts(){
    if(!dbOpen())
        return;
    if(contactsData->rowCount() == 0)
        qDebug()<<"no rows";
    else{

        QMap<QString, QString> map;
        QRegularExpression reg("[\\+\\-\\(\\)\\d]{10,}");
        for(int i=0; i<contactsData->rowCount(); i++){
            QString temp = contactsData->item(i)->text();
            if(reg.match(temp).hasMatch()){
                map.insert(temp, temp.replace(QRegularExpression("[\\(\\)\\- ]"),"").right(10));
                //qDebug()<<temp.replace(QRegularExpression("[\\(\\)\\- ]"),"");//.right(10);
            }
            else
                map.insert(temp, temp);
        }
        QMapIterator<QString, QString> iterator(map);
        contactsData->clear();
        QSqlQuery qry;
        qry.prepare("CREATE TABLE IF NOT EXISTS contactAliases (aID integer primary key autoincrement, alias varchar(200) unique, contactID varchar(200), displayString text );");
        if(!qry.exec())
            return;
        qry.prepare("CREATE TABLE IF NOT EXISTS contacts (cID integer primary key autoincrement, name text unique, numMessages integer, lastMssgTime text);");
        if(!qry.exec())
            return;
        if(!dbase.transaction())
            return;
        qry.clear();

        qry.prepare("INSERT OR IGNORE INTO contactAliases (alias, contactID, displayString)"
                    "VALUES (:alias, :contactID, :displayString) ");
        while(iterator.hasNext()){
            iterator.next();
            qry.bindValue(":alias", iterator.key());
            qry.bindValue(":contactID", iterator.value());
            qry.bindValue(":displayString", iterator.value());
            //qry.bindValue(":contactName", iterator.value());
            if(!qry.exec())
                break;
        }

        qry.clear();
        if(!dbase.commit())
            return;

        updateContactsTable();


    }
    setListViews();
    qDebug()<<contactsData->rowCount();
    //sideList->setModel(contactsData);
    //dbase.close();
}

void messageViewer::updateContactsTable(){

    if(!dbase.isOpen())
        return;

    QSqlQuery qry;
    QMap<QString, int> map;
    QMap<QString, QString> timeMap;
    dbase.transaction();

    qry.prepare("SELECT alias, displayString FROM contactAliases;");
    if(qry.exec()){
        while(qry.next()){

            int temp = 0;
            QSqlQuery intQry;
            intQry.prepare("SELECT COUNT(*), mTime FROM messages WHERE mPersonID = :personID ORDER BY mTime DESC");
            intQry.bindValue(":personID", qry.value("alias").toString());
            if(intQry.exec()){
                intQry.next();
                temp = intQry.value(0).toInt();
                //qDebug()<<temp;
                if(map.find(qry.value("displayString").toString()) != map.end()){
                    temp = temp + map.value(qry.value("displayString").toString());
                }
                //qDebug()<<qry.value("alias").toString()<<temp;
                map.insert(qry.value("displayString").toString(), temp);
                timeMap.insert(qry.value("displayString").toString(), intQry.value(1).toString());
            }

        }
    }
    qry.clear();
    qry.prepare("INSERT OR IGNORE INTO contacts(name, numMessages, lastMssgTime)"
                "VALUES (:name, :numMessages, :lastMssg)");
    QMapIterator<QString, int> iterator(map);
    while(iterator.hasNext()){
        iterator.next();

        qry.bindValue(":name", iterator.key());
        qry.bindValue(":numMessages", iterator.value());
        qry.bindValue(":lastMssg", timeMap.value(iterator.key()));
        if(!qry.exec())
            break;
    }
    dbase.commit();

}

void messageViewer::updateMessages(QModelIndex index){
    if(dbOpen()){
    QStandardItem *contact = contactsData->itemFromIndex(index);
    messagesData = new QStandardItemModel(this);
    QStandardItem *item = messagesData->invisibleRootItem();

    QSqlQuery qry;
    QStringList aliases;
    QString tempString = "SELECT alias FROM contactAliases WHERE displayString = :string";
    qry.prepare(tempString);
    qry.bindValue(":string",contact->text());
    qDebug()<<contact->text();

    if(qry.exec()){
        while(qry.next()){
            aliases.append(qry.value("alias").toString());
            qDebug()<<qry.value("alias").toString();
        }
        QString queryString = "SELECT mID, mState, mTime, mPersonID, mText FROM messages DESC WHERE mPersonID = ? ";
        for(int i=1; i<aliases.length(); i++)
            queryString += "OR mPersonID = ? ";
        queryString += " ORDER BY mTime DESC;";
        QSqlQuery intQry;
        intQry.prepare(queryString);
        for(int i=0; i<aliases.length(); i++)
            intQry.addBindValue(aliases.value(i));

        if(intQry.exec()){

            while(intQry.next()){
                QStandardItem *temp = new QStandardItem(intQry.value("mText").toString());
                item->setData(intQry.value("mState").toInt(), Qt::UserRole+1);
                item->appendRow(temp);
            }

            mainList->setModel(messagesData);
        }
        else
            qDebug()<<intQry.lastError();
    }
    dbase.close();
    }

}

void messageViewer::contactImport(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Select files for import", QDir::homePath(), "vCard (*.vcf)");
    parsedContacts = new QStandardItemModel(this);
    QStandardItem *contacts = parsedContacts->invisibleRootItem();
    QStringListIterator fileIterator(fileNames);
    QMap<QString, QString> toTable;
    while(fileIterator.hasNext()){
        QFile *file = new QFile(fileIterator.next());
        if(!file->open(QFile::ReadOnly))
            return;

        QTextStream txt(file);
        QString temp;
        temp = txt.readAll();
        VCFparser vcf(temp);
        QMap<QString, QStandardItem*> *map = vcf.parse();
        //QString tempAlias = map->value("TEL")->text().replace("[\\(\\) -;]","");
        QString tempDisplayString;
        if(map->keys().contains("FN"))
            tempDisplayString = map->value("FN")->text().split(';').join(' '); //FIXME
        else
            tempDisplayString = map->value("N")->text().split(';').join(' '); //FIXME TOO
        QListIterator<QStandardItem*> i(map->values("TEL"));
        while(i.hasNext()){
            QString tempAlias = i.next()->text().replace(QRegularExpression("[\\(\\)\\- \\;]"),"");
            toTable.insert(tempAlias, tempDisplayString);
        }
    }

    QMapIterator<QString, QString> iterator(toTable);
    if(!dbase.isOpen())
        if(!dbOpen())
            return;
    dbase.transaction();
    QSqlQuery qry;
    QMap<QString, QString> fromTableAlias;
    QMap<QString, QString> fromTableID;

    qry.prepare("CREATE TABLE IF NOT EXISTS contactAliases (aID integer primary key autoincrement, alias varchar(200) unique, contactID varchar(200), displayString text );");
    if(!qry.exec())
        return;
    qry.prepare("CREATE TABLE IF NOT EXISTS contacts (cID integer primary key autoincrement, name text unique, numMessages integer, lastMssgTime text);");
    if(!qry.exec())
        return;

    qry.clear();

    qry.prepare("SELECT name,cid from contacts");
    if(!qry.exec())
        return;
    while(qry.next()){
        fromTableID.insert(qry.value("name").toString(), qry.value("cid").toString());
    }
    qry.clear();

    while(iterator.hasNext()){
        iterator.next();
        QSqlQuery intQry;
        if(!fromTableID.keys().contains(iterator.key())) // && !fromTableID.keys().contains(iterator.key()))
        {
            intQry.prepare("INSERT OR REPLACE INTO contactAliases(alias, contactID, displayString)"
                        "VALUES (:alias, :contactID, :displayString)");
            intQry.bindValue(":alias", iterator.key());
            intQry.bindValue(":contactID", iterator.key().right(10)); //ONLY TAKING TEL VALUES - 10 digits
            intQry.bindValue(":displayString", iterator.value());
            //qDebug()<<iterator.value()<<iterator.key().right(10);
        }
        else{
            intQry.prepare("UPDATE contactAliases SET displayString = :displayString WHERE contactID = :contactID");
            intQry.bindValue(":contactID", iterator.key().right(10));
            intQry.bindValue(":displayString", iterator.value());
            qDebug()<<iterator.value()<<iterator.key();

        }


        if(!intQry.exec()){
            qDebug()<<intQry.lastQuery();
            qDebug()<<intQry.lastError();
            break;
        }
        intQry.clear();


    }

    dbase.commit();

    qry.prepare("SELECT name,cid from contacts");
    if(!qry.exec())
        return;
    while(qry.next()){
        fromTableAlias.insert(qry.value("name").toString(), qry.value("cid").toString());
    }
    qry.clear();

    iterator.toFront();

    dbase.transaction();

    while(iterator.hasNext()){
        iterator.next();
        if(fromTableAlias.keys().contains(iterator.key().right(10))){ //search if the number is the old value
            qry.prepare("UPDATE OR IGNORE contacts SET name=:newname WHERE cid=:idnum");
            qry.bindValue(":idnum", fromTableAlias.value((iterator.key().right(10))).toInt()); //the corresponding id
            qry.bindValue(":newname", iterator.value());

            if(!qry.exec()){
                qDebug()<<qry.lastError();
            }
            else
                qDebug()<<"updating "<<iterator.value()<<fromTableAlias.value((iterator.key().right(10))).toInt();
        }

        else if(! fromTableAlias.keys().contains(iterator.value())){ //search if the name is not set
            //qry.prepare("INSERT OR REPLACE INTO ")
        }


    }



    dbase.commit();

    setListViews(); //this'll close the connection
    //dbase.close();
}

void messageViewer::fbImporter(){
    QString fileName = QFileDialog::getOpenFileName(this, "Select the messages.html file from your FB data", QDir::homePath(), "HTML (*.html);; XML (*.xml)");
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
        return;
    QString text = file.readAll();
    FBparser parser(text);
    parser.parse();
}

void messageViewer::dbDelete(){
    dbase.close();
    dbase.removeDatabase(dbase.connectionName());
    QStringList path = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString filename = path.first().append(QDir::separator()).append("my.db.sqlite");
    QFile::remove(filename);
    if(contactsData->hasChildren())
        contactsData->clear();
    if(messagesData->hasChildren())
        contactsData->clear();

}
