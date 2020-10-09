/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#include "papacket.h"
#include <QStringList>

#define inherited QObject

PAPacket::PAPacket(const PAPacket& other)
: inherited()
{
    memcpy(&mPacket,&other.mPacket,sizeof(mPacket));
}

PAPacket::PAPacket(QObject *parent)
: inherited(parent)
{
    memset(&mPacket,0,sizeof(mPacket));
    memset(mPacket.mac,0xFF,PIKEAERO_PAP_MAC_SZ);
    setSignature();
}

PAPacket::PAPacket(QByteArray raw)
: inherited()
{
    memset(&mPacket,0,sizeof(mPacket));
    memset(mPacket.mac,0xFF,PIKEAERO_PAP_MAC_SZ);
    fromByteArray(raw);
}

PAPacket::~PAPacket()
{
}

PAPacket& PAPacket::operator=( const PAPacket& other )
{
    memcpy(&mPacket,&other.mPacket,sizeof(mPacket));
    return *this;
}

QByteArray PAPacket::toByteArray()
{
    QByteArray raw;
    switch ( type() )
    {
        case PIKEAERO_PAP_TYPE_DISCOVER_REQUEST:            /* A discovery request was received */
            raw.setRawData((char*)&mPacket,sizeof(packet_t));
        break;
        case PIKEAERO_PAP_TYPE_IP_SETTINGS_RESPONSE:        /* A request to change IP settings */
        case PIKEAERO_PAP_TYPE_DISCOVER_RESPONSE:           /* Discovery response sent out */
            raw.setRawData((char*)&mPacket,sizeof(packet_t));
            break;
        case PIKEAERO_PAP_TYPE_IP_SETTINGS_REQUEST:         /* A request to change IP settings */
            raw.setRawData((char*)&mPacket,sizeof(packet_t));
            break;
        case PIKEAERO_PAP_TYPE_RESET_REQUEST:               /* A request to reset the device */
            //raw.setRawData((char*)&mPacket,sizeof(polling_settings_get_request));
            break;
        case PIKEAERO_PAP_TYPE_RESET_RESPONSE:              /* A confirmation response to reset request */
            //raw.setRawData((char*)&mPacket,sizeof(general_settings_set_request));
            break;
    }

    return raw;
}

void PAPacket::fromByteArray(QByteArray raw)
{
    if ( raw.count() <= (int)sizeof(mPacket) )
    {
        memcpy(&mPacket,raw.data(),raw.count());
    }
}

void PAPacket::setType(PacketType type)
{
    mPacket.type = type;
}

PacketType PAPacket::type()
{
    return static_cast<PacketType>(mPacket.type);
}

void PAPacket::setSignature(QString signature)
{
    memset(mPacket.signature,0,PIKEAERO_PAP_SIGNATURE_SZ);
    for(int n=0; n < signature.length() && n < PIKEAERO_PAP_SIGNATURE_SZ; n++)
    {
        mPacket.signature[n] = signature.toLatin1().data()[n];
    }
}

QString PAPacket::signature()
{
    QString rc;
    for(int n=0; n < PIKEAERO_PAP_SIGNATURE_SZ && mPacket.signature[n]; n++)
    {
        rc += (char)mPacket.signature[n];
    }
    return rc;
}

bool PAPacket::setHostname(QString hostname)
{
    memset(mPacket.hostname,0,PIKEAERO_PAP_HOSTNAME_SZ);
    for(int n=0; n < hostname.length() && n < PIKEAERO_PAP_HOSTNAME_SZ; n++)
    {
        mPacket.hostname[n] = hostname.toLatin1().data()[n];
    }
    return true;
}

QString PAPacket::hostname()
{
    QString rc;
    for(int n=0; n < PIKEAERO_PAP_HOSTNAME_SZ && mPacket.hostname[n]; n++)
    {
        rc += (char)mPacket.hostname[n];
    }
    return rc;
}

/**
 * @brief PAPacket::setMacAddressFromString
 * @param macAddress The MAC address in ASCII as "xx:xx:xx:xx:xx:xx"s
 * @return true on success, else false
 */
bool PAPacket::setMacAddressFromString(QString macAddress)
{
    bool rc = false;
    QStringList hexList = macAddress.split(':');
    if ( hexList.count() == PIKEAERO_PAP_MAC_SZ )
    {
        QByteArray macBytes;
        rc=true;
        for(int n=0; rc && n < hexList.count(); n++)
        {
            macBytes.append((char) hexList.at(n).toInt(&rc,16));
        }
        if ( rc )
        {
            rc = setMacAddress(macBytes);
        }
    }
    return rc;
}

/**
 * @brief PAPacket::setMacAddress
 * @param macAddress A byte array of 6 gytes containing the MAC address to set.
 * @return true on success, else false
 */
bool PAPacket::setMacAddress(QByteArray macAddress)
{
    bool rc=false;
    memset(mPacket.mac,0xFF,PIKEAERO_PAP_MAC_SZ);
    if ( macAddress.count() == PIKEAERO_PAP_MAC_SZ )
    {
        memcpy(mPacket.mac,macAddress.data(),PIKEAERO_PAP_MAC_SZ);
        rc=true;
    }
    return rc;
}

