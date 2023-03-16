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
    QValueAxis *m_voltageAxisV;
    QValueAxis *m_voltageAxisH;

    QValueAxis *m_currentAxisV;
    QValueAxis *m_currentAxisH;

    QSplineSeries *m_voltageSeries;
    QSplineSeries *m_currentSeries;

    QScopedPointer<QChart> m_voltageChart;
    QScopedPointer<QChart> m_currentChart;

    int maxVSamples = 300;
    int maxCSamples = 300;

    Ui::RovDataSplines *ui;
};

#endif // ROVDATASPLINES_H
