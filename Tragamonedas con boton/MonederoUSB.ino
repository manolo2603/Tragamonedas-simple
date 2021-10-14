#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7
const int coinpin = 2;                            //pin al cual estara conectada la señal del monedero
const int ledpin = 13;                            //pin al cual estara conectado un led, para indicar cuando una moneda es aceptada.
volatile int pulse = 0;                           //Contador de monedas/pulsos insertadas.
boolean bInserted = false;                        //Variable de control para identificar cuando una moneda es insertada.
int msg = 0;
int t = 20;
int pulsador = 0;
String cod = "";
int contadorMonedas = 0;
char *codigosUnaHora[] = {
   "Q2KXZFH335HQ6",
    "R4C2ABZLLRDL63",
    "GCLWHAR4BTMJX",
    "HYWNMBJEOGSZ4",
    "SR4MDMEFOXXD23",
    "TXEJDXJXGG54A3",
    "YXLMFSB3CKJ3R",
    "TMPEJLPBTKWK53",
    "MCNDMI3RRB44O",
    "ZTQLHZKOGSMLO",
    "BIH6AHMOTFTSN",
    "YAHAC5JGAZPDP",
    "J5XQNWOC34GC43",
    "BCPIFCOXMZAPY",
    "LOASZIZRMIPNT",
    "Z3HLCBEZ6YWZ5",
    "HQXDFMHBC5L6L",
    "J36RXZN3YM5YT",
    "K6MHGXGNAHD333",
    "HRADKSCNN2FKM",
    "2X52GWZCXYG6N",
    "OS42PJI2MQ2TZ",
    "WAYQTHJ5KLWPT",
    "JJBTAZX22TMW3",
    "RDZBEWLA4WPM53",
    "JI5I5MJ32ZRQ63",
    "OH5PITCNEOID",
    "3Y6QDOELJ3ILF",
    "XJJ55FYN54R25",
    "LI5T6DXWW2DO63",
    "XYCI3KA2NZIAT",
    "L6BSSQMQPIJRF",
    "OP2SM4YMFNZG3",
    "BOXJEOJHNTWMZ",
    "6BXCLMFIM4QIY",
    "CXKDRZMNBDWR4",
    "GKJLJFOIBZIFT",
    "JLQN6SEH3KMYF",
    "XMIDSFIZTAPZ43",
    "LJM5M5QR6XDZH",
    "WK6262MCMIGO",
    "DHRBF4FNOAXSW",
    "S6TJ5TK3PMKGL",
    "3AGASEQEOEQNY",
    "JLZH4RABZNW2M",
    "KBMS4NTEZ6QAS",
    "M6QOCSBF6CKOP",
    "AXAPPGBR2BLP53",
    "L63JWKIIWHSC43"};

char *codigos2horas[] = {
   "2Q2KXZFH335HQ6",
    "2R4C2ABZLLRDL63",
    "2GCLWHAR4BTMJX",
    "2HYWNMBJEOGSZ4",
    "2SR4MDMEFOXXD23",
    "2TXEJDXJXGG54A3",
    "2YXLMFSB3CKJ3R",
    "2TMPEJLPBTKWK53",
    "2MCNDMI3RRB44O",
    "2ZTQLHZKOGSMLO",
    "2BIH6AHMOTFTSN",
    "2YAHAC5JGAZPDP",
    "2J5XQNWOC34GC43",
    "2BCPIFCOXMZAPY",
    "2LOASZIZRMIPNT",
    "2Z3HLCBEZ6YWZ5",
    "2HQXDFMHBC5L6L",
    "2J36RXZN3YM5YT",
    "2K6MHGXGNAHD333",
    "2HRADKSCNN2FKM",
    "22X52GWZCXYG6N",
    "2OS42PJI2MQ2TZ",
    "2WAYQTHJ5KLWPT",
    "2JJBTAZX22TMW3",
    "2RDZBEWLA4WPM53",
    "2JI5I5MJ32ZRQ63",
    "2OH5PITCNEOID",
    "23Y6QDOELJ3ILF",
    "2XJJ55FYN54R25",
    "2LI5T6DXWW2DO63",
    "2XYCI3KA2NZIAT",
    "2L6BSSQMQPIJRF",
    "2OP2SM4YMFNZG3",
    "2BOXJEOJHNTWMZ",
    "26BXCLMFIM4QIY",
    "2CXKDRZMNBDWR4",
    "2GKJLJFOIBZIFT",
    "2JLQN6SEH3KMYF",
    "2XMIDSFIZTAPZ43",
    "2LJM5M5QR6XDZH",
    "2WK6262MCMIGO",
    "2DHRBF4FNOAXSW",
    "2S6TJ5TK3PMKGL",
    "23AGASEQEOEQNY",
    "2JLZH4RABZNW2M",
    "2KBMS4NTEZ6QAS",
    "2M6QOCSBF6CKOP",
    "2AXAPPGBR2BLP53",
    "2L63JWKIIWHSC43"};

