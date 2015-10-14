#ifndef CORESERVICE_H
#define CORESERVICE_H

#include "hashrenamer.h"
#include <QStringList>

class CoreService
{
public:
    CoreService *getInstance();
    void runWithFilenamesStingList(QStringList &filenamesStringList);

private:
    CoreService();
    CoreService(CoreService &other);

    static CoreService *instance;
};

#endif // CORESERVICE_H
