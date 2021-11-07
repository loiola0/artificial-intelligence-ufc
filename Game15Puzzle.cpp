#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <set>
#include <map>
#include <ctime>
#include <vector>
#include <queue>
#include <functional>
#include <locale.h>
#include <unistd.h>

using namespace std;

class Node;

bool idfs();
bool A_estrela();
bool dfs(int k);
bool gulosa();
bool bfs();

void read();


string print(Node *a);

bool temSolucao();
int contaInversoes(int v[]);
int meuSwap(int x);
int Manhattan (Node *x);

// para melhor desempenho em heurísticas
void   criaDic();

string meuHash(int v[]);
bool igual(int a[],int b[]);
bool nosIguais(Node *a, Node *b);
void copiar(int a[],int b[]);

int analisaLinha (char* linha);
int obterValor();


//Movimentação necessária
pair<int,int> tr[] = {make_pair(-1, 0),
					  make_pair( 1, 0),
					  make_pair( 0, 1),
					  make_pair( 0,-1)};


int ar_i[16],z_i[2],ar_f[16],z_f[2];
vector<int> posFinal(16);
map<string,int> setx;

int main(){
    setlocale(LC_ALL, "Portuguese");
	read();
	criaDic();


	if(!temSolucao()){
		printf("Sem solução!\n");
		return 0;
	}

	//Procurando a solução para o puzzle
	int start_s=clock();
	printf("Procurando solução..\n");


	//Escolha qual algoritmo quer rodar e fazer o teste, basta descomentar para executa-lo
	bool ok = dfs(15);
	// bool ok = bfs();
	// bool ok = idfs();
	// bool ok = gulosa();
	// bool ok = A_estrela();

	if(!ok){
		//No caso de não possui solução
		printf("Sem solução\n");
	}
	else{
		// Fornece informações de tempo e memória gastos para encontrar a solução
		int stop_s=clock();
		cout << "Tempo: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
		printf("Memoria gasta: %dKB\n",obterValor());

	}
	return 0;
}

/****Classe****/
class Node{

	public:
	int array[16]; //tamango do puzzle 4x4
	int zero[2];   //posicao do zero {i, j}
	int profundidade;     //profundidade
	string caminho; //caminho percorrido após uma movimentação

	//descontructor
	~Node(){

	}

	//construtor
	Node(int array[],int zero[], int profundidade, string caminho){
		copiar(array, this->array);
		this->zero[0]=zero[0];
		this->zero[1]=zero[1];
		this->profundidade = profundidade;
		if(profundidade == 0){
			this->caminho = print(this);
		}
		else{
			this->caminho = caminho + print(this);
		}
	}


	vector<Node*> make_desc(){
		int i,j;

		vector<Node*> l;
		for(int k=0;k<4;k++){
			int c[16],t[2];
			//caminho de nós
			i=this->zero[0]+tr[k].first;
			j=this->zero[1]+tr[k].second;
			if(i>=0 && i<4 && j>=0 && j<4){

				copiar(this->array,c);
				c[this->zero[0]*4+this->zero[1]]=c[i*4+j];

				c[i*4+j]=0;
				t[0]=i;
				t[1]=j;

				//verifique se o nó já foi visto, se não foi visto, adicione
				//se for visto, mas a profundidade for menor do que a anterior, adicione também
				if(setx.find(meuHash(c))==setx.end()){
					Node *tt = new Node(c,t,this->profundidade+1,this->caminho);
					l.push_back(tt);
				}
				else if(setx[meuHash(c)]>this->profundidade+1){
					setx[meuHash(c)]=this->profundidade+1;
					Node *tt = new Node(c,t,this->profundidade+1,this->caminho);
					l.push_back(tt);
				}
			}
		}
		return l;
	}
};

//Algoritmos