int aux;
int aux2;

int tamanio = sizeof(codigosUnaHora);
int tamanio2 = sizeof(codigos2horas);
void setup()
{
  pinMode(4, INPUT);
  // Inicializamos el puerto serial.
  Serial.begin(9600);

  //Agregamos la interrupcion con el pin indicado.
  attachInterrupt(digitalPinToInterrupt(coinpin), coinInterrupt, RISING);

  //Asignamos el pin para el led como salida
  pinMode(ledpin, OUTPUT);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16, 2);
  lcd.clear();
  t = 20;
}

void loop()
{

  //EEPROM.update (0, 0);
  //EEPROM.update (1, 0);
  aux = EEPROM.read(0);
  aux2 = EEPROM.read(1);

  pulsador = digitalRead(4); //lee el estado del botón
  if (pulsador == HIGH)
  { //si el estado es pulsado
    Serial.println("Pulsado");
    getCodigos();
  }
  else
  {
    digitalWrite(ledpin, LOW);
    lcd.setCursor(0, 1);
    //lcd.print( t-(millis() / 1000));
    lcd.print(";D");
    lcd.setCursor(0, 0);
    lcd.print("Buen Dia");
  }

  if (bInserted)
  {                    // Validamos si hay una moneda insertada.
    bInserted = false; //Apagamos la variable de control
    lcd.setCursor(0, 0);
    contadorMonedas++;
    Serial.println("Moneda Insertada");
  }
}

// Interrupcion.
void coinInterrupt()
{

  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
  pulse++;
  bInserted = true;
}

void getCodigos()
{
  lcd.clear();

  if (aux < tamanio || aux2 < tamanio2)
  {
    if (contadorMonedas == 1)
    {
      Serial.println(EEPROM.read(0));
      lcd.setCursor(0, 0);
      lcd.print("Codigo 1 Hrs es :");
      lcd.setCursor(0, 1);
      lcd.print(codigosUnaHora[EEPROM.read(0)]);
      //Serial.println("Codigo as :" + codigosUnaHora[EEPROM.read(0)]);
      // lcd.print(t-(millis() / 1000));
      delay(2000); //Esperamos un segundo.
      aux++;
      EEPROM.update(0, aux);
      Serial.println("NOP");
      contadorMonedas = 0;
    }
    else if (contadorMonedas == 2)
    {
      Serial.println(EEPROM.read(0));

      lcd.print("Codigo 2 Hrs es :");
      lcd.setCursor(0, 1);
      lcd.print(codigos2horas[EEPROM.read(0)]);
      // lcd.print(t-(millis() / 1000));
      delay(2000); //Esperamos un segundo.
      aux2++;
      EEPROM.update(0, aux2);
      Serial.println("NOP");
      contadorMonedas = 0;
    }else{
      lcd.print("Maximo 2 monedas");
      lcd.setCursor(0, 1);
      lcd.print("de $5");
       delay(2000);
       contadorMonedas = 0;
      
    }
  }

  lcd.clear();
}
