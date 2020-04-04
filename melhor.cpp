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

struct G{
  int a, b, c, d; //vertices que compoe a garra especiÂ­fica
};

time_t ini, fim, tempo_max;
clock_t inic, fimm;
int qtde_vertices;
bool aresta[TT][TT];
vector<int> aux;
vector<vector<int> > vizinhos(TT,aux);
//vg l_garras; //lista com todas as garras do grafo //descartada (!)
vg l[TT]; //lista de garras relacionadas com vertices centrais 

void imprime(vb s);
vb s_aleatoria(int tam);
vb aleatoria(int tam); 
void lista_de_garras();
bool garras(vb &s, bool remove);
bool busca_garras_min(vb &s, int v);
vb melhor_v(vector<vb> &vizinhos, vb &s);
int f_objetivo(vb &s);
vb bagunca(vb &s, int n);
vb BL1(vb &s);
vb VNS(vb &s);
vb VNS2(vb &s);

int main(int argc, char **argv){
  srand(time(NULL));
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
  /*
  for(auto v : vizinhos[1])
    cout<<v<<" ";
  cout<<endl;
  exit(10);
  */
  if(qtde_vertices<=50)tempo_max=10;
  else if(qtde_vertices<=100)tempo_max=30;
  else if(qtde_vertices<=200)tempo_max=60;
  else tempo_max=120;

  time(&ini);
  inic = clock();
  lista_de_garras();
  /*
  vb s(50,1);
  s[4]=0; s[15]=0; s[16]=0; s[22]=0; s[29]=0; s[30]=0; s[31]=0; s[32]=0; s[33]=0; s[36]=0; s[39]=0; s[47]=0; s[48]=0;
  if(garras(s,0)) cout<<"Tem garras"<<endl;
  else cout<<"NÃ£o tem garras"<<endl;
  exit(10);
  */
  s_inicial = aleatoria(qtde_vertices);
  //imprime(s_inicial);
  //cout<<"aaaaaa"<<endl;

  if(strcmp(argv[1],"0")==0)
    s_final = VNS(s_inicial);
  else s_final = VNS2(s_inicial);
  cout<<" "<<f_objetivo(s_final)<<endl;

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
    if(!garras(s_linha,0))
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
  if(garras(s,0)) penalidade += qtde_vertices;
  return penalidade;
}

vb bagunca(vb &s, int n){
  if(n == 0)
    return s;
  vb s_nova;
  set<int> indices;
  set<int>::iterator it;
  int x;
  for(int i = 0; i < n; i++){
    do{
      x = rand()%s.size();
      it = indices.find(x);
    }while(it != indices.end());
    indices.insert(x);
  }
  
  for(int i = 0; i < s.size(); i++){
    it = indices.find(i);
    if(it != indices.end())
      s_nova.push_back(!s[i]);
    else s_nova.push_back(s[i]);
  }
  return s_nova;
}

vb melhor_v(vector<vb> &vizinhos, vb &s){
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
  while(garras(aux,1));

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
      if(busca_garras_min(aux,i)){
        aux[i]=0;
      } 
    }
  }
  return aux;
}

vb VNS(vb &s){
  vb melhor_final = s, s_linha, s_atual = s;
  int b = 1, k = 1, max_b = s.size()/2, kc, tc;
  bool melhorou;

  while(difftime(fim,ini) < tempo_max){
    melhorou = false;

    if(k == 1) s_linha = BL1(s_atual);
    //imprime(s_linha);
    //imprime(s_atual);
    int aa = f_objetivo(s_linha), bb = f_objetivo(s_atual);
    //cout<<aa<<" --- "<<bb<<endl;
    if(aa < bb){
      s_atual = s_linha;
      melhorou = true;

      if(aa < f_objetivo(melhor_final)){
        melhor_final = s_linha;
        time(&fim);
        kc = k; tc = difftime(fim,ini);
        cout<<"m_pontuacao: "<<aa<<" ";
        imprime(s_linha);
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
      }while(garras(aux,0));
      //cout<<"bag"<<endl;
      s_atual = aux;
      k = 1;
      b++;
      if(b > max_b)
        b = max_b;
    }
    time(&fim);
  }
  cout<<tc;
  return melhor_final;
}

vb VNS2(vb &s){
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
            //cout<<v<<" "<<v1<<" "<<v2<<" "<<v3<<endl;
          }
        }
      }
    }    
  }
}

bool garras(vb &s, bool remove){ //busca por garras nas listas de todos os vertices
  if(remove){
    vector<G> g;
    bool tem = false;
    for(int v=0; v<s.size(); v++)
      if(s[v] and !l[v].empty())
        for(auto garra:l[v])
          if(s[garra.b] and s[garra.c] and s[garra.d]){
            tem = true;
            g.push_back(garra);
          }
    if(tem){
      G garra = g[rand()%g.size()]; //escolhe uma garra aleatoria
      int v = rand()%4;
      if(v==0) s[garra.a]=0;
      else if(v==1) s[garra.b]=0;
      else if(v==2) s[garra.c]=0;
      else s[garra.d]=0;
      return true;
    }
    return false;
  }
  for(int v=0; v<s.size(); v++)
    if(s[v] and !l[v].empty())
      for(auto garra:l[v])
        if(s[garra.b] and s[garra.c] and s[garra.d])
          return true;
  return false;
}

bool busca_garras_min(vb &s, int v){ //busca por garras nas listas de v e dos vizinhos de v
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