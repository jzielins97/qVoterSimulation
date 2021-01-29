#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <vector>
#include <cmath>
#include <fstream>
#include <string.h>

/**************************************************************************
funkcja do losowania lobby dla danego węzła z grafu pełnego
    wejście:
      int id -> index węzła dla którego losujemy
      int size -> wielkość lobby jakie chcemy
      std::vector<bool> my_vector -> wektor wszystkich węzłów grafu pełnego (true/false)
    wyjście:
      std::vector<bool> choices -> wektor lobby z wartościami węzłów z grafu pełnego
***************************************************************************/
std::vector<bool> get_lobby(int id, int size, std::vector<bool> my_vector){
  std::set<int> indexes; //wektor z indeksami węzłów, które są wybrane do lobby
  std::vector<bool> choices; //wector z końcowym lobby
  int max_index = my_vector.size();
  while (indexes.size() < std::min(size, max_index))
  {
      int random_index = rand() % max_index; //losowanie członka lobby
      if (indexes.find(random_index) == indexes.end() && random_index != id) //sprawdzanie czy dany indeks nie był już użyty
      {
          choices.push_back(my_vector[random_index]);
          indexes.insert(random_index);
      }
  }
  return choices;
}

/**************************************************************************
funkcja do wykonania wszystkoch kroków czasowych symulacji.
  wejście:
    std::vector<bool> layer -> wektor węzłów z grafu pełnego
    int q -> wielkość lobby
    float p -> prawdopodobieństwo nie bycia konformistą
    int N -> ilość małych kroków czasowych w każdym "dużym" kroku (zakładamy 10000 dużych kroków czasowych)
             duży krok czasowy -krok czasowy, po którym każdy z węzłów powinien zostać wylosowany raz
  wyjście:
    float m_avg -> średnia magnetyzacja na końcu symulacji
**************************************************************************/
float do_simulation(std::vector<bool> layer, int q, float p, int N=5000){
  float m_avg = 0; //średnia magnetyzacja

  for(int it=0; it<N*1e3; it++){ //pętla po czasie (zakładamy po 5000 powtórzeń dla każdego węzła)
    int il = rand()%layer.size();

    //pierwszy poziom---------------------------------------------------------->
    bool val1 = layer[il]; //wartość wylosowanego węzła na poziomie 1
    float draw = 1.0*rand()/RAND_MAX;
    if(draw > p){ //prawdopodobieństwo dla którego losujemy lobby
      std::vector<bool> lobby = get_lobby(il,q,layer);
      bool check = true;
      for(int i=1; i<lobby.size();i++){ //sprawdzanie czy lobby jest jednolite
        if(lobby[i] != lobby[0]){
          check = false;
          break;
        }
      }
      if(check) val1 = lobby[0]; //lobby jest jednolite -> zmiana węzła na lobby
    }else if(draw > p/2) val1 = !layer[il]; //sprawdzamy czy zmienia zdanie samowolnie
    //koniec pierwszego poziomu------------------------------------------------<

    //drugi poziom------------------------------------------------------------->
    bool val2 = layer[il]; //wartość wylosowanego węzła na poziomie 2
    draw = 1.0*rand()/RAND_MAX;
    if(draw > p){ //prawdopodobieństwo dla którego losujemy lobby
      std::vector<bool> lobby = get_lobby(il,q,layer);
      bool check = true;
      for(int i=1; i<lobby.size();i++){
        if(lobby[i] != lobby[0]){
          check = false;
          break;
        }
      }
      if(check) val2 = lobby[0]; //lobby jest jednolite -> zmiana węzła na lobby
    }else if(draw > p/2) val2 = !layer[il]; //sprawdzamy czy zmienia zdanie samowolnie
    //koniec drugiego poziomu--------------------------------------------------<

    //aktualizacja węzłów
    if(val1 == val2) layer[il] = val1;

  }//koniec pętli czasowej

  for(int i=0; i<layer.size();i++){
    if(layer[i]) m_avg += 1;
    else m_avg -= 1;
  }
  m_avg = m_avg/layer.size();
  return m_avg;
}


int main(int argc, char* argv[]){
  if(argc < 4){
    std::cout<<"ERROR: use "<<argv[0]<<" <q> <N> <R> optional{<dp> <p_max> <p_min>}"<<std::endl;
    return -1;
  }

  int q = atoi(argv[1]); //wielkość lobby
  int N = atoi(argv[2]); //wielkość grafu
  int R = atoi(argv[3]); //liczba realizacji (do uśredniania)

  float p_min = 0;
  float p_max = 1.0;
  float dp = 0.05;
  if(argc > 4) dp = atof(argv[4]);
  if(argc > 5) p_max = atof(argv[5]);
  if(argc > 6) p_min = atof(argv[6]);

  //output stream set up
  char filename[100];
  sprintf(filename,"output/q%d_N%d_R%d_p%.1f-%.1f_%.2f.txt",q,N,R,p_min,p_max,dp);
  std::ofstream output(filename);

  srand(time(nullptr));
  std::vector<bool> layer;
  for(float p=p_min; p<=p_max; p+=dp){ //101
    float m_avg = 0;

    for(int ir=0; ir<R; ir++){
      //tworzenie grafu pełnego (jeden poziom, ponieważ poziomy są takie same aż do losowania osobnika)
      if(layer.size()>0) layer.clear();
      for(int i=0; i<N; i++){
        float r = 1.0*rand()/RAND_MAX;
        if(r < 0.6) layer.push_back(true);
        else layer.push_back(false);
      }

      m_avg += do_simulation(layer,q,p,N); //wykonanie symulacji
    }
    output<<p<<" "<<m_avg/R<<std::endl;
    std::cout<<"Done "<<p<<std::endl;
  }
  output.close();

  return 0;
}
