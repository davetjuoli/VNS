#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <set>
#include <cstring>
#define vb vector<bool>
#define vg vector<G>
#define TT 350
#define INF 987654321
using namespace std;
//editando com vs code =)
struct G{
  int a, b, c, d; //vértices que compõe a garra específica
};

time_t ini, fim, tempo_max;
int qtde_vertices;
bool aresta[TT][TT];
//vg l_garras; //lista com todas as garras do grafo //descartada (!)
vg l[TT]; //lista de garras relacionadas com vértices centrais 

void imprime(vb s);
vb s_aleatoria2(int tam);
vb s_aleatoria3(int tam);
void lista_de_garras();
bool garras(const vb &s);
vb melhor_v(const vector<vb> &vizinhos, const vb &s);
int f_objetivo(const vb &s);
vb bagunca(const vb &s, int n);
vb BL1(const vb &s);
vb BL2(const vb &s);
vb VNS2(const vb &s);
vb VNS(const vb &s);

int main(int argc, char **argv){
  srand(time(NULL));
  vb s_inicial, s_final;
  string aux;
  int edge;
  //lê a entrada
  getline(cin,aux);
  cin >> qtde_vertices;
  for(int i=0; i<qtde_vertices; i++)
    cin >> aux;
  for(int i=0; i<qtde_vertices; i++){
    for(int j=i+1; j<qtde_vertices; j++){
      cin >> edge;
      if(edge > 0){
        aresta[i][j] = aresta[j][i] = 1;
      }
    }
  }

  if(qtde_vertices<=50)tempo_max=10;
  else if(qtde_vertices<=100)tempo_max=30;
  else if(qtde_vertices<=200)tempo_max=60;
  else tempo_max=120;

  time(&ini);
  lista_de_garras();

  s_inicial = s_aleatoria3(qtde_vertices);
  //imprime(s_inicial);

  if(strcmp( argv[1], "0" ) == 0 ){
    s_final = VNS(s_inicial);
  }
  /*
  else{
    s_final = VNS2(s_inicial);
  } 
  */
  cout<<" "<<f_objetivo(s_final)<<endl;
  return 0;
}

vb s_aleatoria2(int tam){
  vb v_disponiveis(tam,1), s(tam,0), s_linha(tam,0);
  while(!v_disponiveis.empty()){
    int u = rand()%v_disponiveis.size();
    s_linha[u] = 1;
    if(!garras(s_linha))
      s = s_linha;
    v_disponiveis.erase(v_disponiveis.begin()+u);
  }
  return s;
}

vb s_aleatoria3(int tam){
  vb s = s_aleatoria2(tam), s_linha;
  for(int i=1; i<=9; i++){
    s_linha = s_aleatoria2(tam);
    if(f_objetivo(s_linha) < f_objetivo(s))
      s = s_linha;
  }
  return s;
}

int f_objetivo(const vb &s){
  int penalidade = 0;
  for(int i=0; i<qtde_vertices; i++)
    if(!s[i])
      penalidade++;
  if(garras(s)){
    penalidade += qtde_vertices;
  }
  return penalidade;
}

vb bagunca(const vb &s, int n){
  vb s_nova;
  set<int> indices;
  set<int>::iterator it;
  int x;
  for(int i=0; i<n; i++){
    do{
      x = rand()%s.size();
      it = indices.find(x);
    }while(it!=indices.end());
    indices.insert(x);
  }
  it = indices.begin();
  for(int i=0; i<s.size(); i++){
    if(i == *it){
      s_nova.push_back(!s[i]);
      it++;
    }
    else s_nova.push_back(s[i]);
  }
  //imprime(s_nova);
  return s_nova;
}

vb melhor_v(const vector<vb> &vizinhos, const vb &s){
  int menor = INF, melhor_v_v;
  for(int i=0; i<vizinhos.size(); i++){
    int aux = f_objetivo(vizinhos[i]);
    if(aux <= menor){
      menor = aux;
      melhor_v_v = i;
    }
  }
  if(menor < f_objetivo(s))
    return vizinhos[melhor_v_v];
  return s;
}

vb BL1(const vb &s){
  vector<vb> vizinhos;
  for(int i=0; i<qtde_vertices; i++){
    vb aux;
    for(int j=0; j<qtde_vertices; j++){
      if(j==i)
        aux.push_back(!s[j]);
      else aux.push_back(s[j]);
    }
    vizinhos.push_back(aux);
  }
  return melhor_v(vizinhos, s);  
}

void imprime(vb s){
  for(auto x:s) cout<<x;
  cout<<endl;
}
/*
vb BL2(const vb &s){
  vector<vb> vizinhos;
  for(int i=0; i<qtde_vertices; i++){
    for(int j=0; j<qtde_vertices; j++){
      vb aux;
      for(int x=0; x<qtde_vertices; x++){
        if(x==i or x==j)
          aux.push_back(!s[x]);
        else aux.push_back(s[x]);
      }
      vizinhos.push_back(aux);
    }
  }
  return melhor_v(vizinhos,s);
}
*/

