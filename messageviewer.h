#ifndef MESSAGEVIEWER_H
#define MESSAGEVIEWER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QListView>
#include <QSqlDatabase>

class messageViewer : public QWidget
{
    Q_OBJECT
    
public:
    messageViewer(QWidget *parent = 0);
    ~messageViewer();
private:
    QStandardItemModel *parsedMessages = 0, *parsedContacts = 0;
    QStandardItemModel *cleanup(QStandardItemModel*);
    QListView *sideList = 0, *mainList = 0;
    bool dbOpen();
    QSqlDatabase dbase;
    void setListViews();
    QStandardItemModel *contactsData = 0, *messagesData = 0;
    void updateContactsTable();
    QStringList colorList;

private slots:
    void csvImport();
    void cleanContacts();
    void updateMessages(QModelIndex);
    void contactImport();
    void fbImporter();
    void dbDelete();
};

#endif // MESSAGEVIEWER_H
