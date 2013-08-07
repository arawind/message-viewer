#ifndef FBPARSER_H
#define FBPARSER_H

#include <QString>

class FBparser
{
public:
    FBparser(QString input="");
    void parse(QString input="");
private:
    QString text;
};

#endif // FBPARSER_H
