#include "network.h"
#include "remote.h"
#include "serial.h"
#include "server.h"
#include "settings.h"

#include <QSysInfo>

Server* Server::instance = NULL;

Server* Server::Instance()
{
    if (!instance)
    {
        instance = new Server();
    }
    return instance;
}

Server::Server()
{
    Logger::Instance()->add("Initializing server");
    status = "Initializing";

    if (isLatestServerRunning())
    {
        Settings::Instance()->loadSettings();
        Network::Instance();        // In order to initialize the network
        apps = new QList<App>();
        Logger::Instance()->trackLaunchEvent();
        Logger::Instance()->add("Server ready");
        status = "Ready";
    }
}

void Server::finish()
{
    Settings::Instance()->saveSettings();
    Serial::Instance()->closeSerialPort();
}

bool Server::isLatestServerRunning()
{
    // ToDo: find out whether latest server is running
    return true;
}

App Server::getApp(QString ip)
{
    foreach (App savedApp, *apps)
    {
        if (savedApp.ip == ip)
        {
            return savedApp;
        }
        App *app = new App();
        app->ip = ip;
        apps->append(*app);
        return *app;
    }
}

App Server::getCurrentApp()
{
    return getApp(Remote::Instance()->lastCommand->source);
}

QString Server::getServerVersionName()
{
    return APP_VERSION;
}

QString Server::getServerOs()
{
    return QSysInfo::productVersion();
}

QString Server::getServerName(){
    QString name = "Unknown";
#ifdef __APPLE__
    name = getenv("USER");
#endif
    return name;
}
