// datacollector.h
#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QObject>

class DataCollector : public QObject
{
    Q_OBJECT
public:
    explicit DataCollector(QObject *parent = nullptr);

signals:
    // 定义信号：将温度和湿度传递给主界面
    void dataUpdated(double temp, double humi);

public slots:
    void collectData();
};

#endif