//Depth first search(DFS) limited by k (Busca em profunidade com limitação)
bool dfs(int k){

	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	vector<Node*> l;
	vector<Node*>  tt;
	l.push_back(start);
	Node *t;
	int flag;
	while(l.size()>0){
		flag=0;
		t = l.back();
		l.pop_back();

		if(setx.find(meuHash(t->array))==setx.end())
			setx.insert(make_pair(meuHash(t->array),t->profundidade));

		else if(setx[meuHash(t->array)]>=t->profundidade){
			setx[meuHash(t->array)]=t->profundidade;
		}

		else{
			flag=1;
		}

		if(nosIguais(t,end)){
			cout << t->caminho;
			printf("Profundidade: %d\n",t->profundidade);
			return true;
		}
		if(t->profundidade<k && !flag){
			tt = t->make_desc();
			for(int i=0;i<(int)tt.size();i++){
				l.push_back(tt[i]);
			}
		}
		delete t;
	}
	return false;
}

//Breath first search(Busca por largura)
bool bfs(){

	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	queue<Node*> l;
	vector<Node*>  tt;
	l.push(start);
	Node *t;
	int flag;
	while(l.size()>0){
		flag = 0;
		t = l.front();
		l.pop();

		if(setx.find(meuHash(t->array))==setx.end())
			setx.insert(make_pair(meuHash(t->array),t->profundidade));

		else if(setx[meuHash(t->array)]>=t->profundidade){
			setx[meuHash(t->array)]=t->profundidade;
		}

		else{
			flag=1;
		}

		if(nosIguais(t,end)){
			cout << t->caminho;
			printf("Profundidade: %d\n",t->profundidade);
			return true;
		}
		if(t->profundidade<80 && !flag){
			tt = t->make_desc();
			for(int i=0;i<(int)tt.size();i++){
				l.push(tt[i]);
			}
		}
		delete t;
	}
	return false;
}

//Iterative Depth first search(Busca iterativa por profunidade)
bool idfs(){

	for(int i=0;i<80;i++){
		if(dfs(i)){
			return true;
		}
		setx.clear();
	}
	return false;
}

struct f{
	bool operator()(const pair<int,Node*>& a, pair<int,Node*>& b){
		return a.first>b.first;
	}
};

//Busca Gulosa Heurística Manhattan
bool gulosa(){

	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	priority_queue< pair<int,Node*>, vector <pair<int,Node*> > , f > pq;
	pq.push (make_pair(0, start));
	Node *noAtual;
	int flag;
	while (!pq.empty()) {
		flag = 0;
		noAtual = pq.top().second;
		pq.pop();

		if(setx.find(meuHash(noAtual->array))==setx.end())
			setx.insert(make_pair(meuHash(noAtual->array),noAtual->profundidade));

		if(setx[meuHash(noAtual->array)]>=noAtual->profundidade){
			setx[meuHash(noAtual->array)]=noAtual->profundidade;
		}

		else{
			flag=1;
		}

		if(nosIguais(noAtual,end)){
			cout << noAtual->caminho;
			printf("Profundidade: %d\n",noAtual->profundidade);
			return true;
		}
		if(!flag){
			vector<Node*> dsc = noAtual->make_desc();
			for (int i=0; i<(int)dsc.size();i++) {
				pq.push(make_pair(Manhattan(dsc[i]),dsc[i]));
			}
		}
		delete noAtual;
	}
	return false;
}



//Busca A* com heurística Manhattan
bool A_estrela () {
	//inicializa os nós do início e do fim
	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	//start->cost = 0;
	priority_queue< pair<int,Node*>, vector <pair<int,Node*> > , f > pq;
	pq.push (make_pair(0, start));
	Node *noAtual;
	int flag;
	while (!pq.empty()) {
		flag = 0;
		noAtual = pq.top().second;
		pq.pop();

		if(setx.find(meuHash(noAtual->array))==setx.end())
			setx.insert(make_pair(meuHash(noAtual->array),noAtual->profundidade));

		if(setx[meuHash(noAtual->array)]>=noAtual->profundidade){
			setx[meuHash(noAtual->array)]=noAtual->profundidade;
		}

		else{
			flag=1;
		}

		if(nosIguais(noAtual,end)){
			cout << noAtual->caminho;
			printf("Profundidade: %d\n", noAtual->profundidade);
			return true;
		}
		if(!flag){
			vector<Node*> dsc = noAtual->make_desc();
			for (int i=0; i<(int)dsc.size();i++) {
				pq.push(make_pair(Manhattan(dsc[i])*1.1+dsc[i]->profundidade,dsc[i]));
			}
		}
		delete noAtual;
	}
	return false;
}


