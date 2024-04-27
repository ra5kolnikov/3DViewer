#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <QColor>
#include <QObject>
#include "builder.h"
#include "config.h"

class Director
{
public:
    Director() = default;
    static Config BuildConfig1();
    static Config BuildConfig2();

signals:
private:
};

#endif  // DIRECTOR_H
