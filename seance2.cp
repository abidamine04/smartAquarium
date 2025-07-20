#line 1 "C:/Users/abida/OneDrive - ESPRIT/projet micro/seance2.c"

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



sbit greenLED at RC6_bit;
sbit blueLED at RC5_bit;
sbit redLED at RC7_bit;
sbit lamp1 at RC3_bit;
sbit lamp2 at RC4_bit;
sbit heater at RB2_bit;
sbit foodMotor at RB1_bit;
sbit buzzer at RB3_bit;
sbit niveau at RB5_bit;
sbit lumin at RB6_bit;
sbit buttonConsult at RA2_bit;





void waterLevel(){
  (redLED = 1) ;
  (buzzer = 1) ;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Niveau d'eau bas");
 Delay_ms(6000);
}
void lumiere(){
  (lamp1 = 1) ;
  (lamp2 = 1) ;
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
  (redLED = 1) ;
 Delay_ms(500);
  (redLED = 0) ;
 Delay_ms(500);
 }
  (foodMotor = 1) ;
 Delay_ms(2000);
}
int ii=0,read,m,NB=183;
unsigned char mm;
void main(){
 int quality,tempC;


 TRISB = 0b01100001;
 TRISC = 0;

 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Out(1, 1, "etatNormal");
 Lcd_Out(2, 1, "modeJour");

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

  (buzzer = 0) ;
  (foodMotor = 0) ;
  (lamp1 = 0) ;
  (lamp2 = 0) ;
  (heater = 0) ;
  (redLED = 0) ;
  (blueLED = 0) ;

  (greenLED = 1) ;
 while (1) {

 tempC = ADC_Read(0)*0.496;

 quality = ADC_Read(1)*0.496;

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
 if (tempC <  20 ) {
  (redLED = 1) ;
  (buzzer = 1) ;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, "Temp Critical!");
 ii++;
 EEPROM_Write(0x02,ii);
 Delay_ms(3000);

 }
 else if (quality <  70 ) {
  (blueLED = 1) ;
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(2, 1, "quality low");
 ii++;
 EEPROM_Write(0x02,ii);
 Delay_ms(4000);

 }
 else{
  (buzzer = 0) ;
  (foodMotor = 0) ;
  (lamp1 = 0) ;
  (lamp2 = 0) ;
  (heater = 0) ;
  (redLED = 0) ;
  (blueLED = 0) ;
  (greenLED = 1) ;
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
  (redLED = 1) ;
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
