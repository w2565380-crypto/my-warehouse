#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QObject>

class DataCollector : public QObject
{
    Q_OBJECT
public:
    explicit DataCollector(QObject *parent = nullptr);

public slots:
    void collectData(); // 这是一个槽函数，会被定时器触发
};

#endif // DATACOLLECTOR_H
