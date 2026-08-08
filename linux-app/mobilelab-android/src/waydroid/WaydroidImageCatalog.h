#pragma once
#include <QVector>
#include <QString>
#include <QJsonArray>

struct AndroidImage {
    QString name;
    QString path;
    QString architecture;
    QString fingerprint;
    bool valid=false;
};

class WaydroidImageCatalog {
public:
    QVector<AndroidImage> discover() const;
    QJsonArray toJson() const;
};
