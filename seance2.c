// LCD module connections
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

// Define components' pins

sbit greenLED  at RC6_bit;   // Green LED for normal state
sbit blueLED   at RC5_bit;   // Blue LED (off in rest state)
sbit redLED    at RC7_bit;   // Red LED (off in rest state)
sbit lamp1     at RC3_bit;   // Lamp 1
sbit lamp2     at RC4_bit;   // Lamp 2
sbit heater    at RB2_bit;   // Heating element
sbit foodMotor at RB1_bit;   // Food distribution motor
sbit buzzer    at RB3_bit;   // Buzzer
sbit niveau    at RB5_bit;   //niveau d'eau
sbit lumin     at RB6_bit;   //lumineusite
sbit buttonConsult  at RA2_bit;
#define SetHigh(pin) (pin = 1)
#define SetLow(pin)  (pin = 0)
// Define thresholds
#define CRITICAL_TEMP 20
#define QUALITY_THRESHOLD 70
void waterLevel(){
 SetHigh(redLED);
 SetHigh(buzzer);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Niveau d'eau bas");
 Delay_ms(6000);
}
void lumiere(){
 SetHigh(lamp1);
 SetHigh(lamp2);
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Mode nuit");
 Delay_ms(5000);
}
void food(){
 int i;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Distribution");
 Lcd_Out(2, 1, "de nourriture");
 for(i=0; i<3; i++){
   SetHigh(redLED);
   Delay_ms(500);
   SetLow(redLED);
   Delay_ms(500);
   }
  SetHigh(foodMotor);
  Delay_ms(2000);
}
int ii=0,read,m,NB=183;
unsigned char mm;
void main(){
  int quality,tempC;
  // Set PORTB  et portD
 // TRISD = 0;
  TRISB = 0b01100001;
  TRISC = 0;
  // Initialize LCD
  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);  // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);  // Turn off cursor
  Lcd_Out(1, 1, "etatNormal");
  Lcd_Out(2, 1, "modeJour");
  // ADC initialization
  ADC_Init();
  INTCON.GIE = 1;
  INTCON.INTE = 1;
  OPTION_REG.INTEDG = 0;
  INTCON.RBIE = 1;
  OPTION_REG.T0CS=0;
  OPTION_REG.PSA=0;
  OPTION_REG.PS0=1;
  OPTION_REG.PS1=1;
  OPTION_REG.PS2=1;
  TMR0=0;
  // Turn off all components initially
   SetLow(buzzer);
   SetLow(foodMotor);
   SetLow(lamp1);
   SetLow(lamp2);
   SetLow(heater);
   SetLow(redLED);
   SetLow(blueLED);
   // Turn on green LED
   SetHigh(greenLED);
  while (1) {
   // Read teerature and quality from ADC
   tempC = ADC_Read(0)*0.496;
   //quality = ADC_Read(1)*0.496;
   quality = ADC_Read(1)*0.496;
   // Check temperature and quality thresholds
   if(m==1){
    waterLevel();
    ii++;
    EEPROM_Write(0x02,ii);
    m=0;
   }
   if(m==2){
    lumiere();
    m=0;
   }
   if(m==3){
    food();
    m=0;
   }
   if (tempC < CRITICAL_TEMP) {
      SetHigh(redLED);
      SetHigh(buzzer);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1, 1, "Temp Critical!");
      ii++;
      EEPROM_Write(0x02,ii);
      Delay_ms(3000);

   }
   else if (quality < QUALITY_THRESHOLD) {
      SetHigh(blueLED);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(2, 1, "quality low");
      ii++;
      EEPROM_Write(0x02,ii);
      Delay_ms(4000);

   }
   else{
   SetLow(buzzer);
   SetLow(foodMotor);
   SetLow(lamp1);
   SetLow(lamp2);
   SetLow(heater);
   SetLow(redLED);
   SetLow(blueLED);
   SetHigh(greenLED);
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_Out(1, 1, "etatNormal");
   Lcd_Out(2, 1, "modeJour");
   }
   if(buttonConsult){
    read=EEPROM_Read(0x02);
    IntToStr(read,mm);
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, mm);
   }
  };
}
void interrupt(){


 if(INTCON.INTF){
  m=3;
  INTCON.INTF=0;
 }
 if(INTCON.T0IF){
  NB--;
  if(NB==0){
  NB=183;
  SetHigh(redLED);
  INTCON.T0IE=0;
  }
  INTCON.T0IF=0;}
 if(INTCON.RBIF){
 if(niveau==1){
  m=1;

  INTCON.T0IE=1;
  }

 if(lumin==0){
  m=2;
 }
 INTCON.RBIF=0;
}}
