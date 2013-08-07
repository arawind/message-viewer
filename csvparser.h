#ifndef CSVPARSER_H
#define CSVPARSER_H
#include <QString>
#include <QStandardItemModel>

class CSVparser
{

public:
    explicit CSVparser(QString input = "");
    QStandardItemModel* parse(QString input="");
private:
    QString text;
    
};

#endif // CSVPARSER_H
