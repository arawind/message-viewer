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
    QStandardItemModel *parsedMessages, *parsedContacts;
    QStandardItemModel *cleanup(QStandardItemModel*);
    QListView *sideList, *mainList;
    bool dbOpen();
    QSqlDatabase dbase;
    void setListViews();
    QStandardItemModel *contactsData, *messagesData;
    void updateContactsTable();

private slots:
    void csvImport();
    void cleanContacts();
    void updateMessages(QModelIndex);
    void contactImport();
    void fbImporter();
};

#endif // MESSAGEVIEWER_H
