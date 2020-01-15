#include "lwm2mObject.h"

using namespace std;

bool lwm2mObject::write(lwm2mData & data);
bool lwm2mObject::read(lwm2mData * dataP);
bool lwm2mObject::execute(lwm2mData & data);
bool lwm2mObject::create();
bool lwm2mObject::remove();
bool lwm2mObject::discover(lwm2mData * dataP);