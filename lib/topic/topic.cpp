#include <topic.h>
topic::topic(char* mqttNode,char* mqttTopic){

  _ftopic = mqttTopic;
  _mqttNode = mqttNode;
};
