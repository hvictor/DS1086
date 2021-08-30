/* **************************************************************************************************************
 *  DS1086 Device address = [1011 = Device Identifier][000 = Default Slave Address][R/W bit handled from Wire.h]
 *  Therefore the address will be: 1011 000 (without specifying the R/W bit explicitly) = 0101 1000 = 0x58
 *  *************************************************************************************************************/

#include <Wire.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(); 
  Serial.begin(9600);
}

byte read_OFFSET_default()
{
  // RANGE register is at 0x37
 
  Wire.beginTransmission(0x58);
  Wire.write(0x37);               // Point to RANGE register
  Wire.endTransmission(false);    // Do not release the line
  Wire.requestFrom(0x58, 1);      // Request to read 1 byte. The address has the last bit set (READ bit).
  byte retval = Wire.read();
  return retval;
}

byte read_OFFSET()
{
  // OFFSET register is at 0x0E
 
  Wire.beginTransmission(0x58);
  Wire.write(0x0E);               // Point to OFFSET register
  Wire.endTransmission(false);    // Do not release the line
  Wire.requestFrom(0x58, 1);      // Request to read 1 byte. The address has the last bit set (READ bit).
  byte retval = Wire.read();
  return retval;
}

byte read_PRESCALER()
{
  // PRESCALER register is at 0x02
 
  Wire.beginTransmission(0x58);
  Wire.write(0x02);               // Point to PRESCALER register
  Wire.endTransmission(false);    // Do not release the line
  Wire.requestFrom(0x58, 1);      // Request to read 1 byte. The address has the last bit set (READ bit).
  byte retval = Wire.read();
  return retval;
}

void loop()
{

  // Read OFFSET value from register RANGE[5 downto 0]
  // The default OFFSET is described as "OS" in the device's datasheet
  byte default_offset = read_OFFSET_default() & 0x1F;
  byte offset = read_OFFSET() & 0x1F;
  byte prescaler = read_PRESCALER() & 0x0F;
  Serial.println("**********************************");
  Serial.print("DEFAULT OFFSET = ");
  Serial.println(default_offset, HEX);
  Serial.print("CURRENT OFFSET = ");
  Serial.println(offset, HEX);  
  Serial.print("CURRENT PRESCALER = ");
  Serial.println(prescaler, HEX);
  Serial.println("**********************************");

  Serial.println();
  Serial.println("Programming EEPROM....");
  delay(1000);
    
  // Store 0x03 in the PRESCALER register
  Wire.beginTransmission(0x58);   
  Wire.write(0x02); // PRESCALER address = 0x02        
  Wire.write(0x03);
  Wire.endTransmission();

  delay(100);

  // Store OS+5 in the lowest 5 bits of the OFFSET register
  offset = default_offset + 5;
  Wire.beginTransmission(0x58);   
  Wire.write(0x0E); // OFFSET address = 0x0E
  Wire.write(offset);
  Wire.endTransmission();

  delay(100);

  // Store 0xE000 in the DAC register (DAC HIGH <- MSB, DAC LOW <- LSB)
  Wire.beginTransmission(0x58);   
  Wire.write(0x08); // DAC address = 0x08        
  Wire.write(0xE0);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(100);

  // Write EEPROM
  Wire.beginTransmission(0x58);
  Wire.write(0x3F);
  Wire.endTransmission();

  Serial.println("EEPROM programmed.");

  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    delay(1000);
  }
}
