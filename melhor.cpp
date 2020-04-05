#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <set>
#include <cstring>
#define vb vector<bool>
#define vvb vector<vb>
#define vg vector<G>
#define TT 350
#define INF 987654321
using namespace std;

struct G{
  int a, b, c, d; //vertices que compoe a garra especiÂ­fica
};

time_t tempo_max;
clock_t inic, fimm;
int qtde_vertices, qtde_garras=0;
bool aresta[TT][TT];
vector<int> aux;
vector<vector<int> > vizinhos(TT,aux);
vg l[TT]; //lista de garras relacionadas com vertices centrais 

void imprime(vb s);
vb s_aleatoria(int tam);
vb aleatoria(int tam);
void lista_de_garras();
bool garras(vb &s);
bool garras_min(vb &s, int v);
void remove(vb &s);
vb melhor_v(vvb &vizinhos, vb &s);
int f_objetivo(vb &s);
vb bagunca(vb &s, int n);
vb BL1(vb &s);
vb VNS(vb &s);

long int seed;

int main(int argc, char **argv){
  //srand(time(NULL));
  seed=time(NULL);
  srand(seed);
  cout<<" "<<seed;
  vb s_inicial, s_final;
  string aux;
  int edge;
  //le a entrada
  getline(cin,aux);
  cin >> qtde_vertices;
  for(int i=0; i<qtde_vertices; i++) cin >> aux;

  for(int i=0; i<qtde_vertices; i++){
    for(int j=i+1; j<qtde_vertices; j++){
      cin >> edge;
      if(edge > 0){
        aresta[i][j] = aresta[j][i] = 1;
        vizinhos[i].push_back(j);
        vizinhos[j].push_back(i);
      }
    }
  }

  if(qtde_vertices<=50)tempo_max=10;
  else if(qtde_vertices<=100)tempo_max=5;
  else if(qtde_vertices<=200)tempo_max=60;
  else tempo_max=120;

  inic = clock();
  lista_de_garras();
  //cout<<"qtde de garras: "<<qtde_garras<<endl;

  s_inicial = aleatoria(qtde_vertices);

  s_final = VNS(s_inicial);

  cout<<" "<<f_objetivo(s_final)<<" "<<seed<<endl;

  return 0;
}

void imprime(vb s){
  for(auto x:s) cout<<x;
  cout<<endl;
}

