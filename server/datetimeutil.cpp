#include "datetimeutil.h"

DateTimeUtil::DateTimeUtil()
{

}

QString DateTimeUtil::getCurTimeStr()
{
    QTime tm = QTime::currentTime();
    return QString("<font color=red>[") + tm.toString("hh:mm:ss") + QString("</font>]");
}
