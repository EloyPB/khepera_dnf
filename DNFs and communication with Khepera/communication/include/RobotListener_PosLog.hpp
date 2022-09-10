

#ifndef INCLUDE_ROBOTLISTENER_POSLOG_HPP_
#define INCLUDE_ROBOTLISTENER_POSLOG_HPP_

#include "RobotListener.hpp"

class PosLog : public RobotListener {
public:
    PosLog(RobotComm* pRobotComm);
	~PosLog();

public slots:
    void listenAndAct();

private:
	FILE *mFile;


};


#endif /* INCLUDE_ROBOTLISTENER_POSLOG_HPP_ */