vb VNS(const vb &s){
  vb melhor_final = s, s_linha, s_atual = s;
  int b = 1, k = 1, max_b = s.size()/2, kc, tc;
  bool melhorou;

  while(difftime(fim,ini) < tempo_max){
    melhorou = false;

    if(k == 1) s_linha = BL1(s_atual);
    //else s_linha = BL2(s_atual);

    int aa = f_objetivo(s_linha), bb = f_objetivo(s_atual);
    if(aa < bb){

      s_atual = s_linha;
      melhorou = true;

      if(aa < f_objetivo(melhor_final)){
        melhor_final = s_linha;
        time(&fim);
        kc = k; tc = difftime(fim,ini);
        //cout<<"m_pontuaç: "<<aa<<" ";
        //imprime(s_linha);
        //cout<<" k = "<<k<<" t = "<<difftime(fim,ini)<<endl;   
      }
      k = 1;
    }else{
      k++;
    }
    if(melhorou == false and k == 2){
      vb aux = s_atual;
      do{
        aux = bagunca(s_atual, b);
      }while(garras(aux));
      s_atual = aux;
      k = 1;
      b++;
      if(b > max_b)
        b = max_b;
    }
    time(&fim);
  }
  //cout<<"vns1_sem_bl2 k = "<<kc<<" t = "<<tc;
  cout<<tc;
  return melhor_final;
}
/*
vb VNS2(const vb &s){
  vb melhor_final = s, s_atual = s, sc = s, sc_linha;
  int kc, tc;

  while(difftime(fim,ini) < tempo_max){
    int b = 0, k = 1, max_b = s.size()/2;

    while(b < max_b){
      if(b!=0)
        sc = bagunca(s_atual, b);
      //imprime(sc);
        //  cout<<" "<<b<<endl;

      while(k < 3){
        if(k == 1){
          //imprime(sc);
          sc_linha = BL1(sc);
          //imprime(sc_linha);
        } 
        //else sc_linha = BL2(sc);

        if(f_objetivo(sc_linha) < f_objetivo(sc)){
          sc = sc_linha;
          kc = k;
          k = 1;
        }else{
          k++;
        }
      }
      if(f_objetivo(sc) < f_objetivo(s_atual)){
        s_atual = sc;
        if(f_objetivo(s_atual) < f_objetivo(melhor_final)){
          melhor_final = s_atual;
          time(&fim);
          tc = difftime(fim,ini);
          //cout<<"m_pontuaç: "<<f_objetivo(melhor_final)<<" ";
          //imprime(melhor_final);
          //cout<<" k = "<<kc<<" t = "<<difftime(fim,ini)<<endl;
        }
        b = 1; //cout<<b<<" .. "<<endl;
      }else{
        b++;
        //cout<<b<<" .. "<<endl;
      }
    }
    time(&fim);
  }
  cout<<" vns2_com_bl2 k = "<<kc<<" t = "<<tc;
  return melhor_final;
}
*/
void lista_de_garras(){
  G aux;
  int tam = qtde_vertices;
  for(int v1=0; v1<tam; v1++){
    for(int v2=v1+1; v2<tam; v2++){
      for(int v3=v2+1; v3<tam; v3++){
        for(int v4=v3+1; v4<tam; v4++){
          if(aresta[v1][v2] and aresta[v1][v3] and aresta[v1][v4] and !aresta[v2][v3] and !aresta[v2][v4] and !aresta[v3][v4]){
            aux.a=v1; aux.b=v2; aux.c=v3; aux.d=v4;
            l[v1].push_back(aux);
            //cout<<v1<<" "<<v2<<" "<<v3<<" "<<v4<<endl;
          }
          if(aresta[v2][v1] and aresta[v2][v3] and aresta[v2][v4] and !aresta[v1][v3] and !aresta[v1][v4] and !aresta[v3][v4]){
            aux.a=v2; aux.b=v1; aux.c=v3; aux.d=v4;
            l[v2].push_back(aux);
            //cout<<v2<<" "<<v1<<" "<<v3<<" "<<v4<<endl;
          }
          if(aresta[v3][v1] and aresta[v3][v2] and aresta[v3][v4] and !aresta[v1][v2] and !aresta[v1][v4] and !aresta[v2][v4]){
            aux.a=v3; aux.b=v1; aux.c=v2; aux.d=v4;
            l[v3].push_back(aux);
            //cout<<v3<<" "<<v1<<" "<<v2<<" "<<v4<<endl;
          }
          if(aresta[v4][v1] and aresta[v4][v2] and aresta[v4][v3] and !aresta[v1][v2] and !aresta[v1][v3] and !aresta[v2][v3]){
            aux.a=v4; aux.b=v1; aux.c=v2; aux.d=v3;
            l[v4].push_back(aux);
            //cout<<v4<<" "<<v1<<" "<<v2<<" "<<v3<<endl;
          }
        }
      }
    }
  }
}

bool garras(const vb &s){
  bool tem = false;
  for(int v=0; v<s.size(); v++){
    if(s[v] and !l[v].empty()){
      for(auto garra:l[v]){
        if(s[garra.b] and s[garra.c] and s[garra.d]){
          tem = true;
          break;
        }
      }
    }
    if(tem)break;
  }
  if(tem) return true;
  return false;
}
