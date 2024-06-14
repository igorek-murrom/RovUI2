// SPDX-License-Inentifier: Apache-2.0 and MIT
/*
 * HiPNUC CHManager Instrument Clusters used to make
 * the Attitude Indicator widget and Compass widget.
 *
 * Copyright (C) 2018 Marek M. Cel, Dave Culp
 * Copyright (C) 2016-2020 HiPNUC All rights reserved.
 *
 */

#include "instrumentcluster.h"

/**
 * @brief CHInstrumentWidget::CHInstrumentWidget - Constructs a flight
 * instrument with a title and a parent.
 * @param title
 * @param parent
 */
InstrumentWidget::InstrumentWidget(const QString &title, QWidget *parent)
    : QWidget(parent), m_adi(0), layout(0) {
    QGraphicsView *graphics = nullptr;
    this->resize(400, 300);

    int result =
        QString::compare(title, "AttitudeIndicator", Qt::CaseInsensitive);

    this->result = result;
    if (!result) {
        graphics = new AttitudeIndicator(this);
        m_adi    = (AttitudeIndicator *)graphics;
    } else {
        graphics = new Compass(this);
        m_hsi    = (Compass *)graphics;
    }

    graphics->setObjectName(title);
    graphics->setEnabled(false);
    graphics->setGeometry(QRect(70, 40, 51, 31));
    graphics->setFrameShape(QFrame::NoFrame);
    graphics->setFrameShadow(QFrame::Plain);
    graphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphics->setInteractive(false);

    layout = new InstrumentLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(graphics);

    setLayout(layout);
}

InstrumentWidget::~InstrumentWidget() {}

void InstrumentWidget::setData(float yaw, float roll, float pitch) {
    if (result)
        m_hsi->setYaw(yaw);
    else
        m_adi->setData(roll, pitch);
}

void InstrumentWidget::updateView(float yaw, float roll, float pitch) {

    this->setData(yaw, roll, pitch);
}

void InstrumentWidget::transaction() {}// timer->start(100); }

/**
 * @brief CHAttitudeIndicator::CHAttitudeIndicator
 * @param parent
 */
AttitudeIndicator::AttitudeIndicator(QWidget *parent)
    : QGraphicsView(parent), m_scene(nullptr), m_itemBack(nullptr),
      m_itemFace(nullptr), m_itemRing(nullptr), m_itemCase(nullptr),
      m_roll(0.0f), m_pitch(0.0f), m_faceDeltaX_new(0.0f),
      m_faceDeltaX_old(0.0f), m_faceDeltaY_new(0.0f), m_faceDeltaY_old(0.0f),
      m_scaleX(1.0f), m_scaleY(1.0f), m_originalHeight(240),
      m_originalWidth(240), m_originalPixPerDeg(1.7f),
      m_originalAdiCtr(120.0f, 120.0f), m_backZ(-30), m_faceZ(-20),
      m_ringZ(-10), m_caseZ(10) {

    reset();

    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    m_scene->clear();
    init();
}

AttitudeIndicator::~AttitudeIndicator() {
    if (m_scene != nullptr) {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }

    reset();
}

void AttitudeIndicator::reinit() {
    if (m_scene) {
        m_scene->clear();
        init();
    }
}

void AttitudeIndicator::update() {
    updateView();

    m_faceDeltaX_old = m_faceDeltaX_new;
    m_faceDeltaY_old = m_faceDeltaY_new;
}

void AttitudeIndicator::setData(float roll, float pitch) {
    m_roll  = roll;
    m_pitch = pitch;

    if (m_roll < -180.0f)
        m_roll = -180.0f;
    if (m_roll > 180.0f)
        m_roll = 180.0f;

    if (m_pitch < -25.0f)
        m_pitch = -25.0f;
    if (m_pitch > 25.0f)
        m_pitch = 25.0f;

    update();
}

void AttitudeIndicator::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    reinit();
}

