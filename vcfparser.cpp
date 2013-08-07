#include "vcfparser.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QTextStream>
#include <QMap>
#include <QDebug>

VCFparser::VCFparser(QString input)
{
    text=input;
}

QMap<QString, QStandardItem *> *VCFparser::parse(QString input){
    if(!input.isEmpty())
        text=input;
    QTextStream txtStream(&text);
    QString temp;
    QMap<QString, QStandardItem*>* map = new QMap<QString, QStandardItem*>();
    txtStream>>temp;
    if(temp.compare("BEGIN:VCARD")==0)
        while(!txtStream.atEnd()){
            temp = txtStream.readLine();
            QString key;
            QString properties;
            QString values;
            if(temp.indexOf(';')!=-1 && temp.indexOf(';') < temp.indexOf(':')){
                key = temp.section(';',0,0);
                properties = temp.section(';',1).section(':',0,0);
                values = temp.section(':',1);
            }
            else{
                key = temp.section(':',0,0);
                values = temp.section(':',1);
            }
            QStandardItem* tempItem = new QStandardItem(values);
            QStandardItem* propItem = new QStandardItem(properties);
            tempItem->appendRow(propItem);
            if(key!="END" && !key.isEmpty())
                map->insertMulti(key, tempItem);
            //qDebug()<<"aabbcc"<<key<<properties<<values;
        }
    return map;
}
