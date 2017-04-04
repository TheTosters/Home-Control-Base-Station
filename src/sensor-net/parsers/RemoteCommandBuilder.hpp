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

extern const string REMOTE_CMD_GET_SYSTEM_CAPABILITIES;
extern const string REMOTE_CMD_READ_TEMP_HISTORY;
extern const string REMOTE_CMD_GET_SOFTWARE_VERSION;

class RemoteCommandBuilder {
    public:
        RemoteCommandBuilder(const string& cmd);

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
};

#endif /* RemoteCommandBuilder_hpp */
