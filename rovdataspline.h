#ifndef ROVDATASPLINE_H
#define ROVDATASPLINE_H

#include <QtCharts>
#include <QObject>
#include <QWidget>

class rovDataSpline : QWidget
{
Q_OBJECT
public:
    rovDataSpline();

public slots:
//    void addVoltageSample(float sample);
//    void addCurrentSample(float sample);

private:
    QScopedPointer<QSplineSeries> m_voltageSeries;
    QScopedPointer<QSplineSeries> m_currentSeries;
};

#endif // ROVDATASPLINE_H
