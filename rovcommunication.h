#ifndef ROVCOMMUNICATION_H
#define ROVCOMMUNICATION_H

#include <QObject>
#include <QtNetwork>

/**
 * \brief The RovCommunication class is responsible for transmitting data to the
 * ROV via UDP
 */
class RovCommunication : public QObject {
    Q_OBJECT
  public:
    /**
     * \brief Default constructor
     * \param parent Parent
     */
    explicit RovCommunication(QObject *parent = nullptr);

  signals:
    /**
     * \brief Emitted when a packet with telemetry is recieved
     * \param tele Telemetry
     */
    void telemetryReady(QByteArray tele);

    /**
     * \brief unused
     * \see RovHeartBeat
     */
    void telemetryStopped();

    /**
     * \brief unused
     * \see RovHeartBeat
     */
    void telemetryStarted();
  public slots:

    /**
     * \brief Write data to the UDP socket
     */
    void write(QByteArray);
  private slots:

    /**
     * \brief Read data from the UDP socket if available
     */
    void read();

  private:
    /**
     * \brief Incoming data port
     */
    quint16      m_rcPort     = 46176;
    /**
     * \brief Outcoming data port
     */
    quint16      m_rovPort    = 46175;
    /**
     * \brief ROV address
     */
    QHostAddress m_rovAddress = QHostAddress("192.168.1.5");

    /**
     * \brief The UDP socket
     */
    QScopedPointer<QUdpSocket> m_udpSocket;
};

#endif // ROVCOMMUNICATION_H
