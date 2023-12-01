#include <SPI.h>

#define MISO_PIN   12
#define MOSI_PIN   13
#define SCK_PIN    14
#define SS_PIN     15

#define MSG_IN_RXB0 0x01
#define N0_MSGS 0X00

#define INIT_TRANSACTION 0x01
#define CAN_MSG_STATUS 0x04
#define MSG_PRESENT 0x33
#define MSG_ABSENT 0x32
#define READ_MSG 0x03
#define ABORT_TRANSACTION 0x02

void send_read_command(void);uint8_t flag = 0;
uint8_t test_snd_buff = {0x03};
uint8_t test_rcv_buff;
int j = 0;
uint32_t canId = 0;
static int can_msg_count = 0;

void setup() {
  // Initialize SPI communication
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  // Set the desired data format (16 bits) using the SPI data mode
  SPISettings settings(400000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(settings);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  Serial.begin(115200);
}
void loop()
{
  check_and_receive_can();  
//  delayMicroseconds(500);
  delay(1);
}

void check_and_receive_can()
{
  digitalWrite(SS_PIN, LOW);
  test_snd_buff = CAN_MSG_STATUS;
  test_rcv_buff = SPI.transfer(test_snd_buff);        //It will send 0x03(Query)and it will receive 0x33(resp)
//  Serial.println(test_rcv_buff, HEX); 
  if(test_rcv_buff == MSG_PRESENT)
  {
    can_msg_count++;
    test_snd_buff = READ_MSG;
    test_rcv_buff = SPI.transfer(test_snd_buff);
    send_read_command();
  }
  else
  {
    test_snd_buff = ABORT_TRANSACTION;
//    test_rcv_buff = SPI.transfer(test_snd_buff);
  }
  digitalWrite(SS_PIN, HIGH);
}

void send_read_command(void)
{
  int sum = 0;
  uint8_t dataToSend[14] = {0x00};// sending read command along with 10 dummy bytes
  uint8_t receivedData[14];
  for (int i = 0; i < 14; i++)
  {
    receivedData[i] = SPI.transfer(dataToSend[i]);
  }
//  Serial.println("Sequence number is:-");
//  Serial.println("Received CAN ID is:-");
//  canId |= ((uint32_t)receivedData[1] << 24);
//  canId |= ((uint32_t)receivedData[2] << 16);
//  canId |= ((uint32_t)receivedData[3] << 8);
//  canId |= receivedData[4];
  
//  Serial.println(canId, HEX);
  Serial.println(can_msg_count);
//  Serial.println("Length of message is:-");
//  Serial.println(receivedData[13], HEX);
  
  canId = 0;
  
//  Serial.println("Received CAN data bytes are:-");
  for(j = 5;j < 13;j++)
  {
    sum += receivedData[j];
//    Serial.print(receivedData[j], HEX);
//    if(j != 12)
//      erial.print(",");
  }
  if(sum != 36)    
    Serial.println("Fault");
}
