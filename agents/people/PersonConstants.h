#ifndef PERSONCONSTANTS_H
#define PERSONCONSTANTS_H

#include <QObject>
#include <QDateTime>

class PersonConstants : public QObject
{
    Q_OBJECT
public:
    enum gender {MALE, FEMALE};
    enum age_type {YOUNG, ADULT, ELDER};

    /**
     * @brief createBornDate
     * Create a born date time for the specified enum type
     * @param type_age A value from PersonConstants::age_type
     * @return QDateTime
     */
    static QDateTime createBornDate(PersonConstants::age_type type_age){
        QDateTime age;
        int current_year = QDateTime::currentDateTime().date().year();
        switch(type_age){
        case YOUNG: { // 0 - 19
            int rand = qrand() % 20;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        case ADULT: // 20 - 64
        default: {
            int rand = (qrand() % 45) + 20;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        case ELDER: { // 65 - 100
            int rand = (qrand() % 35) + 65;
            age = QDateTime(QDate(current_year - rand , 1 , 1 ));
            return age; }
        }
    }

};


#endif // PERSONCONSTANTS_H

