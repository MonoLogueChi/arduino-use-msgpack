#include <Arduino.h>
#include <MsgPacketizer.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

enum CustomType
{
  Type0 = 0,
  Type1 = 1,
  Type2 = 2
};

enum CustomAddress
{
  Address00 = 0x00,
  Address01 = 0x01,
  Address10 = 0x10
};

struct CustomData
{
  uint8_t Address;
  uint32_t Data;
  MSGPACK_DEFINE(Address, Data);
};

struct CustomPackData
{
  uint8_t Type;
  CustomData Data;
  MSGPACK_DEFINE(Type, Data);
};

CustomData d;
CustomPackData pd;

byte send_index = 0x00;
byte recv_index = 0x00;

void Reu8g2(CustomPackData p)
{
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print("指令:");
  u8g2.setCursor(0, 35);
  u8g2.print("地址:");
  u8g2.setCursor(0, 55);
  u8g2.print("数据:");

  u8g2.setCursor(64, 15);
  u8g2.print(p.Type);
  u8g2.setCursor(64, 35);
  u8g2.print(p.Data.Address);
  u8g2.setCursor(64, 55);
  u8g2.print(p.Data.Data);

  u8g2.sendBuffer();
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_unifont_t_chinese3);
  u8g2.setFontDirection(0);

  MsgPacketizer::subscribe(Serial, recv_index,
                           [&](const CustomPackData &p)
                           {
                             switch (p.Type)
                             {
                             case Type2:
                               Reu8g2(p);
                               break;
                             default:
                               break;
                             }
                           });

  d.Address = Address00;
  d.Data = 1024;
  pd.Type = Type1;
  pd.Data = d;

  MsgPacketizer::publish(Serial, send_index, pd)
      ->setFrameRate(1.f);
}

void loop()
{
  MsgPacketizer::update();
}