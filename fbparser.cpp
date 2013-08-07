#include "fbparser.h"
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <QDebug>
#include <QFile>
FBparser::FBparser(QString input)
{
    text = input;
    qDebug() << text.length();
}

void FBparser::parse(QString input){
    if(!input.isEmpty())
        text = input;
    //text = text.toUtf8();
    text = text.replace(QRegularExpression("[\^\\x00-\\x7F]")," ");
    QFile file("/home/arawind/newhtml");
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    out<<text;
    text = text.replace("<br>", " ",Qt::CaseInsensitive);
    QXmlStreamReader reader(text);
    while(reader.readNext()){
        if(reader.tokenString()=="Invalid"){
            qDebug()<<reader.errorString()<<reader.tokenString();
            //qDebug()<<reader.name();
            //qDebug()<<reader.text();
            //qDebug()<<reader.
            break;
        }
        qDebug()<<reader.name()<<reader.tokenString()<<reader.text();
    }

    /*
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class");
    qDebug()<<reader.readNext()<<reader.tokenString()<<reader.name()<<reader.attributes().value("class"); */
}
