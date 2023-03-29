#include "rovdatasplines.h"
#include "ui_rovdatasplines.h"




RovDataSplines::RovDataSplines(QWidget *parent) :
    QDialog(parent),
    m_vChart(new ContinousDataSplineChart(0, 12, 7, 2)),
    m_cChart(new ContinousDataSplineChart(0, 30, 6, 1)),
    ui(new Ui::RovDataSplines)
{
    ui->setupUi(this);


    ui->voltageChartView->setChart(m_vChart.data());
    ui->currentChartView->setChart(m_cChart.data());
}

void RovDataSplines::addVoltageSample(QPointF sample){
    m_vChart->addSample(sample);
}
void RovDataSplines::addCurrentSample(QPointF sample){
    m_cChart->addSample(sample);
}

RovDataSplines::~RovDataSplines()
{
    delete ui;
}

ContinousDataSplineChart::ContinousDataSplineChart(int minVal, int maxVal, int ticksCount, int minorTicksCount) :
    QChart(new QChart()),
    m_splineSeries(new QSplineSeries(this)),
    m_vertAxis(new QValueAxis),
    m_horAxis(new QValueAxis),
    m_maxSamples(100)
{

    addSeries(m_splineSeries.data());

    m_vertAxis->setRange(minVal, maxVal);
    m_vertAxis->setTickCount(ticksCount);
    m_vertAxis->setMinorTickCount(minorTicksCount);

    m_horAxis->setRange(0, m_maxSamples);

    legend()->hide();

    setTitle("Continous chart (" + QString::number(minVal) + "," + QString::number(maxVal) + ")");

    addAxis(m_horAxis.data(), Qt::AlignBottom);
    addAxis(m_vertAxis.data(), Qt::AlignLeft);

    m_splineSeries->attachAxis(m_horAxis.data());
    m_splineSeries->attachAxis(m_vertAxis.data());

    m_horAxis->hide();

}

void ContinousDataSplineChart::addSample(QPointF sample){
    m_splineSeries->append(sample);
    if (m_splineSeries->count()>=m_maxSamples){
        m_splineSeries->remove(0);
    }
    int p = m_splineSeries->at(0).x();
    int q = m_splineSeries->at(m_splineSeries->count()).x();
    m_horAxis->setRange(p, q);
}