void AttitudeIndicator::init() {
    m_scaleX = (float)width() / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    reset();

    m_itemBack = new QGraphicsSvgItem(":/svg/compass/adi/adi_back.svg");
    m_itemBack->setCacheMode(QGraphicsItem::NoCache);
    m_itemBack->setZValue(m_backZ);
    m_itemBack->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_itemBack->setTransformOriginPoint(m_originalAdiCtr);
    m_scene->addItem(m_itemBack);

    m_itemFace = new QGraphicsSvgItem(":/svg/compass/adi/adi_face.svg");
    m_itemFace->setCacheMode(QGraphicsItem::NoCache);
    m_itemFace->setZValue(m_faceZ);
    m_itemFace->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_itemFace->setTransformOriginPoint(m_originalAdiCtr);
    m_scene->addItem(m_itemFace);

    m_itemRing = new QGraphicsSvgItem(":/svg/compass/adi/adi_ring.svg");
    m_itemRing->setCacheMode(QGraphicsItem::NoCache);
    m_itemRing->setZValue(m_ringZ);
    m_itemRing->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_itemRing->setTransformOriginPoint(m_originalAdiCtr);
    m_scene->addItem(m_itemRing);

    m_itemCase = new QGraphicsSvgItem(":/svg/compass/adi/adi_case.svg");
    m_itemCase->setCacheMode(QGraphicsItem::NoCache);
    m_itemCase->setZValue(m_caseZ);
    m_itemCase->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_scene->addItem(m_itemCase);

    centerOn(width() / 2.0f, height() / 2.0f);

    updateView();
}

void AttitudeIndicator::reset() {
    m_itemBack = nullptr;
    m_itemFace = nullptr;
    m_itemRing = nullptr;
    m_itemCase = nullptr;

    m_roll  = 0.0f;
    m_pitch = 0.0f;

    m_faceDeltaX_new = 0.0f;
    m_faceDeltaX_old = 0.0f;
    m_faceDeltaY_new = 0.0f;
    m_faceDeltaY_old = 0.0f;
}

void AttitudeIndicator::updateView() {
    m_scaleX = (float)width() / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    m_itemBack->setRotation(-m_roll);
    m_itemFace->setRotation(-m_roll);
    m_itemRing->setRotation(-m_roll);

    float roll_rad = M_PI * m_roll / 180.0;

    float delta = m_originalPixPerDeg * m_pitch;

    m_faceDeltaX_new = m_scaleX * delta * sin(roll_rad);
    m_faceDeltaY_new = m_scaleY * delta * cos(roll_rad);

    m_itemFace->moveBy(m_faceDeltaX_new - m_faceDeltaX_old,
                       m_faceDeltaY_new - m_faceDeltaY_old);

    m_scene->update();
}

Compass::Compass(QWidget *parent)
    : QGraphicsView(parent), m_scene(nullptr), m_itemFace(nullptr),
      m_itemCase(nullptr), m_yaw(0.0f), m_scaleX(1.0f), m_scaleY(1.0f),
      m_originalHeight(240), m_originalWidth(240),
      m_originalHsiCtr(120.0f, 120.0f), m_faceZ(-20), m_caseZ(10) {

    reset();

    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    m_scene->clear();
    init();
}

Compass::~Compass() {
    if (m_scene != nullptr) {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }

    reset();
}

void Compass::reinit() {
    if (m_scene) {
        m_scene->clear();
        init();
    }
}

void Compass::update() { updateView(); }

void Compass::setYaw(float yaw) {
    m_yaw = yaw;
    update();
}

void Compass::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);

    reinit();
}

void Compass::init() {
    m_scaleX = (float)width() / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    reset();

    m_itemFace = new QGraphicsSvgItem(":/svg/compass/hsi/hsi_face.svg");
    m_itemFace->setCacheMode(QGraphicsItem::NoCache);
    m_itemFace->setZValue(m_faceZ);
    m_itemFace->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_itemFace->setTransformOriginPoint(m_originalHsiCtr);
    m_scene->addItem(m_itemFace);

    m_itemCase = new QGraphicsSvgItem(":/svg/compass/hsi/hsi_case.svg");
    m_itemCase->setCacheMode(QGraphicsItem::NoCache);
    m_itemCase->setZValue(m_caseZ);
    m_itemCase->setTransform(QTransform::fromScale(m_scaleX, m_scaleY), true);
    m_scene->addItem(m_itemCase);

    centerOn(width() / 2.0f, height() / 2.0f);

    updateView();
}

void Compass::reset() {
    m_itemFace = nullptr;
    m_itemCase = nullptr;

    m_yaw = 0.0f;
}

void Compass::updateView() {
    m_itemFace->setRotation(-m_yaw);
    m_scene->update();
}

/**
 * @brief CHInstrumentLayout::CHInstrumentLayout
 * @param parent
 * @param spacing
 */
InstrumentLayout::InstrumentLayout(QWidget *parent, int spacing)
    : QLayout(parent) {
    init(spacing);
}

InstrumentLayout::InstrumentLayout(int spacing) : QLayout(0) {
    init(spacing);
}

