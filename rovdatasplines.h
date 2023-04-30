#ifndef ROVDATASPLINES_H
#define ROVDATASPLINES_H

#include <QDialog>
#include <QtCharts>

/**
 * \brief The ContinousDataSplineChart class is responsible for simplifying the
 * work with graphs
 */
class ContinousDataSplineChart : public QChart {
    Q_OBJECT
  public:
    /**
     * \brief Getter for m_maxSamples
     */
    int maxSamples() { return m_maxSamples; };

    /**
     * \brief Setter for m_maxSamples
     * \param samples Samples
     */
    void setMaxSamples(int samples) { m_maxSamples = samples; };

    /**
     * \brief ContinousDataSplineChart
     * \param minVal Minimum Y value that should appear in addSample
     * \param maxVal Maximum Y value that should appear in addSample
     * \param ticksCount Y axis tick count
     * \param minorTicksCount Y axis minor tick count
     */
    explicit ContinousDataSplineChart(int minVal, int maxVal, int ticksCount,
                                      int minorTicksCount);

  public slots:
    /**
     * \brief Adds a sample to the chart
     * \param sample Sample
     */
    void addSample(QPointF sample);

  private:
    /**
     * \brief Spline series with data points
     */
    QScopedPointer<QSplineSeries> m_splineSeries;

    /**
     * \brief Vertical axis of the chart
     */
    QScopedPointer<QValueAxis> m_vertAxis;
    /**
     * \brief Horizontal axis of the chart
     */
    QScopedPointer<QValueAxis> m_horAxis;

    /**
     * \brief maximum amount of samples
     */
    int m_maxSamples;
};

namespace Ui {
    class RovDataSplines;
}

/**
 * \brief The RovDataSplines class is responsible for pretty graphs
 * \todo transition to ContinousDataSplineChart and make an ROV angles
 * visualisation
 */
class RovDataSplines : public QDialog {
    Q_OBJECT

  public:
    /**
     * \brief Default constructor
     * \param parent Parent
     */
    explicit RovDataSplines(QWidget *parent = nullptr);

    /**
     * \brief default destructor
     */
    ~RovDataSplines();

  public slots:
    /**
     * \brief Adds a sample to the voltage chart
     * \param sample You guessed it, sample
     */
    void addVoltageSample(QPointF sample);

    /**
     * \brief Adds a sample to the current chart
     * \param sample You guessed it, sample
     */
    void addCurrentSample(QPointF sample);

  private:
    /**
     * \brief Voltage chart
     */
    QScopedPointer<ContinousDataSplineChart> m_vChart;

    /**
     * \brief Current chart
     */
    QScopedPointer<ContinousDataSplineChart> m_cChart;

    /**
     * \brief maxVSamples
     */
    int maxVSamples = 300;
    int maxCSamples = 300;

    /**
     * \brief UI object
     */
    Ui::RovDataSplines *ui;
};

#endif // ROVDATASPLINES_H
