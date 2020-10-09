/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#ifndef PAPACKET_H
#define PAPACKET_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include <stdint.h>
#include <inttypes.h>

#define PacketType                                      uint8_t
#define PIKEAERO_MULTICAST_GROUP_STRING                 "239.255.255.128"
#define PIKEAERO_MULTICAST_GROUP_OCTETS                 {239,255,255,128}
#define PIKEAERO_MULTICAST_PORT                         (5454)
#define PIKEAERO_MULTICAST_REPLY_TIMEOUT                (5000)      /* milliseconds */

#define PIKEAERO_PAP_MAC_SZ                             6           /* MAC address size */
#define	PIKEAERO_PAP_SIGNATURE							"PIKEAER0"	/* 'PIKEAERO' */
#define	PIKEAERO_PAP_SIGNATURE_SZ						8			/* 'PIKEAERO' */
#define PIKEAERO_PAP_HOSTNAME_SZ                        32
#define PIKEAERO_PAP_DHCP_MASK							0x01		/* DHCP bit mask */

#define PIKEAERO_PAP_TYPE_REQUEST_MASK                  0x0F
#define PIKEAERO_PAP_TYPE_RESPONSE_MASK                 0xF0

#define PIKEAERO_PAP_TYPE_DISCOVER_REQUEST				0x01		/* A discovery request was received */
#define PIKEAERO_PAP_TYPE_DISCOVER_RESPONSE				0x10		/* Discovery response sent out */

#define	PIKEAERO_PAP_TYPE_IP_SETTINGS_REQUEST			0x02		/* A request to change IP settings */
#define	PIKEAERO_PAP_TYPE_IP_SETTINGS_RESPONSE			0x20		/* A request to change IP settings */

#define	PIKEAERO_PAP_TYPE_RESET_REQUEST					0x03		/* A request to reset the device */
#define	PIKEAERO_PAP_TYPE_RESET_RESPONSE				0x30		/* A confirmation response to reset request */


class PAPacket : public QObject
{
    Q_OBJECT
    private:

        typedef struct
        {
            char		signature[PIKEAERO_PAP_SIGNATURE_SZ];	// 'PIKEAER0'
            char		hostname[PIKEAERO_PAP_HOSTNAME_SZ];		// Hostname
            uint8_t		type;									// packet type
            uint8_t		mac[6];									// MAC address
            uint8_t		altmac[6];								// Alternate MAC address
            uint8_t		ip[4];									// Ip Address (IPV4)
            uint8_t		netmask[4];								// Netmask (IPV4)
            uint8_t		gateway[4];								// Gateway (IPV4)
            uint8_t		dhcp:1;									// DHCP enabled/disable
            uint8_t		setmac:1;								// Apply altmac to mac
        } __attribute__((packed)) packet_t;

    public:

        PAPacket(QObject *parent = 0);
        PAPacket(const PAPacket& other);
        PAPacket(QByteArray raw);
        virtual ~PAPacket();

        PAPacket&           operator=( const PAPacket& other );

        void                fromByteArray(QByteArray raw);
        QByteArray          toByteArray();

        void                setType(PacketType type);
        PacketType          type();

        void                setSignature(QString signature=PIKEAERO_PAP_SIGNATURE);
        QString             signature();

        bool                setHostname(QString hostname);
        QString             hostname();

        bool                setMacAddressFromString(QString macAddress);
        bool                setMacAddress(QByteArray macAddress);
        QByteArray          macAddress();

        bool                setAltMacAddressFromString(QString macAddress);
        bool                setAltMacAddress(QByteArray macAddress);
        QByteArray          altMacAddress();

        bool                setIpAddress(QHostAddress ipAddress);
        QHostAddress        ipAddress();

        bool                setNetMask(QHostAddress netMask);
        QHostAddress        netMask();

        bool                setGateway(QHostAddress gateway);
        QHostAddress        gateway();

        bool                setDHCP(bool dhcp);
        bool                dhcp();

        bool                setSetMac(bool setmac);
        bool                setmac();

    private:
        packet_t            mPacket;
};

#endif // PAPACKET_H
