/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp bodynetservice.xml -p bodynetservice-interface
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "bodynetservice-interface.h"

/*
 * Implementation of interface class LocalBodyNetServiceInterface
 */

LocalBodyNetServiceInterface::LocalBodyNetServiceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

LocalBodyNetServiceInterface::~LocalBodyNetServiceInterface()
{
}

