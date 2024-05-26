#include "rovdatasplines.h"
#include "ui_rovdatasplines.h"

RovDataSplines::RovDataSplines(QWidget *parent)
    : QDialog(parent), m_vChart(new ContinousDataSplineChart(0, 30, 16, 1)),
      m_cChart(new ContinousDataSplineChart(0, 30, 16, 1)),
      m_yChart(new ContinousDataSplineChart(0, 360, 16, 1)),
      m_dChart(new ContinousDataSplineChart(-1, 5, 16, 1)),
      m_pChart(new ContinousDataSplineChart(-100, 100, 16, 1)),
      m_rChart(new ContinousDataSplineChart(-100, 100, 16, 1)),
      ui(new Ui::RovDataSplines) {
    ui->setupUi(this);

    m_vChart->setTitle("Voltage chart");
    m_cChart->setTitle("Current chart");
    m_yChart->setTitle("Yaw chart");
    m_dChart->setTitle("Depth chart");
    m_pChart->setTitle("Pitch chart");
    m_rChart->setTitle("Roll chart");

    ui->voltageChartView->setChart(m_vChart.data());
    ui->currentChartView->setChart(m_cChart.data());

    ui->yawChartView->setChart(m_yChart.data());
    ui->depthChartView->setChart(m_dChart.data());
    ui->pitchChartView->setChart(m_pChart.data());
    ui->rollChartView->setChart(m_rChart.data());

}

void RovDataSplines::addVoltageSample(QPointF sample) {
    m_vChart->addSample(sample);
}
void RovDataSplines::addCurrentSample(QPointF sample) {
    m_cChart->addSample(sample);
}


void RovDataSplines::addYawSample(QPointF sample) {
    m_yChart->addSample(sample);
}

void RovDataSplines::addDepthSample(QPointF sample) {
    m_dChart->addSample(sample);
}

void RovDataSplines::addPitchSample(QPointF sample) {
    m_pChart->addSample(sample);
}

void RovDataSplines::addRollSample(QPointF sample) {
    m_rChart->addSample(sample);
}




RovDataSplines::~RovDataSplines() { delete ui; }

ContinousDataSplineChart::ContinousDataSplineChart(int minVal, int maxVal,
                                                   int ticksCount,
                                                   int minorTicksCount)
    : QChart(new QChart()), m_splineSeries(new QSplineSeries(this)),
      m_vertAxis(new QValueAxis), m_horAxis(new QValueAxis), m_maxSamples(100) {

    addSeries(m_splineSeries.data());

    m_vertAxis->setRange(minVal, maxVal);
    m_vertAxis->setTickCount(ticksCount);
    m_vertAxis->setMinorTickCount(minorTicksCount);

    m_horAxis->setRange(0, m_maxSamples);

    legend()->hide();

    setTitle("Continous chart (" + QString::number(minVal) + "," +
             QString::number(maxVal) + ")");

    addAxis(m_horAxis.data(), Qt::AlignBottom);
    addAxis(m_vertAxis.data(), Qt::AlignLeft);

    m_splineSeries->attachAxis(m_horAxis.data());
    m_splineSeries->attachAxis(m_vertAxis.data());

    m_horAxis->hide();
}

void ContinousDataSplineChart::addSample(QPointF sample) {
    m_splineSeries->append(sample);
    while (m_splineSeries->count() >= m_maxSamples) {
        m_splineSeries->remove(0);
    }
    int p = m_splineSeries->at(0).x();
    int q = m_splineSeries->at(m_splineSeries->count()).x(); // get x coordinates of first and last point
    m_horAxis->setRange(p, q); // set range from first to last
}
