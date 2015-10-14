#include "coreservice.h"

CoreService * CoreService::instance = new CoreService();

CoreService *CoreService::getInstance()
{
    return instance;
}

CoreService::CoreService()
{

}
