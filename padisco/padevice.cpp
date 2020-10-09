/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#include "padevice.h"

#define inherited QObject

PADevice::PADevice(QObject *parent)
: inherited(parent)
{
}

PADevice::PADevice(QByteArray replyDatagram)
{
   mPacket.fromByteArray(replyDatagram);
}

PADevice::PADevice(const PADevice& other)
: inherited()
{
    mPacket = other.mPacket;
}

PADevice::~PADevice()
{
}

bool PADevice::operator==( const PADevice& other )
{
    PADevice* p = (PADevice*)&other;
    if ( macAddress() == p->macAddress() )
        return true;
    return false;
}

bool PADevice::operator!=( const PADevice& other )
{
    PADevice* p = (PADevice*)&other;
    if ( macAddress() == p->macAddress() )
        return false;
    return true;
}

PADevice& PADevice::operator=( const PADevice& other )
{
    mPacket = other.mPacket;
    return *this;
}

QDataStream& PADevice::operator<<( QDataStream & out )
{
    out << mPacket.toByteArray();
    return out;
}

QDataStream& PADevice::operator>>( QDataStream & in )
{
    QByteArray bytes;
    in >> bytes;
    return in;
}

QString PADevice::hostname()
{
    return mPacket.hostname();
}

QByteArray PADevice::macAddress()
{
    return mPacket.macAddress();
}

QString PADevice::macAddressString()
{
    QByteArray mac = macAddress();
    QString rc;
    for(int n=0; n < mac.count(); n++)
    {
        if (rc != "")
            rc += ":";
        quint8 ch = mac.at(n);
        QString hex;
        hex.sprintf("%02X",ch);
        rc += hex;
    }
    return rc;
}

void PADevice::setDHCP(bool dhcp)
{
   mPacket.setDHCP(dhcp);
}

bool PADevice::dhcp()
{
    bool dhcp = false;
    if(mPacket.dhcp() == 1)
        dhcp = true;
    return dhcp;
}

QByteArray PADevice::altMacAddress()
{
    return mPacket.altMacAddress();
}

QString PADevice::altMacAddressString()
{
    QByteArray mac = altMacAddress();
    QString rc;
    for(int n=0; n < mac.count(); n++)
    {
        if (rc != "")
            rc += ":";
        quint8 ch = mac.at(n);
        QString hex;
        hex.sprintf("%02X",ch);
        rc += hex;
    }
    return rc;
}

void PADevice::setSetMac(bool setmac)
{
   mPacket.setSetMac(setmac);
}

bool PADevice::setmac()
{
    bool setmac = false;
    if(mPacket.setmac() == 1)
        setmac = true;
    return setmac;
}

void PADevice::setIPAddress(QHostAddress ipAddress)
{
    mPacket.setIpAddress(ipAddress);
}

QHostAddress PADevice::ipAddress()
{
    return mPacket.ipAddress();
}

void PADevice::setSubnetMask(QHostAddress subnetMask)
{
    mPacket.setNetMask(subnetMask);
}

QHostAddress PADevice::subnetMask()
{
    return mPacket.netMask();
}

void PADevice::setGatewayAddress(QHostAddress gatewayAddress)
{
    mPacket.setGateway(gatewayAddress);
}

QHostAddress PADevice::gatewayAddress()
{
    return mPacket.gateway();
}

QByteArray PADevice::toByteArray()
{
    return mPacket.toByteArray();
}

void PADevice::fromByteArray(QByteArray bytes)
{
    mPacket.fromByteArray(bytes);
}

void PADevice::setSettingsRequest()
{
    mPacket.setType(PIKEAERO_PAP_TYPE_IP_SETTINGS_REQUEST);
}

QString PADevice::toString()
{
    QString rc;
    rc += macAddressString() + ":";     rc += " ";
    rc += dhcp()?"1":"0";               rc += " ";
    rc += ipAddress().toString();       rc += " ";
    rc += subnetMask().toString();      rc += " ";
    rc += gatewayAddress().toString();  rc += " ";
    return rc;
}

PAPacket& PADevice::packet()
{
    return mPacket;
}

