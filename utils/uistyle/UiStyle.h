#ifndef UISTYLE_H
#define UISTYLE_H

#include <QObject>
#include <QJsonObject>
#include <QColor>

class UiStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color)
    Q_PROPERTY(QColor border_color)
    Q_PROPERTY(int border_weight)
    Q_PROPERTY(QString dash_array)
    Q_PROPERTY(QString icon_url)

public:
    explicit UiStyle(QObject *parent = 0);
    ~UiStyle();
    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // METHODS
    static QColor randomColor();
    static QString toHTML(QColor color);
    static QColor fromHTML(QString color);

    // VARIABLES
    QColor color; // Agent geometry inner color
    QColor border_color; // Agent geometry border color
    int border_weight; // Agent geometry border weight
    QString dash_array; // Agent geometry line format
    QString icon_url; // Agent geometry (only for points) icon
};

#endif // UISTYLE_H
