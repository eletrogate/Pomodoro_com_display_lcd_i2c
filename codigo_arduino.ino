#include <Wire.h> // Biblioteca Wire responsável por conter funções de comunicação entre dispositivos através do protocolo I2C.
#include <LiquidCrystal_I2C.h>//Biblioteca I2C
LiquidCrystal_I2C lcd(0x38,16,2);  // Criando um LCD de 16x2 no endereço 0x38

/*Definição de Pinos*/
#define led_Verde 5 // Pino do led
#define buzzer 4 // Pino do buzzer

/*Criando as variáveis*/
int pomodoro_estudo = 25; //Tempo de estudo
int pomodoro_descanso = 10; //Tempo de descanso
signed short minutos, segundos, tipos, periodo =0;//Variáveis para a contagem do tempo
char tela[16];//Definindo a variável para conter os caracteres de tempo
int tempo = 200;// Variável para definir a velocidade da música
int divisor = 0, duracaoNota = 0;


int musica[] = {
  // Super Mario Bros theme
  // site da partitura - https://musescore.com/user/2123/scores/2145
  // Koji Kondo
  //Existem dois valores por nota, o primeiro é a altura da nota e o segundo é a sua duração. Ex: 659,8
  
  659,8, 659,8, 0,8, 659,8, 0,8, 523,8, 659,8, 
  784,4, 0,4, 392,8, 0,4, 
  523,-4, 392,8, 0,4, 330,-4, 
  440,4, 494,4, 466,8, 440,4,
  392,-8, 659,-8, 784,-8, 880,4, 698,8, 784,8,
  0,8, 659,4,523,8, 587,8, 494,-4,
  523,-4, 392,8, 0,4, 330,-4, 
  440,4, 494,4, 466,8, 440,4,
  392,-8, 659,-8, 784,-8, 880,4, 698,8, 784,8,
  0,8, 659,4,523,8, 587,8, 494,-4,
};

int notas = sizeof(musica) / sizeof(musica[0]) / 2; // Fornecemos o número de byte, cada valor int é composto por 2 bytes (16 bits)
int notaInteira = (60000 * 4) / tempo;// Calcula o valor da nota.


void setup() {
 //Iniciamos o Display LCD
 lcd.init();
 lcd.backlight();
 //Declarando os pinos usados como Saída
 pinMode(led_Verde, OUTPUT);
 pinMode(buzzer, OUTPUT);

}

void loop() {
  //Print no Display o tempo percorrido
  lcd.setCursor(0,0);
  lcd.print("Tempo decorrido: ");
  lcd.setCursor(1,1);
  sprintf(tela, "%.2d min %.2d seg", minutos, segundos);
  lcd.print(tela);
  //Espera um segundo para adicionar +1 na variável segundos
  delay(1000);
  segundos++;

  //Se os segundos for igual a 60, seta a variável segundos para 0 e acrescenta +1 na variável mintuos
  if(segundos == 60){
    segundos =0;
    minutos++;
  }
  
  //Se os minutos for igual a 25 e o período for igual a 0
  if(minutos == pomodoro_estudo && periodo == 0){
    minutos = 0; //A variável minutos seta para 0
    periodo = 1; // A variável periodo recebe 1 para informar que estamos na contagem descanso
    digitalWrite(led_Verde, HIGH);//Liga o led verde

    delay(1000);

    //Toca a musica do mário
    for (int notaAtual = 0; notaAtual < notas * 2; notaAtual = notaAtual + 2) {

    // Calcula a duração de cada nota
    divisor = musica[notaAtual + 1];
    if (divisor > 0) {
      // Nota normal, lida sem nenhuma interferência
      duracaoNota = (notaInteira) / divisor;
    } else if (divisor < 0) {
      //Durações negativas representa notas pontilhadas. 
      duracaoNota = (notaInteira) / abs(divisor);
      duracaoNota *= 1.5; // Aumenta a duração da nota pela metade
    }

    // Cada nota é tocada apenas 90% do seu tempo, os outros 10% é para pausas
    tone(buzzer, musica[notaAtual], duracaoNota * 0.9);

    // Espera a duração específica da nota acabar para tocar a próxima
    delay(duracaoNota);
    noTone(buzzer);
    }

  //se minutos for igual ao tempo do intervalo de descanso e o período igual a 1 
  }else if(minutos == pomodoro_descanso && periodo == 1){
    minutos =0; //A variável minutos seta para 0
    periodo = 0;  //A variável periodo recebe 0 para informar que estamos na contagem descanso
    digitalWrite(led_Verde, LOW); //Desliga o led verde
    delay(1000);
  }
}