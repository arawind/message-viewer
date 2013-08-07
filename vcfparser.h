#ifndef VCFPARSER_H
#define VCFPARSER_H

#include <QStandardItemModel>
#include <QString>
#include <QMap>

class VCFparser
{
public:
    VCFparser(QString input="");
    QMap<QString, QStandardItem*> *parse(QString input="");
private:
    QString text;
    QStandardItemModel *vCard;
};

#endif // VCFPARSER_H
