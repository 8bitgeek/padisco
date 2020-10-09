/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#include "pafinder.h"
#include <QUdpSocket>
#include <iostream>

#define inherited QObject

PAFinder::PAFinder(QHostAddress groupAddress, quint16 port, int refreshInterval, QObject *parent)
: inherited(parent)
, mGroupAddress(groupAddress)
, mPort(port)
{
    mRefreshTimer = startTimer(refreshInterval);

    mSenderSocket = new QUdpSocket(this);
    mListenerSocket = new QUdpSocket(this);

    QObject::connect(mListenerSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
    QObject::connect(mListenerSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    QObject::connect(mListenerSocket,SIGNAL(readyRead()),this,SLOT(readDatagrams()));

    mListenerSocket->bind(QHostAddress::AnyIPv4,port,QUdpSocket::ShareAddress);
    mListenerSocket->joinMulticastGroup(groupAddress);

    mSenderSocket->bind(QHostAddress(QHostAddress::AnyIPv4), 0);
}

PAFinder::~PAFinder()
{
    QObject::disconnect(mListenerSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
    QObject::disconnect(mListenerSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    QObject::disconnect(mListenerSocket,SIGNAL(readyRead()),this,SLOT(readDatagrams()));
    delete mSenderSocket;
    delete mListenerSocket;
}

void PAFinder::stateChanged ( QAbstractSocket::SocketState socketState )
{
    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState: //	0	The socket is not connected.
            break;
        case QAbstractSocket::HostLookupState:  //	1	The socket is performing a host name lookup.
            break;
        case QAbstractSocket::ConnectingState:  //	2	The socket has started establishing a connection.
            break;
        case QAbstractSocket::ConnectedState:   //	3	A connection is established.
            break;
        case QAbstractSocket::BoundState:       //	4	The socket is bound to an address and port (for servers).
            #if 0
            {
                QList<QNetworkInterface> mListIfaces = QNetworkInterface::allInterfaces();

                for (int i = 0; i < mListIfaces.length(); ++i)
                {
                    QNetworkInterface iface = mListIfaces.at(i);
                    if ( !(iface.flags() & QNetworkInterface::IsLoopBack)   &&
                         (iface.flags() & QNetworkInterface::CanMulticast)  &&
                         (iface.flags() & QNetworkInterface::IsUp)          &&
                         (iface.flags() & QNetworkInterface::IsRunning)
                       )
                    {
                        bool rez = mListenerSocket->joinMulticastGroup(QHostAddress(PIKEAERO_MULTICAST_GROUP_STRING), iface);
                        qDebug() << iface.name();
                        qDebug() << rez;
                    }
                }
            }
            #endif
            //mListenerSocket->joinMulticastGroup(QHostAddress(PIKEAERO_MULTICAST_GROUP_STRING));
            break;
        case QAbstractSocket::ClosingState:     //	6	The socket is about to close (data may still be waiting to be written).
            break;
        case QAbstractSocket::ListeningState:   //	5	For internal use only.
            break;
    }
}

void PAFinder::error(QAbstractSocket::SocketError socketError)
{
    printf("socket error %d\n",socketError);
    /* FIXME */
}

void PAFinder::processDatagram(QByteArray datagram)
{
    PADevice device(datagram);
    if ( device.packet().type() & PIKEAERO_PAP_TYPE_RESPONSE_MASK )
        emit deviceFound(device);
}

void PAFinder::readDatagrams()
{
    emit receiving();
    while (mListenerSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(mListenerSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        mListenerSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        processDatagram(datagram);
    }
    //emit timeout();
}

void PAFinder::sendDatagram(QByteArray datagram)
{
    emit sending();
    QObject::connect(mSenderSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
    QObject::connect(mSenderSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    //QObject::connect(mSenderSocket,SIGNAL(readyRead()),this,SLOT(readDatagrams()));
    mSenderSocket->writeDatagram(datagram.data(),datagram.size(),mGroupAddress,mPort);
}

void PAFinder::sendResetRequest(PAPacket& packet)
{
    emit sending();
    packet.setType(PIKEAERO_PAP_TYPE_RESET_REQUEST);

    QByteArray datagram = packet.toByteArray();
    mSenderSocket->writeDatagram(datagram.data(),datagram.size(),mGroupAddress,mPort);
}

void PAFinder::sendApplyRequest(PAPacket& packet)
{
    emit sending();
    packet.setType(PIKEAERO_PAP_TYPE_IP_SETTINGS_REQUEST);

    QByteArray datagram = packet.toByteArray();
    mSenderSocket->writeDatagram(datagram.data(),datagram.size(),mGroupAddress,mPort);
}

void PAFinder::sendFinderRequest()
{
    emit sending();
    PAPacket packet;
    packet.setType(PIKEAERO_PAP_TYPE_DISCOVER_REQUEST);

    QByteArray datagram = packet.toByteArray();
    mSenderSocket->writeDatagram(datagram.data(),datagram.size(),mGroupAddress,mPort);
}

void PAFinder::timerEvent(QTimerEvent *e)
{
    if ( e->timerId() == mRefreshTimer )
    {
        //sendFinderRequest();
    }
}
