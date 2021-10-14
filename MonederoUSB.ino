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
String cod = "";
char *codigos[] = {
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
    "L63JWKIIWHSC43",
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
    "L63JWKIIWHSC43"
    };
int aux;

int tamanio = sizeof(codigos);
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

  // lcd.setCursor(0, 0);
  //lcd.print("Buenas Tardes");
  int tamanio = sizeof(codigos);
  //int aux=EEPROM.read (0);
}

void loop()
{
  int tamanio = sizeof(codigos)/sizeof(codigos[0]);
  //EEPROM.update (0, 0);
  aux = EEPROM.read(0);

  if (bInserted)
  { // Validamos si hay una moneda insertada.
    lcd.clear();
    bInserted = false;          //Apagamos la variable de control
    digitalWrite(ledpin, HIGH); //Encendemos el led

    lcd.setCursor(0, 0);
    Serial.println("COIN"); //Imprimimos en el puerto serial la palabra "COIN"

    if (aux < tamanio)
    {
      Serial.println(EEPROM.read(0));

      lcd.print("Su codigo es :");
      lcd.setCursor(0, 1);
      lcd.print(codigos[EEPROM.read(0)]);
      // lcd.print(t-(millis() / 1000));
      delay(2000); //Esperamos un segundo.
      aux++;
      EEPROM.update(0, aux);
      Serial.println("NOP");
    }

    lcd.clear();
  }
  else
  {

    // Apagamos el led.
    //lcd.clear();
    digitalWrite(ledpin, LOW);
    lcd.setCursor(0, 1);
    //lcd.print( t-(millis() / 1000));
    lcd.print(";D");
    lcd.setCursor(0, 0);
    lcd.print("Buen Dia");
  }  
}



// Interrupcion.
void coinInterrupt()
{

  // Cada vez que insertamos una moneda valida, incrementamos el contador de monedas y encendemos la variable de control,
  pulse++;
  bInserted = true;
}
