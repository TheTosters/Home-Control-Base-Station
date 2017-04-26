/*
 * RemoteCommandBuilder.hpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Zarnowski
 */

#ifndef RemoteCommandBuilder_hpp
#define RemoteCommandBuilder_hpp

#include <string>
#include <sensor-net/parsers/Number.hpp>

using namespace std;

//config
extern const string REMOTE_CMD_CONFIGURE_TEMPERATURE_RESOLUTION;
extern const string REMOTE_CMD_CONFIGURE_TEMPERATURE_PERIOD;
extern const string REMOTE_CMD_CONFIGURE_POWER_PERIOD;
extern const string REMOTE_CMD_CONFIGURE_SYSTEM_TIME;
extern const string REMOTE_CMD_CONFIGURE_DEFAULT_GPIO_OUTPUT;
extern const string REMOTE_CMD_GET_SYSTEM_CAPABILITIES;
extern const string REMOTE_CMD_CONFIGURE_NODE_NAME;
extern const string REMOTE_CMD_CONFIGURE_SAVING_MODE;
extern const string REMOTE_CMD_CONFIGURE_SAVING_ACTIVITY;

//aux
extern const string REMOTE_CMD_REQUEST_SYSTEM_RESET;
extern const string REMOTE_CMD_CHANGE_OUTPUT_GPIO;
extern const string REMOTE_CMD_GET_SOFTWARE_VERSION;

//read
extern const string REMOTE_CMD_READ_TEMP_HISTORY;

//values
extern const string REMOTE_CMD_VALUE_TEMP_HISTORY;

class RemoteCommandBuilder {
    public:
        RemoteCommandBuilder(const string& cmd, bool includePreamble = true);

        void addArgument(int64_t value);
        void addArgument(Number value);
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
