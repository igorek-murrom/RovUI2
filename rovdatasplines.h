#ifndef ROVDATASPLINES_H
#define ROVDATASPLINES_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class RovDataSplines;
}

class RovDataSplines : public QDialog
{
    Q_OBJECT

public:
    explicit RovDataSplines(QWidget *parent = nullptr);
    ~RovDataSplines();

public slots:
    void addVoltageSample(QPointF sample);
    void addCurrentSample(QPointF sample);

private:
    QScopedPointer<QSplineSeries> m_voltageSeries;
    QScopedPointer<QSplineSeries> m_currentSeries;
    QScopedPointer<QChart> m_voltageChart;
    QScopedPointer<QChart> m_currentChart;

    Ui::RovDataSplines *ui;
};

#endif // ROVDATASPLINES_H
