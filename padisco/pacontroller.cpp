/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#include "pacontroller.h"
#include <iostream>

#define inherited QCoreApplication

PAController::PAController(int argc, char* argv[])
: inherited(argc,argv)
, mArgc(argc)
, mArgv(argv)
, mFinder(NULL)
, mWaitTimer(-1)
{
    QObject::connect(this,SIGNAL(sigQuit()),this,SLOT(quit()));
    QObject::connect(&mTimer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    mTimer.setInterval(0);
    mTimer.start();
}

PAController::~PAController()
{
    mTimer.stop();
    if ( mFinder != NULL )
    {
        delete mFinder;
    }
}

void PAController::save()
{
}

void PAController::restore()
{
}

/** ***************************************************************************
* @brief ENTRY POINT ON TIMER
******************************************************************************/
void PAController::timerTimeout()
{
    parseCommandLine(mArgc,mArgv);
    emit sigQuit();
}

void PAController::sending()
{
    //std::cout << "sending" << std::endl;
}

void PAController::receiving()
{
    //std::cout << "receiving" << std::endl;
}

void PAController::timeout()
{
    mWaitTimeout=true;
}

void PAController::deviceFound(PADevice device)
{
    QString rc;
    rc += "--mac ";     rc += device.macAddressString();           rc += " ";
    rc += "--host ";    rc += device.hostname();                   rc += " ";
    rc += "--dhcp ";    rc += device.dhcp()?"1":"0";               rc += " ";
    rc += "--ip ";      rc += device.ipAddress().toString();       rc += " ";
    rc += "--sub ";     rc += device.subnetMask().toString();      rc += " ";
    rc += "--gw ";      rc += device.gatewayAddress().toString();  rc += " ";
    std::cout << rc.toLatin1().data() << std::endl;
}

bool PAController::applyDevice(int timeout)
{
    bool rc = false;
    QEventLoop loop;
    instantiateFinder();
    if ( mFinder )
    {
        mFinder->sendApplyRequest(mPacket);
        mWaitTimeout = false;
        mWaitTimer = startTimer(timeout);
        while (!mWaitTimeout)
        {
            loop.processEvents();
        }
        rc=true;
    }
    return rc;
}

bool PAController::resetDevice(int timeout)
{
    bool rc = false;
    QEventLoop loop;
    instantiateFinder();
    if ( mFinder )
    {
        mFinder->sendResetRequest(mPacket);
        mWaitTimeout = false;
        mWaitTimer = startTimer(timeout);
        while (!mWaitTimeout)
        {
            loop.processEvents();
        }
        rc=true;
    }
    return rc;
}

/**
 * @brief PAController::findDevices Locate devices on the network.
 * @return true on success, else false
 */
bool PAController::findDevices(int timeout)
{
    bool rc = false;
    QEventLoop loop;
    instantiateFinder();
    if ( mFinder )
    {
        mFinder->sendFinderRequest();
        mWaitTimeout = false;
        mWaitTimer = startTimer(timeout);
        while (!mWaitTimeout)
        {
            loop.processEvents();
        }
        rc=true;
    }
    return rc;
}

void PAController::instantiateFinder()
{
    if ( mFinder == NULL )
    {
        QString ipAddress(PIKEAERO_MULTICAST_GROUP_STRING);
        QHostAddress groupHostAddress(ipAddress);
        mFinder = new PAFinder(groupHostAddress,PIKEAERO_MULTICAST_PORT);
        QObject::connect(mFinder,SIGNAL(timeout()),this,SLOT(timeout()));
        QObject::connect(mFinder,SIGNAL(sending()),this,SLOT(sending()));
        QObject::connect(mFinder,SIGNAL(receiving()),this,SLOT(receiving()));
        QObject::connect(mFinder,SIGNAL(deviceFound(PADevice)),this,SLOT(deviceFound(PADevice)));
    }
}

void PAController::timerEvent(QTimerEvent* e)
{
    if ( e->timerId() == mWaitTimer )
    {
        mWaitTimeout = true;
    }
}

/** ***************************************************************************
* @brief Print the version.
******************************************************************************/
void PAController::printVersion()
{
    printf( "%s Version %s\n", trademark().toLocal8Bit().data(), version().toLocal8Bit().data() );
    printf( "%s\n", copyright().toLocal8Bit().data() );
    printf( "%s\n", contact().toLocal8Bit().data() );
}

/** ***************************************************************************
* @brief Print the help.
******************************************************************************/
void PAController::printHelp()
{
    printVersion();
    fprintf( stderr,"--version     -v \n" );
    fprintf( stderr,"--help        -h -?\n");
    fprintf( stderr,"--timeout     -t <milliseconds>\n");
    fprintf( stderr,"--find        -f \n" );
    fprintf( stderr,"--host        -H <hostname>\n");
    fprintf( stderr,"--mac         -m xx:xx:xx:xx:xx:xx\n");
    fprintf( stderr,"--altmac      -m yy:yy:yy:yy:yy:yy\n");
    fprintf( stderr,"--dhcp        -d <1|0>\n");
    fprintf( stderr,"--ip          -i xxx.xxx.xxx.xxx\n");
    fprintf( stderr,"--sub         -s xxx.xxx.xxx.xxx\n");
    fprintf( stderr,"--gw          -g xxx.xxx.xxx.xxx\n");
    fprintf( stderr,"--setmac      -S \n" );
    fprintf( stderr,"--apply       -a \n" );
    fprintf( stderr,"--reset       -r \n");
    fprintf( stderr,"\n");
    fprintf( stderr,"Examples,\n");
    fprintf( stderr,"padisco --find\n");
    fprintf( stderr,"padisco --mac 00:20:6D:69:6E:3D --dhcp 0 -ip 192.168.1.10 --apply\n");
    fprintf( stderr,"padisco --mac 00:20:6D:69:6E:3D --host GATEWAY1 --apply\n");
    fprintf( stderr,"padisco --mac 00:20:6D:69:6E:3D --reset\n");
    fprintf( stderr,"padisco --mac 00:20:6D:69:6E:3D --altmac FE:22:34:56:78:90 --setmac 1 --apply\n");
    fprintf( stderr,"padisco --mac FF:FF:FF:FF:FF:FF --dhcp 1 --sub 255.255.255.0 --apply\n");
}

/** ***************************************************************************
* @brief Parse the command line.
******************************************************************************/
bool PAController::parseCommandLine(int argc,char** argv)
{
    bool rc=true;
    int timeout = PIKEAERO_MULTICAST_REPLY_TIMEOUT;
    if ( argc > 1 )
    {
        mPacket.setDHCP( 1 ); // default DHCP on.

        for( int n=1; rc && n<argc && rc==1; n++)
        {
            QString arg(argv[n]);                        // current arg
            QString argn( n+1 < argc ? argv[n+1] : "" ); // next arg
            if (arg == "--version"      || arg == "-v")                 printVersion();
            else if (arg == "--help"    || arg == "-h" || arg == "-?")  printHelp();
            else if (arg == "--find"    || arg == "-f")                 findDevices(timeout);
            else if (arg == "--apply"   || arg == "-a")                 rc=applyDevice(timeout);
            else if (arg == "--reset"   || arg == "-r")                 rc=resetDevice(timeout);
            else if (arg == "--timeout" || arg == "-t")                 {timeout=argn.toInt();                          ++n;}
            else if (arg == "--host"    || arg == "-H")                 {rc=mPacket.setHostname( argn );                ++n;}
            else if (arg == "--mac"     || arg == "-m")                 {rc=mPacket.setMacAddressFromString( argn );    ++n;}
            else if (arg == "--altmac"  || arg == "-M")                 {rc=mPacket.setAltMacAddressFromString( argn ); ++n;}
            else if (arg == "--dhcp"    || arg == "-d")                 {rc=mPacket.setDHCP( argn.toInt() );            ++n;}
            else if (arg == "--setmac"  || arg == "-S")                 {rc=mPacket.setSetMac( argn.toInt() );          ++n;}
            else if (arg == "--ip"      || arg == "-i")                 {rc=mPacket.setIpAddress( QHostAddress(argn) ); ++n;}
            else if (arg == "--sub"     || arg == "-s")                 {rc=mPacket.setNetMask( QHostAddress(argn) );   ++n;}
            else if (arg == "--gw"      || arg == "-g")                 {rc=mPacket.setGateway( QHostAddress(argn) );   ++n;}
            else
            {
                fprintf(stderr,"invalid '%s'",arg.toLatin1().data());
                printHelp();
                rc=false;
            }
        }
    }
    else
    {
        rc = findDevices(timeout);
    }
    return rc;

}
