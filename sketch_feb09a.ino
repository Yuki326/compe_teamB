//--------/
//   設定 ★がついてる場所の値を変更して調整をお願いします。
//---------/
#include <MsTimer2.h>

#define MOTER 13//使用するピン番号 ★
#define LED 2 //★

#define MOTERTIME 10000 //モータの稼働時間 単位ms ★
#define TIMELIMIT 30000

/* LED設定 */

#define SELECT 3//ここでパターンを指定 // ★

/*点滅パターン 1がON、0がOFF */
int64_t pattern[] ={//64bitまで格納
  0b0000101101010,//パターン１　右から読む
  0b000101101101100111101101100,
  0b01110111011100010111000111011101110001110111,
  0b111011101110111011101110111011101110111011101110,
  };

int interval[] = {1000,500,300,300};

/* モーター設定 */

#define CYCLE 10 //pwmの１周期にかける時間　単位 ms ★
#define MAXPOWER 5//pwmの最大出力時に１周期のうち何m秒をONにするか ★
#define KASOKUTIME 4000//加速,減速にかける時間 ms ★
int KASOKUCYCLE = KASOKUTIME/MAXPOWER;

//-------------------/
//  制御
//------------------/
int power = 0;//初期化
int counter = 0;
void setup()
{
//  Serial.begin(9600);  
  pinMode(MOTER, OUTPUT);
  pinMode(LED, OUTPUT);
  MsTimer2::set(1, controlMoter); 
}

void loop()
{
  unsigned long time = 0;
  
  MsTimer2::start();
  while(time <= MOTERTIME){
    time = (unsigned long)millis();//プログラム開始時点からの経過時間を取得
  }
  MsTimer2::stop();
  digitalWrite(MOTER,LOW);
  
  while(time <= TIMELIMIT){
    time = (unsigned long)millis();//プログラム開始時点からの経過時間を取得
    digitalWrite(LED, (pattern[SELECT-1]>>(time-MOTERTIME)/interval[SELECT-1])&1);
//    Serial.print(time%10);
  }
  while(1){
    delay(1);
  }
}
void controlMoter(){
  digitalWrite(MOTER,counter % CYCLE < power);
  if(counter % KASOKUCYCLE == 0){
    if(counter <= KASOKUTIME){
      if(power < MAXPOWER){
        power++;
//      analogWrite(MOTER,power*25);
      }
    }
    if(MOTERTIME - counter <= KASOKUTIME){
      if(power > 0){
        power --;
//        analogWrite(MOTER,power*25);
      }
    }
  }
  counter++;
}