// Demo: NMEA2000 library. Send main cabin temperature to the bus.

#include <Arduino.h>
//#define N2k_CAN_INT_PIN 21
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>

void setup() {
  // Set Product information
  NMEA2000.SetProductInformation("00000001", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "Simple battery monitor",  // Manufacturer's Model ID
                                 "1.0.0.12 (2016-02-02)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2015-08-03)" // Manufacturer's Model version
                                 );
  // Det device information
  NMEA2000.SetDeviceInformation(1, // Unique number. Use e.g. Serial number.
                                170, // Device function=Battery. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                35, // Device class=Electrical Generation. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf                               
                               );
  // Uncomment 3 rows below to see, what device will send to bus                           
   Serial.begin(115200);
  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.
  NMEA2000.SetForwardOwnMessages();

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,22);
  //NMEA2000.SetDebugMode(tNMEA2000::dm_ClearText); // Uncomment this, so you can test code without CAN bus chips on Arduino Mega
  //NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();
}


void loop() {
  SendN2kBattery();
  NMEA2000.ParseMessages();
}

#define BatUpdatePeriod 1000

void SendN2kBattery() {
  static unsigned long TempUpdated=millis();
  tN2kMsg N2kMsg;

  if ( TempUpdated+BatUpdatePeriod<millis() ) {
    TempUpdated=millis();
    SetN2kDCBatStatus(N2kMsg,1,13.87,5.12,35.12,1);
    NMEA2000.SendMsg(N2kMsg);
    SetN2kDCStatus(N2kMsg,1,1,N2kDCt_Battery,56,92,38500,0.012);
    NMEA2000.SendMsg(N2kMsg);
    SetN2kBatConf(N2kMsg,1,N2kDCbt_Gel,N2kDCES_Yes,N2kDCbnv_12v,N2kDCbc_LeadAcid,AhToCoulomb(420),53,0.501,75);
    NMEA2000.SendMsg(N2kMsg);
    // Serial.print(millis()); Serial.println(", Temperature send ready");
  }
}
