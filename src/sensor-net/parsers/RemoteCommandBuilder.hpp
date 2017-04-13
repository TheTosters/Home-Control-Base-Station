/*
 * RemoteCommandBuilder.hpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Zarnowski
 */

#ifndef RemoteCommandBuilder_hpp
#define RemoteCommandBuilder_hpp

#include <string>

using namespace std;

extern const string REMOTE_CMD_READ_TEMP_HISTORY;
extern const string REMOTE_CMD_GET_SOFTWARE_VERSION;
extern const string REMOTE_CMD_CONFIGURE_TEMPERATURE_RESOLUTION;
extern const string REMOTE_CMD_CONFIGURE_TEMPERATURE_PERIOD;
extern const string REMOTE_CMD_CONFIGURE_POWER_PERIOD;
extern const string REMOTE_CMD_CONFIGURE_SYSTEM_TIME;
extern const string REMOTE_CMD_GET_SYSTEM_CAPABILITIES;
extern const string REMOTE_CMD_CONFIGURE_NODE_NAME;
extern const string REMOTE_CMD_CONFIGURE_RESET_SYSTEM;
extern const string REMOTE_CMD_CONFIGURE_SAVING_MODE;
extern const string REMOTE_CMD_CONFIGURE_SAVING_ACTIVITY;

class RemoteCommandBuilder {
    public:
        RemoteCommandBuilder(const string& cmd, bool includePreamble = true);

        void addArgument(int64_t value);
        void addArgument(int value);
        void addArgument(double value);
        void addArgument(const string& value);
        void startSequence();
        void endSequence();

        string buildCommand();
    private:
        string outCmd;
        enum ElementType {UNKNOWN, DIGIT, STRING} elementsType;
        bool isSequenceOpen;
        bool needComa;
        bool expectedNextSubsequence;
        bool includePreamble;
};

#endif /* RemoteCommandBuilder_hpp */
