// SPDX-License-Inentifier: Apache-2.0 and MIT
/*
 * HiPNUC CHManager Instrument Clusters used to make
 * the Attitude Indicator widget and Compass widget.
 *
 * Copyright (C) 2018 Marek M. Cel, Dave Culp
 * Copyright (C) 2016-2020 HiPNUC All rights reserved.
 *
 */

#ifndef CHINSTRUMENTCLUSTER_H
#define CHINSTRUMENTCLUSTER_H

#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QLayout>
#include <QLayoutItem>
#include <QPointer>
#include <QRect>
#include <QWidget>
#include <QWidgetItem>

class AttitudeIndicator;
class Compass;
class InstrumentLayout;

/**
 * @class CHInstrumentWidget
 * @brief The CHInstrumentWidget class provides a basic widgets of flight
 * instruments, including Attitude Indicator and Compass.
 */
class InstrumentWidget : public QWidget {
    Q_OBJECT
  public:
    explicit InstrumentWidget(const QString &title, QWidget *parent = nullptr);
    ~InstrumentWidget();

    void transaction();

  private:
    void setData(float yaw, float roll, float pitch);

    QGraphicsView     *view;
    AttitudeIndicator *m_adi;
    Compass           *m_hsi;
    InstrumentLayout  *layout;
    QTimer            *timer;
    int                result;

  public slots:
    void updateView(float yaw, float roll, float pitch);
};

/**
 * @brief The Attitude Indicator class
 */
class AttitudeIndicator : public QGraphicsView {
    Q_OBJECT
  public:
    explicit AttitudeIndicator(QWidget *parent = nullptr);
    ~AttitudeIndicator();

    void setData(float roll, float pitch);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QGraphicsScene *m_scene;

    QGraphicsSvgItem *m_itemBack;
    QGraphicsSvgItem *m_itemFace;
    QGraphicsSvgItem *m_itemRing;
    QGraphicsSvgItem *m_itemCase;

    float m_roll;
    float m_pitch;

    float m_faceDeltaX_new;
    float m_faceDeltaX_old;
    float m_faceDeltaY_new;
    float m_faceDeltaY_old;

    float m_scaleX;
    float m_scaleY;

    const int m_originalHeight;
    const int m_originalWidth;

    const float m_originalPixPerDeg;

    QPointF m_originalAdiCtr;

    const int m_backZ;
    const int m_faceZ;
    const int m_ringZ;
    const int m_caseZ;

    void init();
    void reinit();
    void reset();
    void update();
    void updateView();
};

/**
 * @brief The CHCompass class
 */
class Compass : public QGraphicsView {
    Q_OBJECT
  public:
    explicit Compass(QWidget *parent = nullptr);
    ~Compass();

    void setYaw(float yaw);

  protected:
    void resizeEvent(QResizeEvent *event);

  private:
    QGraphicsScene   *m_scene;
    QGraphicsSvgItem *m_itemFace;
    QGraphicsSvgItem *m_itemCase;

    float m_yaw;

    float m_scaleX;
    float m_scaleY;

    const int m_originalHeight;
    const int m_originalWidth;

    QPointF m_originalHsiCtr;

    const int m_faceZ;
    const int m_caseZ;

    void init();
    void reset();
    void reinit();
    void update();
    void updateView();
};

/**
 * @brief The CHInstrumentLayout class
 */
class InstrumentLayout : public QLayout {
    Q_OBJECT

  public:
    explicit InstrumentLayout(QWidget *parent, int spacing = -1);
    explicit InstrumentLayout(int spacing = -1);
    ~InstrumentLayout();

    void addItem(QLayoutItem *item);
    void addWidget(QWidget *widget);
    int  count() const;

    Qt::Orientations expandingDirections() const;

    QRect geometry();

    bool hasHeightForWidth() const;
    bool hasItem() const;

    QLayoutItem *itemAt(int index) const;
    QSize        minimumSize() const;
    QLayoutItem *replaceItem(QLayoutItem *item);

    void setGeometry(const QRect &rect);

    QSize        sizeHint() const;
    QLayoutItem *take();
    QLayoutItem *takeAt(int index);

  private:
    QLayoutItem *m_item;
    QRect       *m_rectLast;
    QRect       *m_geometry;

    bool areRectsEqual(const QRect &rect_1, const QRect &rect_2) const;

    QPoint calculateCenterPnt(QSize fromSize, QSize itemSize) const;
    QSize  calculateProperSize(QSize fromSize) const;

    void init(int spacing);
    void setRectLast(const QRect &rect);
};

#endif // CHINSTRUMENTCLUSTER_H