//Entrada dos dados
void read(){
	int x;
	printf("Por favor, digite a configuracao inicial 4x4:\n");
	for(int i=0;i<16;i++){
		scanf("%d",&x);
		if(!x){
			z_i[0]=i/4;
			z_i[1]=i%4;
		}
		ar_i[i]=x;
	}
	printf("Por favor, digite a configuracao final 4x4:\n");
	for(int i=0;i<16;i++){
		scanf("%d",&x);
		if(!x){
			z_f[0]=i/4;
			z_f[1]=i%4;
		}
		ar_f[i]=x;
	}
}

//Saída dos dados
string print(Node *a){
	if(a==NULL){
		printf("Sem no\n");
		return "";
	}
	char s[1000];s[0]='\0';
	for(int i=0;i<16;i++){
		sprintf(s,"%s %2d ",s,a->array[i]);
		if(((i+1)%4)==0)
			sprintf(s,"%s\n",s);
	}
	sprintf(s,"%s\n",s);
	return string(s);
}

//Funções auxiliares
//verifique se a configuração pode ser resolvida
bool temSolucao(){
	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	int inv_i,inv_f,br_i,br_f;
	inv_i = contaInversoes(start->array);
	inv_f = contaInversoes(end->array);
	br_i  = meuSwap(start->zero[0]);
	br_f  = meuSwap(end->zero[0]);
	if (((inv_i % 2 == 0) == (br_i % 2 == 1)) == ((inv_f % 2 == 0) == (br_f % 2 == 1)))
		return true;
	return false;
}

//contar o número de inversões
int contaInversoes(int v[]){
	int som=0;
	for(int i=0;i<16;i++){
		for(int j=i+1;j<16;j++){
			if(v[i]>v[j] && v[i]!=0 && v[j]!=0)
				som+=1;
		}
	}
	return som;
}

//muda x, uma vez que foi contado do fundo, começando em 1, para verificar a solubilidade
int meuSwap(int x){
	switch(x){
		case 3:return 1;
		case 2:return 2;
		case 1:return 3;
		case 0:return 4;
	}
	return 0;
}

//cálculo da heurística de distância de manhattan
int Manhattan (Node *x) {
  int acc = 0;
  for (int i=0; i<16; i++) {
    if (x->array[i]!=0) {
      int index= posFinal[x->array[i]];
      int xf = index/4;
      int yf = index%4;
      int xa = i/4;
      int ya = i%4;
      acc+= abs(xf-xa) + abs(yf-ya);
    }
  }
  return acc;
}


// para melhor performance calculando as eurísticas, cria uma espécie de dicionário
// para um dado bloco k, o índice é pos final [k]
void criaDic() {
  for (int i=0; i < 16; i++) {
    int g = ar_f[i];
    posFinal[g]=i;
  }
}

//retorna o array como uma string
string meuHash(int v[]){
	string s = "";
	for(int  i =0; i<16; i++)
		s += to_string(v[i]);
	return s;
}

//verifique se 2 matrizes de configurações são iguais
bool igual(int a[], int b[]){
	for(int i = 0; i < 16; i++)
		if(a[i] != b[i]) return false;
	return true;
}

//verifica se dois nós são iguais
bool nosIguais(Node *a, Node *b){
	if(a && b)return igual(a->array, b->array);
	return false;
}

//copia o array para o array b
void copiar(int a[], int b[]){
	for(int i=0;i<16;i++){
		b[i]=a[i];
	}
}


int analisaLinha (char* linha){

    int i = strlen(linha);
    const char* p = linha;
    while (*p <'0' || *p > '9') p++;
    linha[i-3] = '\0';
    i = atoi(p);
    return i;
}

int obterValor(){
    FILE* file = fopen("/proc/self/status", "r");
    int resultado = -1;
    char linha[128];

    while (fgets(linha, 128, file) != NULL){
        if (strncmp(linha, "VmRSS:", 6) == 0){
            resultado = analisaLinha(linha);
            break;
        }
    }
    fclose(file);
    return resultado;
}
