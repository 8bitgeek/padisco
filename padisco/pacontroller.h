/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#ifndef PACONTROLLER_H
#define PACONTROLLER_H

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTimerEvent>
#include "pafinder.h"

class PAController : public QCoreApplication
{
    Q_OBJECT
    public:

        explicit PAController(int argc, char* argv[]);
        virtual ~PAController();

        QString				trademark()								{return "PADisco";}
        QString				version()								{return "1.2.0";}
        QString				copyright()								{return "Copyright (c) 2019 Pike Aerospace Research Corporation";}
        QString				contact()								{return "Sales & Support <sales@pikeaero.com> | http://www.pikeaero.com";}


    signals:

        void                sigDiscover();
        void                sigQuit();

    public slots:

        void				printHelp();
        void				printVersion();
        void				save();
        void				restore();

    protected:

        void                timerEvent(QTimerEvent *event);

    private slots:

        bool				parseCommandLine(int argc,char** argv);
        void                timerTimeout();

        void                sending();
        void                receiving();
        void                timeout();
        void                deviceFound(PADevice device);

        bool                findDevices(int timeout=PIKEAERO_MULTICAST_REPLY_TIMEOUT);
        bool                applyDevice(int timeout=PIKEAERO_MULTICAST_REPLY_TIMEOUT);
        bool                resetDevice(int timeout=PIKEAERO_MULTICAST_REPLY_TIMEOUT);

    private:

        void                instantiateFinder();

        QTimer              mTimer;
        int                 mArgc;
        char**              mArgv;
        PAFinder*           mFinder;
        int                 mWaitTimer;
        bool                mWaitTimeout;
        PAPacket            mPacket;

};

#endif // PACONTROLLER_H
