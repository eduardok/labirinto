#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
/*************************************
* Trabalho de Inteligencia Artificial
* ============ LABIRINTO ============
* Autor : Eduardo Bacchi Kienetz
* Criado: 15/JUNHO/2006
* Executar: ./l.exe labirinto3.txt
*************************************/
using namespace std;

#define TAM   10
#define CIMA   0
#define DIR    1
#define BAIXO  2
#define ESQ    3
#define PAREDE 0
#define VAZIO  1

int   matriz[TAM][TAM];
class Agente;
void  mostraLabirinto(Agente &ag);

void sleep(int nbr_seconds){
   clock_t goal;
   goal = (nbr_seconds * CLOCKS_PER_SEC) + clock();
   while(goal > clock());
}  

int aleatorio(int ate){
   sleep(1);
   time_t seconds = time(&seconds);
   srand((unsigned int)seconds);
   return (rand()%ate);
}

class Agente {
   private:
      int linha,coluna,orientacao,vail,vaic,avaliacao1,avaliacao2,avaliacao3,avaliacao4;
      
      bool frente(int oq){
         return (((orientacao==CIMA)  && (matriz[linha-1][coluna]==oq)) ||
                 ((orientacao==BAIXO) && (matriz[linha+1][coluna]==oq)) ||
                 ((orientacao==ESQ)   && (matriz[linha][coluna-1]==oq)) ||
                 ((orientacao==DIR)   && (matriz[linha][coluna+1]==oq)));
      }
      
      bool frente(int l, int c){
         return (((orientacao==CIMA)  && (linha-1==l) && (coluna==c)) ||
                 ((orientacao==BAIXO) && (linha+1==l) && (coluna==c)) ||
                 ((orientacao==ESQ)   && (linha==l) && (coluna-1==c)) ||
                 ((orientacao==DIR)   && (linha==l) && (coluna+1==c)));
      }
      
      bool parede(int l, int c) { return (matriz[l][c]==PAREDE); }
      
      void gira(int lado){
//         mostraLabirinto(*this);
         if(lado==ESQ){
            if(orientacao==CIMA) orientacao = ESQ;
            else orientacao--;
         } else { //gira pra direita
            if(orientacao==ESQ) orientacao = CIMA;
            else orientacao++;
         }
//         sleep(1);
//         mostraLabirinto(*this);
      }
   
   public:
      Agente(){
         linha = coluna = 0;
         orientacao = aleatorio(3);
      }
      ~Agente(){}
 
      void setPosicao(int l, int c){
         linha = l;
         coluna = c;
         matriz[l][c]++;
      }

      bool achouFim(){ return ((linha==TAM-1) || (coluna==TAM-1) || (linha==0) || (coluna==0)); }

      void anda(){
         while(frente(PAREDE)){
            gira(DIR); //gira pra direita pra ver se ainda tem parede
            if(frente(PAREDE)){  //se tem parede
               gira(ESQ); //prefere o outro lado
               gira(ESQ); 
            }
         }
         
         avaliacao1 = avaliacao2 = avaliacao3 = avaliacao4 = 999999;
         if(!parede(linha-1,coluna)) {
            vail = linha-1;
            vaic = coluna;
            avaliacao1 = matriz[linha-1][coluna];
         }
         if(!parede(linha+1,coluna)) {
            avaliacao2 = matriz[linha+1][coluna];
            if(avaliacao2+(orientacao==BAIXO ? -1 : 0)<avaliacao1){
               vail = linha+1;
               vaic = coluna;
            }
         }
         if(!parede(linha,coluna-1)) {
            avaliacao3 = matriz[linha][coluna-1];
            if((avaliacao3+(orientacao==ESQ ? -1 : 0)<avaliacao2) && (avaliacao3+(orientacao==ESQ ? -1 : 0)<avaliacao1)){
               vail = linha;
               vaic = coluna-1;
            }
         }
         if(!parede(linha,coluna+1)) {
            avaliacao4 = matriz[linha][coluna+1];
            if((avaliacao4+(orientacao==DIR ? -1 : 0)<avaliacao3) && (avaliacao4+(orientacao==DIR ? -1 : 0)<avaliacao2) && (avaliacao4+(orientacao==DIR ? -1 : 0)<avaliacao1)){
               vail = linha;
               vaic = coluna+1;
            }
         }
         while(!frente(vail,vaic)) gira(DIR);
         setPosicao(vail,vaic);
      }

      int getLinha()     { return linha; }
      int getColuna()    { return coluna; }
      int getOrientacao(){ return orientacao; }
};

void carregaArquivo(Agente &agente, char * qarq){
   ifstream arq;
   string linha;
   int l = 0;
   
   arq.open(qarq,ios::out);
   if(arq.is_open()){
      while (!arq.eof()) {
         getline(arq,linha);
         for(int t=0;t<TAM;t++){
            matriz[l][t] = ( (linha[t]-48)==1 ? PAREDE : VAZIO );
            if((linha[t]-48)==2) agente.setPosicao(l,t); //Guarda posição do agente fora da matriz
         }
         l++;
      }
      arq.close();
   }
}

void mostraLabirinto(Agente &agente){
   system("clear");
   for(int i=0;i<TAM;i++) {
      for(int x=0;x<TAM;x++){
         if((agente.getLinha()==i) && (agente.getColuna()==x)) {
            cout << "\033[31;1m";
            if     (agente.getOrientacao()==CIMA)  cout << "^";
            else if(agente.getOrientacao()==BAIXO) cout << "_";
            else if(agente.getOrientacao()==ESQ)   cout << "<";
            else                                   cout << ">";   
            cout << "\033[0m";
         } else {
            if(matriz[i][x]==PAREDE) cout << "\033[46;1m" << " " << "\033[0m";
            else                     cout << matriz[i][x]; //" ";  //vazio
         }
      }
      cout << endl;
   }
}

int main(int argc, char *argv[])
{
   Agente agente;
   int TEMPO = (argc>2 ? atoi(argv[2]) : 1 );
   
   if(argc<2){
      cout << "\n\tUso: " << argv[0] << " labirinto.txt [delay]\n\n";
      exit(1);
   }
   
   carregaArquivo(agente,argv[1]);
   system("clear");
   while(!agente.achouFim()){
      mostraLabirinto(agente);
      agente.anda();
      sleep(TEMPO);
   }
   mostraLabirinto(agente);
   exit(0);
}
