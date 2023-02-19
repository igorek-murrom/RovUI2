#ifndef ROVDATAPARSER_H
#define ROVDATAPARSER_H

#include <QObject>
#include <QDebug>
#include <QDataStream>
#include "joystick.h"
#include "rovdatatypes.h"

class RovDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RovDataParser(QObject *parent = nullptr);

signals:
    void telemetryReady(RovTelemetry);
    void controlReady(QByteArray);
public slots:
    void doProcessTelemetry(QByteArray);
    void doPrepareDatagram(Joystick);

private:
    constexpr static short thrusterDirections[10] = {1,1,1,1,1,1,1,1,1,1};
    static const qint16 poly_val = 0x1021;
    static const qint16 seed_val = 0xFFFF;

    qint16 updateCRC(qint16 acc, const qint8 input)
    {
        acc ^= (input << 8);
        for (qint8 i = 0; i < 8; i++) {
            if ((acc & 0x8000) == 0x8000) {
                acc <<= 1;
                acc ^= poly_val;
            } else {
                acc <<= 1;
            }
        }
        return acc;
    }

    qint16 calculateCRC(const char* data, const size_t len)
    {
        qint16 crcout = seed_val;

        for (size_t i = 0; i < len; ++i) {
            crcout = updateCRC(crcout, data[i]);
        }

        return crcout;
    }



};

#endif // ROVDATAPARSER_H