QByteArray PAPacket::macAddress()
{
    QByteArray rc;
    for(int n=0; n < PIKEAERO_PAP_MAC_SZ; n++)
    {
        rc += mPacket.mac[n];
    }
    return rc;
}

/**
 * @brief PAPacket::setAltMacAddressFromString
 * @param macAddress The Alt MAC address in ASCII as "xx:xx:xx:xx:xx:xx"s
 * @return true on success, else false
 */
bool PAPacket::setAltMacAddressFromString(QString macAddress)
{
    bool rc = false;
    QStringList hexList = macAddress.split(':');
    if ( hexList.count() == PIKEAERO_PAP_MAC_SZ )
    {
        QByteArray macBytes;
        rc=true;
        for(int n=0; rc && n < hexList.count(); n++)
        {
            macBytes.append((char) hexList.at(n).toInt(&rc,16));
        }
        if ( rc )
        {
            rc = setAltMacAddress(macBytes);
        }
    }
    return rc;
}

/**
 * @brief PAPacket::setAltMacAddress
 * @param macAddress A byte array of 6 gytes containing the MAC address to set.
 * @return true on success, else false
 */
bool PAPacket::setAltMacAddress(QByteArray macAddress)
{
    bool rc=false;
    memset(mPacket.altmac,0xFF,PIKEAERO_PAP_MAC_SZ);
    if ( macAddress.count() == PIKEAERO_PAP_MAC_SZ )
    {
        memcpy(mPacket.altmac,macAddress.data(),PIKEAERO_PAP_MAC_SZ);
        rc=true;
    }
    return rc;
}

QByteArray PAPacket::altMacAddress()
{
    QByteArray rc;
    for(int n=0; n < PIKEAERO_PAP_MAC_SZ; n++)
    {
        rc += mPacket.altmac[n];
    }
    return rc;
}

bool PAPacket::setIpAddress(QHostAddress ipAddress)
{
    bool rc= false;
    if ( !ipAddress.isNull() )
    {
        quint32 ip = ipAddress.toIPv4Address();
        mPacket.ip[0] = (ip>>24)&0xFF;
        mPacket.ip[1] = (ip>>16)&0xFF;
        mPacket.ip[2] = (ip>>8)&0xFF;
        mPacket.ip[3] = (ip&0xFF);
        rc = true;
    }
    return rc;
}

QHostAddress PAPacket::ipAddress()
{
    QHostAddress rc;
    quint32 ip = 0;
    ip |= (quint32)mPacket.ip[0] << 24;
    ip |= (quint32)mPacket.ip[1] << 16;
    ip |= (quint32)mPacket.ip[2] << 8;
    ip |= (quint32)mPacket.ip[3];
    rc.setAddress(ip);
    return rc;
}

bool PAPacket::setNetMask(QHostAddress netMask)
{
    bool rc= false;
    if ( !netMask.isNull() )
    {
        quint32 ip = netMask.toIPv4Address();
        mPacket.netmask[0] = (ip>>24)&0xFF;
        mPacket.netmask[1] = (ip>>16)&0xFF;
        mPacket.netmask[2] = (ip>>8)&0xFF;
        mPacket.netmask[3] = (ip&0xFF);
        rc=true;
    }
    return rc;
}

QHostAddress PAPacket::netMask()
{
    QHostAddress rc;
    quint32 ip = 0;
    ip |= (quint32)mPacket.netmask[0] << 24;
    ip |= (quint32)mPacket.netmask[1] << 16;
    ip |= (quint32)mPacket.netmask[2] << 8;
    ip |= (quint32)mPacket.netmask[3];
    rc.setAddress(ip);
    return rc;
}

bool PAPacket::setGateway(QHostAddress gateway)
{
    bool rc= false;
    if ( !gateway.isNull() )
    {
        quint32 ip = gateway.toIPv4Address();
        mPacket.gateway[0] = (ip>>24)&0xFF;
        mPacket.gateway[1] = (ip>>16)&0xFF;
        mPacket.gateway[2] = (ip>>8)&0xFF;
        mPacket.gateway[3] = (ip&0xFF);
        rc=true;
    }
    return rc;
}

QHostAddress PAPacket::gateway()
{
    QHostAddress rc;
    quint32 ip = 0;
    ip |= (quint32)mPacket.gateway[0] << 24;
    ip |= (quint32)mPacket.gateway[1] << 16;
    ip |= (quint32)mPacket.gateway[2] << 8;
    ip |= (quint32)mPacket.gateway[3];
    rc.setAddress(ip);
    return rc;
}

bool PAPacket::setDHCP(bool dhcp)
{
    mPacket.dhcp = (dhcp ? 1 : 0);
    return true;
}

bool PAPacket::dhcp()
{
    bool rc=false;
    if ( mPacket.dhcp )
        rc = true;
    return rc;
}

bool PAPacket::setSetMac(bool setmac)
{
    mPacket.setmac = (setmac ? 1 : 0);
    return true;
}

bool PAPacket::setmac()
{
    bool rc=false;
    if ( mPacket.setmac )
        rc = true;
    return rc;
}


