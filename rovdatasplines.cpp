#include "rovdatasplines.h"
#include "ui_rovdatasplines.h"




RovDataSplines::RovDataSplines(QWidget *parent) :
    QDialog(parent),
    m_voltageAxisV(new QValueAxis()),
    m_voltageAxisH(new QValueAxis()),
    m_currentAxisV(new QValueAxis()),
    m_currentAxisH(new QValueAxis()),
    m_voltageSeries(new QSplineSeries(this)),
    m_currentSeries(new QSplineSeries(this)),
    m_voltageChart(new QChart()),
    m_currentChart(new QChart()),
    ui(new Ui::RovDataSplines)
{
    ui->setupUi(this);
    m_voltageAxisV->setRange(5,15);
    m_voltageAxisV->setTickCount(11);
    m_voltageAxisV->setMinorTickCount(3);

    m_currentAxisV->setRange(0,30);
    m_currentAxisV->setTickCount(13);
    m_currentAxisV->setMinorTickCount(4);

    m_voltageSeries->setName("Voltage");
    m_currentSeries->setName("Current");

    m_voltageChart->legend()->hide();
    m_currentChart->legend()->hide();

    m_voltageChart->addSeries(m_voltageSeries);
    m_currentChart->addSeries(m_currentSeries);

    m_voltageChart->setTitle("Voltage chart");
    m_currentChart->setTitle("Current chart");


    m_voltageChart->addAxis(m_voltageAxisH, Qt::AlignBottom);
    m_voltageChart->addAxis(m_voltageAxisV, Qt::AlignLeft);

    m_voltageSeries->attachAxis(m_voltageAxisH);
    m_voltageSeries->attachAxis(m_voltageAxisV);

    m_currentChart->addAxis(m_currentAxisH, Qt::AlignBottom);
    m_currentChart->addAxis(m_currentAxisV, Qt::AlignLeft);

    m_currentSeries->attachAxis(m_currentAxisH);
    m_currentSeries->attachAxis(m_currentAxisV);

    m_voltageChart->axes(Qt::Horizontal).first()->hide();
    m_currentChart->axes(Qt::Horizontal).first()->hide();

    m_voltageChart.data()->setTheme(QChart::ChartThemeQt);
    m_currentChart.data()->setTheme(QChart::ChartThemeQt);


    ui->voltageChartView->setChart(m_voltageChart.data());
    ui->currentChartView->setChart(m_currentChart.data());
}

void RovDataSplines::addVoltageSample(QPointF sample){
    m_voltageSeries->append(sample);
    if (m_voltageSeries->count()>=maxVSamples){
        m_voltageSeries->remove(0);
    }
    int rangeB = m_voltageSeries->at(0).x();
    int rangeE = m_voltageSeries->at(m_voltageSeries->count()).x();
    m_voltageChart->axes(Qt::Horizontal).front()->setRange(rangeB, rangeE);
}
void RovDataSplines::addCurrentSample(QPointF sample){
    m_currentSeries->append(sample);
    if (m_currentSeries->count()>=maxCSamples){
        m_currentSeries->remove(0);
    }
    int rangeB = m_currentSeries->at(0).x();
    int rangeE = m_currentSeries->at(m_currentSeries->count()).x();
    m_currentChart->axes(Qt::Horizontal).front()->setRange(rangeB, rangeE);
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

    m_vertAxis->setRange(minVal, maxVal);
    m_vertAxis->setTickCount(ticksCount);
    m_vertAxis->setMinorTickCount(minorTicksCount);

    m_horAxis->setRange(0, m_maxSamples);

    legend()->hide();

    setTitle("Continous chart (" + QString(minVal) + "," + QString(maxVal) + ")");

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

void ContinousDataSplineChart::setMaxSamples(qint8 num){
    m_maxSamples = num;
}