vb s_aleatoria(int tam){ //funcao constroi solucao aleatoria
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

vb aleatoria(int tam){ //funcao chama 10 solucoes aleatorias e retorna a melhor
  vb s = s_aleatoria(tam), s_linha;
  for(int i=1; i<=9; i++){
    s_linha = s_aleatoria(tam);
    if(f_objetivo(s_linha) < f_objetivo(s))
      s = s_linha;
  }
  return s;
}

int f_objetivo(vb &s){
  int penalidade = 0;
  for(int i=0; i<qtde_vertices; i++)
    if(!s[i])
      penalidade++;
  if(garras(s)) penalidade += qtde_vertices;
  return penalidade;
}

vb bagunca(vb &s, int n){
  if(n==0) return s;
  map<int,bool> dic;
  int indices[n], t=0;
  for(int i=0; i<n; i++){
    int v;
    do{
      v=rand()%qtde_vertices;
    }while(dic[v]);
    dic[v]=1;
    indices[t++]=v;
  }
  sort(indices,indices + t);
  vb s_nova; t=0;
  for(int i=0; i<qtde_vertices; i++){
    if(i==indices[t]){
      s_nova.push_back(!s[i]);
      t++;
    }else s_nova.push_back(s[i]);
  }
  return s_nova;
}

vb melhor_v(vvb &vizinhos, vb &s){
  int menor = INF, melhor;
  for(int i = 0; i < vizinhos.size(); i++){
    int aux = f_objetivo(vizinhos[i]);
    if(aux <= menor){
      menor = aux;
      melhor = i;
    }
  }
  if(menor < f_objetivo(s))
    return vizinhos[melhor];
  return s;
}

vb BL1(vb &s){
  vb aux = s;
  while(garras(aux)){
    remove(aux);
  }
  int indices[qtde_vertices];
  for(int i=0; i<qtde_vertices; i++)
    indices[i]=i;
  for(int i=0; i<qtde_vertices; i++){
    int a = rand()%qtde_vertices, b = rand()%qtde_vertices;
    swap(indices[a],indices[b]);
  }
  for(int j=0; j<qtde_vertices; j++){
    int i = indices[j];
    if(!aux[i]){
      aux[i]=1;
      if(garras_min(aux,i)){
        aux[i]=0;
      } 
    }
  }
  return aux;
}

vb VNS(vb &s){
  vb melhor_final = s, s_atual = s;
  double t, tc;
  fimm = clock() - inic;
  t = (float)fimm/CLOCKS_PER_SEC;
  while(t < tempo_max){
    int b = 0, b_max = qtde_vertices;
    while(b < b_max){
      s_atual = bagunca(melhor_final,b);
      b++;
      s_atual = BL1(s_atual);
      if(f_objetivo(s_atual) < f_objetivo(melhor_final)){
        fimm = clock() - inic;
        tc = (float)fimm/CLOCKS_PER_SEC;
        melhor_final = s_atual;
        b = 1;
      }
    }
    fimm = clock() - inic;
    t = (float)fimm/CLOCKS_PER_SEC;
    s_atual = aleatoria(qtde_vertices);
  }
  cout<<" "<<tc;
  return melhor_final;
}

void lista_de_garras(){
  G aux;
  for(int v=0; v<qtde_vertices; v++){
    int tam = vizinhos[v].size(), v1, v2, v3;
    for(int i=0; i<tam; i++){
      v1 = vizinhos[v][i];
      for(int j=i+1; j<tam; j++){
        v2 = vizinhos[v][j];
        for(int k=j+1; k<tam; k++){
          v3 = vizinhos[v][k];
          if(!aresta[v1][v2] and !aresta[v1][v3] and !aresta[v2][v3]){
            aux.a=v; aux.b=v1; aux.c=v2; aux.d=v3;
            l[v].push_back(aux);
            qtde_garras++;
          }
        }
      }
    }    
  }
}

void remove(vb &s){
  int indices[qtde_vertices]; 
  int p=0;
  for(int i=0; i<qtde_vertices; i++) //conjunto de vertices presentes na solucao
    if(s[i])
      indices[p++]=i;
  for(int i=0; i<p; i++){
    int a=rand()%p, b=rand()%p;
    swap(indices[a],indices[b]); //embaralha
  } 
  for(int i=0; i<p; i++){ 
    int u = indices[i]; //para cada vertice u presente na solucao
    int tam = l[u].size();
    if(tam){
      int indices_u[tam]; //conjunto de garras com centro em u
      for(int j=0; j<tam; j++)
        indices_u[j] = j;
      for(int j=0; j<tam; j++){
        int a=rand()%tam, b=rand()%tam;
        swap(indices_u[a],indices_u[b]); //embaralha
      }
      for(int i=0; i<tam; i++){
        G g = l[u][indices_u[i]]; //para cada garra g com centro em u
        if(s[g.b] and s[g.c] and s[g.d]){ //verifica se a garra está presente na solucao corrente
          int v = rand()%4; //quebra aleatoriamente a garra
          if(v==0) s[g.a]=0;
          else if(v==1) s[g.b]=0;
          else if(v==2) s[g.c]=0;
          else s[g.d]=0;
          return;
        }
      }
    }
  }
  return;
}

bool garras(vb &s){
  for(int v=0; v<s.size(); v++)
    if(s[v] and !l[v].empty())
      for(auto garra:l[v])
        if(s[garra.b] and s[garra.c] and s[garra.d])
          return true;
  return false;  
}

bool garras_min(vb &s, int v){ //busca por garras nas listas de v e dos vizinhos de v
  if(!l[v].empty())
    for(auto garra:l[v])
      if(s[garra.b] and s[garra.c] and s[garra.d])
        return true;
      
  for(auto viz : vizinhos[v])
    if(s[viz] and !l[viz].empty())
      for(auto garra:l[viz])
        if(s[garra.b] and s[garra.c] and s[garra.d])
          return true;
  return false;
}