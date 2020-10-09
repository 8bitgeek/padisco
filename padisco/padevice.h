/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#ifndef PADEVICE_H
#define PADEVICE_H

#include <QObject>
#include <QString>
#include <QtNetwork>
#include <QDataStream>

#include "papacket.h"

class PADevice : public QObject
{
    Q_OBJECT
    public:
        PADevice(QObject *parent = 0);
        PADevice(QByteArray replyDatagram);
        PADevice(const PADevice& other);
        virtual ~PADevice();

        bool					operator!=( const PADevice& other );
        bool					operator==( const PADevice& other );
        PADevice&               operator=( const PADevice& other );
        QDataStream &           operator<<( QDataStream & out );
        QDataStream &           operator>>( QDataStream & in );

        virtual QString         hostname();

        virtual QByteArray      macAddress();
        virtual QString         macAddressString();

        virtual void            setDHCP(bool dhcp);
        virtual bool            dhcp();

        virtual QByteArray      altMacAddress();
        virtual QString         altMacAddressString();

        virtual void            setSetMac(bool setmac);
        virtual bool            setmac();

        virtual void            setIPAddress(QHostAddress ipAddress);
        virtual QHostAddress    ipAddress();

        virtual void            setSubnetMask(QHostAddress subnetMask);
        virtual QHostAddress    subnetMask();

        virtual void            setGatewayAddress(QHostAddress gatewayAddress);
        virtual QHostAddress    gatewayAddress();

        QByteArray              toByteArray();
        void                    fromByteArray(QByteArray bytes);
        void                    setSettingsRequest();

        QString                 toString();

        PAPacket&               packet();

    private:

        PAPacket                 mPacket;
};

#endif // PADEVICE_H
