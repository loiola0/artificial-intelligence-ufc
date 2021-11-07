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
#include <unistd.h>

using namespace std;


class Node;



bool idfs();
bool greedy_misplaced();
bool A_star_Manhattan();
bool A_star_misplaced();
bool ldfs(int k);
bool greedy_Manhattan();
bool bfs();

void read();


string print(Node *a);

bool   hassolution();
int    checkinv(int v[]);
int    myswap(int x);
int    Manhattan (Node *x);
int    misplaced (Node *x);

// para melhor desempenho em heurísticas
void   creat_dicf(); 

string myhash(int v[]);
bool   eq(int a[],int b[]);
bool   nodeEq(Node *a, Node *b);
void   cp(int a[],int b[]);


int parseLine(char* line);
int getValue();


//Movimentação necessária
pair<int,int> tr[] = {make_pair(-1, 0),
					  make_pair( 1, 0),
					  make_pair( 0, 1),
					  make_pair( 0,-1)};


int ar_i[16],z_i[2],ar_f[16],z_f[2];
vector<int> finalpos(16);
map<string,int> setx;

int main(){
	
	read();
	creat_dicf();
	
	
	if(!hassolution()){
		printf("Sem solução!\n");
		return 0;
	}
	
	//Procurando a solução para o puzzle
	int start_s=clock();
	printf("Procurando solução..\n");
	
	
	//Escolha qual algoritmo quer rodar e fazer o teste, basta descomentar para executa-lo
	// bool ok = ldfs(15);
	// bool ok = bfs();
	// bool ok = idfs();
	// bool ok = greedy_Manhattan();
	// bool ok = A_star_Manhattan();	
	bool ok = A_star_Manhattan();
	
	if(!ok){
		//No caso de não possui solução
		printf("No solution\n");
	}
	else{
		// Fornece informações de tempo e memória gastos para encontrar a solução
		int stop_s=clock();
		cout << "Tempo: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
		printf("Memoria gasta: %dKB\n",getValue());
		
	}
	return 0;
}

/****Class****/
class Node{
	
	public:
	int array[16]; //tamango do puzzle 4x4
	int zero[2];   //position of zero {i, j}
	int depth;     //profundidade
	string path;   
	
	//descontructor
	~Node(){
		
	}
	
	//constructor
	Node(int array[],int zero[], int depth, string path){
		cp(array,this->array);
		this->zero[0]=zero[0];
		this->zero[1]=zero[1];
		this->depth = depth;
		if(depth == 0){
			this->path = print(this);
		}
		else{
			this->path = path + print(this);
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
				
				cp(this->array,c);
				c[this->zero[0]*4+this->zero[1]]=c[i*4+j];
				
				c[i*4+j]=0;
				t[0]=i;
				t[1]=j;
				
				//verifique se o nó já foi visto, se não foi visto, adicione
				//se for visto, mas a profundidade for menor do que a anterior, adicione também
				if(setx.find(myhash(c))==setx.end()){
					Node *tt = new Node(c,t,this->depth+1,this->path);
					l.push_back(tt);
				}
				else if(setx[myhash(c)]>this->depth+1){
					setx[myhash(c)]=this->depth+1;
					Node *tt = new Node(c,t,this->depth+1,this->path);
					l.push_back(tt);
				}
			}
		}
		return l;
	}
};

//Algoritmos

