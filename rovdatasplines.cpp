#include "rovdatasplines.h"
#include "ui_rovdatasplines.h"




RovDataSplines::RovDataSplines(QWidget *parent) :
    QDialog(parent),
    m_voltageSeries(new QSplineSeries()),
    m_currentSeries(new QSplineSeries()),
    m_voltageChart(new QChart()),
    m_currentChart(new QChart()),
    ui(new Ui::RovDataSplines)
{
    ui->setupUi(this);

    m_voltageSeries->setName("Voltage");
    m_currentSeries->setName("Current");

    QPen vPen(Qt::red);
    vPen.setWidth(3);
    m_voltageSeries->setPen(vPen);
    QPen aPen(Qt::green);
    aPen.setWidth(3);
    m_currentSeries->setPen(aPen);

    m_voltageChart->legend()->hide();
    m_currentChart->legend()->hide();

    m_voltageChart->addSeries(m_voltageSeries.data());
    m_currentChart->addSeries(m_currentSeries.data());

    m_voltageChart->setTitle("Voltage chart");
    m_currentChart->setTitle("Current chart");

    m_voltageChart->createDefaultAxes();
    m_currentChart->createDefaultAxes();

    m_voltageChart->axes(Qt::Vertical).first()->setRange(0, 12);
    m_currentChart->axes(Qt::Vertical).first()->setRange(0, 25);

    m_voltageChart->axes(Qt::Horizontal).first()->setRange(0, 10);
    m_currentChart->axes(Qt::Horizontal).first()->setRange(0, 10);

    m_voltageChart.data()->setTheme(QChart::ChartThemeQt);
    m_currentChart.data()->setTheme(QChart::ChartThemeQt);


    ui->voltageChartView->setChart(m_voltageChart.data());
    ui->currentChartView->setChart(m_currentChart.data());
}

void RovDataSplines::addVoltageSample(QPointF sample){
    qDebug() << "appending V sample " << sample;
    m_voltageSeries->append(sample);
    if (m_voltageSeries->count()>=50){
        qDebug() << "removing outdated sample" << m_voltageSeries->at(0);
        m_voltageSeries.data()->remove(0);
    }
    int rangeB = m_voltageSeries->at(0).x();
    int rangeE = m_voltageSeries->at(m_voltageSeries->count()).x();
    m_voltageChart->axes(Qt::Horizontal).first()->setRange(rangeB, rangeE);
}
void RovDataSplines::addCurrentSample(QPointF sample){
    qDebug() << "appending A sample " << sample;
    m_currentSeries->append(sample);
    if (m_currentSeries->count()>=50){
        qDebug() << "removing outdated sample" << m_currentSeries->at(0);
        m_currentSeries.data()->remove(0);
    }
    int rangeB = m_currentSeries->at(0).x();
    int rangeE = m_currentSeries->at(m_currentSeries->count()).x();
    m_currentChart->axes(Qt::Horizontal).first()->setRange(rangeB, rangeE);
}

RovDataSplines::~RovDataSplines()
{
    delete ui;
}
