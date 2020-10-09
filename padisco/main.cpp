/** ***************************************************************************
 * Copyright (C) 2017 Pike Aerospace Research Corporation
 * All Rights Reserved
 * @author Mike Sharkey <mike@pikeaero.com>
 *****************************************************************************/
#include "pacontroller.h"

int main(int argc, char *argv[])
{
    PAController a(argc, argv);
    return a.exec();
}