InstrumentLayout::~InstrumentLayout() {
    if (m_item)
        delete m_item;

    m_item = 0;

    if (m_rectLast)
        delete m_rectLast;

    m_rectLast = 0;

    if (m_geometry)
        delete m_geometry;

    m_geometry = 0;
}

void InstrumentLayout::addItem(QLayoutItem *item) {
    if (!hasItem())
        replaceItem(item);
}

void InstrumentLayout::addWidget(QWidget *widget) {
    if (!hasItem())
        replaceItem(new QWidgetItem(widget));
}

int InstrumentLayout::count() const {
    if (hasItem())
        return 1;

    return 0;
}

Qt::Orientations InstrumentLayout::expandingDirections() const {
    return (Qt::Horizontal | Qt::Vertical);
}

QRect InstrumentLayout::geometry() { return (QRect)(*m_geometry); }

bool InstrumentLayout::hasHeightForWidth() const { return false; }

bool InstrumentLayout::hasItem() const { return (m_item != 0); }

QLayoutItem *InstrumentLayout::itemAt(int index) const {
    if (index == 0) {
        if (hasItem())
            return m_item;
    }

    return 0;
}

QSize InstrumentLayout::minimumSize() const { return m_item->minimumSize(); }

QLayoutItem *InstrumentLayout::replaceItem(QLayoutItem *item) {
    QLayoutItem *tempItem = 0;

    if (hasItem())
        tempItem = m_item;

    m_item = item;

    setGeometry(*m_geometry);

    return tempItem;
}

void InstrumentLayout::setGeometry(const QRect &rect) {

    if (!hasItem() || areRectsEqual(*m_rectLast, rect))
        return;

    setRectLast(rect);

    QSize  properSize  = calculateProperSize(rect.size());
    QPoint centerPoint = calculateCenterPnt(rect.size(), properSize);

    m_item->setGeometry(QRect(centerPoint, properSize));

    QRect *tempRect = m_geometry;

    m_geometry = new QRect(centerPoint, properSize);

    delete tempRect;

    QLayout::setGeometry(*m_geometry);
}

QSize InstrumentLayout::sizeHint() const { return m_item->minimumSize(); }

QLayoutItem *InstrumentLayout::take() {
    QLayoutItem *tempItem = 0;

    if (hasItem()) {
        tempItem = m_item;
        m_item   = 0;
    }

    return tempItem;
}

QLayoutItem *InstrumentLayout::takeAt(int index) {
    if (index == 0)
        return take();

    return 0;
}

bool InstrumentLayout::areRectsEqual(const QRect &rect_1,
                                       const QRect &rect_2) const {
    bool result = false;

    if ((rect_1.x() == rect_2.x()) && (rect_1.y() == rect_2.y()) &&
        (rect_1.height() == rect_2.height()) &&
        (rect_1.width() == rect_2.width())) {
        result = true;
    }

    return result;
}

QPoint InstrumentLayout::calculateCenterPnt(QSize fromSize,
                                              QSize itemSize) const {
    QPoint centerPoint;

    if ((fromSize.width() - fromSize.width() / 2.0 - itemSize.width() / 2.0) >
        0.0) {
        centerPoint.setX(fromSize.width() - fromSize.width() / 2.0 -
                         itemSize.width() / 2.0);
    }

    if ((fromSize.height() - fromSize.height() / 2.0 -
         itemSize.height() / 2.0) > 0.0) {
        centerPoint.setY(fromSize.height() - fromSize.height() / 2.0 -
                         itemSize.height() / 2.0);
    }

    return centerPoint;
}

QSize InstrumentLayout::calculateProperSize(QSize fromSize) const {
    QSize resultSize;

    if (fromSize.height() < fromSize.width()) {
        resultSize.setHeight(fromSize.height() - margin());
        resultSize.setWidth(fromSize.height() - margin());
    } else {
        resultSize.setHeight(fromSize.width() - margin());
        resultSize.setWidth(fromSize.width() - margin());
    }

    return resultSize;
}

void InstrumentLayout::init(int spacing) {
    m_item = 0;

    m_rectLast = new QRect(0, 0, 0, 0);
    m_geometry = new QRect(0, 0, 0, 0);

    setSpacing(spacing);
}

void InstrumentLayout::setRectLast(const QRect &rect) {
    QRect *tempRect = m_rectLast;

    m_rectLast = new QRect(rect.topLeft(), rect.size());
    delete tempRect;
}
