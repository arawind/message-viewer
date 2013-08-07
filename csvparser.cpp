#include "csvparser.h"
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QStandardItemModel>
#include <QList>
#include <QDebug>
CSVparser::CSVparser(QString input)
{
    text = input;

}

QStandardItemModel *CSVparser::parse(QString input){
    if(!input.isEmpty())
        text=input;
    bool inQuot = false;
    int numQuot = 0;
    text = text.replace("\r","");
    QTextStream txtStream(&text);
    QChar ch;
    QString temp;
    QStandardItemModel* messages;
    QList<QStandardItem*> tempList;
    QStandardItem *tempItem = new QStandardItem();
    while(!txtStream.atEnd()){
        txtStream>>ch;
        if(ch=='"'){
            inQuot = !inQuot;
            numQuot++;

            if(numQuot==3){
                numQuot = 0;
                temp.append(QChar('"'));
            }
        }

        else if(ch=='\n' && !inQuot){
            numQuot=0;
            QStandardItem *item = new QStandardItem();
            item->setText(temp);
            //tempItem->appendRow(item);
            tempList.append(item);
            //qDebug()<<"----------------";
            temp.clear();
            messages->appendRow(tempList);
            //tempItem = new QStandardItem();
            tempList.clear();
        }

        else if(ch==',' && !inQuot){
            numQuot=0;
            //New Field
            QStandardItem *item = new QStandardItem();
            item->setText(temp);
            //qDebug()<<tempList.length()<<temp;
            //tempItem->appendRow(item);
            //qDebug()<<tempItem->rowCount()<<temp;
            tempList.append(item);
            temp.clear();
        }

        else{
            numQuot=0;
            temp.append(ch);
        }


    }
    return messages;
}
