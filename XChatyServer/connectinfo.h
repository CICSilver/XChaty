#pragma once

#include <QObject>
#include "hv.h"
class ConnectInfo  : public QObject
{
	Q_OBJECT

public:
	ConnectInfo(QObject *parent);
	~ConnectInfo();

private:

};