//Depth first search(DFS) limited by k (Busca em profunidade com limitação)
bool ldfs(int k){
	
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
		
		if(setx.find(myhash(t->array))==setx.end())
			setx.insert(make_pair(myhash(t->array),t->depth));
		
		else if(setx[myhash(t->array)]>=t->depth){
			setx[myhash(t->array)]=t->depth;
		}
		
		else{
			flag=1;
		}
		
		if(nodeEq(t,end)){
			cout << t->path;
			printf("Depth: %d\n",t->depth);
			return true;
		}
		if(t->depth<k && !flag){
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
		
		if(setx.find(myhash(t->array))==setx.end())
			setx.insert(make_pair(myhash(t->array),t->depth));
		
		else if(setx[myhash(t->array)]>=t->depth){
			setx[myhash(t->array)]=t->depth;
		}
		
		else{
			flag=1;
		}
		
		if(nodeEq(t,end)){
			cout << t->path;
			printf("Depth: %d\n",t->depth);
			return true;
		}
		if(t->depth<80 && !flag){
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
		if(ldfs(i)){
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
bool greedy_Manhattan(){
	
	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	priority_queue< pair<int,Node*>, vector <pair<int,Node*> > , f > pq;
	pq.push (make_pair(0,start));
	Node *current_node;
	int flag;
	while (!pq.empty()) {
		flag = 0;
		current_node = pq.top().second;
		pq.pop();
		
		if(setx.find(myhash(current_node->array))==setx.end())
			setx.insert(make_pair(myhash(current_node->array),current_node->depth));
		
		if(setx[myhash(current_node->array)]>=current_node->depth){
			setx[myhash(current_node->array)]=current_node->depth;
		}
		
		else{
			flag=1;
		}
		
		if(nodeEq(current_node,end)){
			cout << current_node->path;
			printf("Depth: %d\n",current_node->depth);
			return true;
		}
		if(!flag){
			vector<Node*> dsc = current_node->make_desc();
			for (int i=0; i<(int)dsc.size();i++) {
				pq.push(make_pair(Manhattan(dsc[i]),dsc[i]));
			}
		}
		delete current_node;
	}
	return false;
}



//Busca gulosa com heurística Manhattan
bool A_star_Manhattan () {
	//initialize nodes start and end
	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	//start->cost = 0;
	priority_queue< pair<int,Node*>, vector <pair<int,Node*> > , f > pq;
	pq.push (make_pair(0,start));
	Node *current_node;
	int flag;
	while (!pq.empty()) {
		flag = 0;
		current_node = pq.top().second;
		pq.pop();
		
		if(setx.find(myhash(current_node->array))==setx.end())
			setx.insert(make_pair(myhash(current_node->array),current_node->depth));
		
		if(setx[myhash(current_node->array)]>=current_node->depth){
			setx[myhash(current_node->array)]=current_node->depth;
		}
		
		else{
			flag=1;
		}
		
		if(nodeEq(current_node,end)){
			cout << current_node->path;
			printf("Depth: %d\n",current_node->depth);
			return true;
		}
		if(!flag){
			vector<Node*> dsc = current_node->make_desc();
			for (int i=0; i<(int)dsc.size();i++) {
				pq.push(make_pair(Manhattan(dsc[i])*1.1+dsc[i]->depth,dsc[i]));
			}
		}
		delete current_node;
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
bool hassolution(){
	Node *end = new Node(ar_f,z_f,0,"");
	Node *start = new Node(ar_i,z_i,0,"");
	int inv_i,inv_f,br_i,br_f;
	inv_i = checkinv(start->array);
	inv_f = checkinv(end->array);
	br_i  = myswap(start->zero[0]);
	br_f  = myswap(end->zero[0]);
	if (((inv_i % 2 == 0) == (br_i % 2 == 1)) == ((inv_f % 2 == 0) == (br_f % 2 == 1)))
		return true;
	return false; 
}

//contar o número de inversões
int checkinv(int v[]){
	int sum=0;
	for(int i=0;i<16;i++){
		for(int j=i+1;j<16;j++){
			if(v[i]>v[j] && v[i]!=0 && v[j]!=0)
				sum+=1;
		}
	}
	return sum;
}

//muda x, uma vez que foi contado do fundo, começando em 1, para verificar a solubilidade
int myswap(int x){
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
      int index= finalpos[x->array[i]];
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
void creat_dicf() {
  for (int i=0; i<16;i++) {
    int g = ar_f[i];
    finalpos[g]=i;
  }
}

//retorna o array como uma string
string myhash(int v[]){
	string s = "";
	for(int  i =0;i<16;i++)
		s+=to_string(v[i]);
	return s;
}

//verifique se 2 matrizes de configurações são iguais
bool eq(int a[], int b[]){
	for(int i=0;i<16;i++)
		if(a[i]!=b[i]) return false;
	return true;
}

//verifica se dois nós são iguais
bool nodeEq(Node *a, Node *b){
	if(a && b)return eq(a->array,b->array);
	return false;
}

//copia o array para o array b
void cp(int a[], int b[]){
	for(int i=0;i<16;i++){
		b[i]=a[i];
	}
}


int parseLine(char* line){
    
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}
 
int getValue(){
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}