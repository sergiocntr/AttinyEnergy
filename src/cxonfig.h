#ifndef cxconfig_h
#define cxconfig_h
//ESP-01 SDA - SCL pin
static int default_sda_pin = 0;
static int default_scl_pin = 2;
const int SLAVE_ADDRESS = 0X02; //attiny
struct energyData{
	float realPower;
	float apparentPower;
	float powerFactor;
  float supplyVoltage;
  float Irms;
};
typedef struct energyData EnergyData;
EnergyData ener;
#endif
