/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#ifndef PAFINDER_H
#define PAFINDER_H

#include <QObject>
#include <QTimerEvent>
#include <QList>
#include <QtNetwork>
#include <QHostAddress>
#include "padevice.h"

class PAFinder : public QObject
{
    Q_OBJECT
    public:
        PAFinder(QHostAddress groupAddress, quint16 port, int refreshInterval=5000, QObject *parent = 0);
        virtual ~PAFinder();

    signals:
        void                    sending();
        void                    receiving();
        void                    timeout();
        void                    deviceFound(PADevice device);

    public slots:
        virtual void            sendDatagram(QByteArray datagram);
        virtual void            sendFinderRequest();
        virtual void            sendApplyRequest(PAPacket& packet);
        virtual void            sendResetRequest(PAPacket& packet);

    private slots:
        void                    readDatagrams();
        void                    processDatagram(QByteArray datagram);
        void                    stateChanged ( QAbstractSocket::SocketState socketState );
        void                    error(QAbstractSocket::SocketError socketError);

    protected:
        virtual void            timerEvent(QTimerEvent *);

    private:
        QHostAddress            mGroupAddress;
        quint16                 mPort;
        QUdpSocket*             mSenderSocket;
        QUdpSocket*             mListenerSocket;
        int                     mRefreshTimer;
};

#endif // PAFINDER_H
