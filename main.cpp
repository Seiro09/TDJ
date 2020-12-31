using namespace std;

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>
#include <bits/stdc++.h>

#define NB_TICS 10000
#define COUT 1.0/NB_TICS

random_device generator{};

typedef struct SOMMET{
  int valeur;
  vector<int> voisins;
  int strategie;
  double cout_sommet;
  bool tache;
}Sommet;


Sommet initialise_sommet(int valeur, int count){
  Sommet S;
  S.valeur = valeur;
  for (int i = 0; i < count; i++) {
    if (i!=valeur) S.voisins.push_back(i);
  }
  S.strategie=-1;
  S.cout_sommet=0;
  S.tache=false;
  return S;
}

void affichage_sommet(Sommet s){
  cout << "Sommet : " << s.valeur << endl;
  /* cout << "Voisins : ";
  for (int sommet : s.voisins) {
    cout << sommet << " ";
  }
  cout << endl; */
  cout << "Stratégie : " << s.strategie << endl;
  cout << "Cout : " << s.cout_sommet << endl;
  cout << endl;
}

void affichage_graphe(vector<Sommet> v){
  for (Sommet s : v) {
    affichage_sommet(s);
  }
}

void strategie_sommet(Sommet *s){
  uniform_int_distribution<int> dist(0,s->voisins.size()-1);
  s->strategie=s->voisins.at(dist(generator));
}

void choix_strategies(vector<Sommet> *graphe){
  for (Sommet &s : *graphe) {
    strategie_sommet(&s);
  }
}

void jeu(vector<Sommet> *graphe){
  int horloge=0;
  uniform_int_distribution<int> dist(0,graphe->size()-1);
  int valeur = dist(generator);
  graphe->at(valeur).tache=true;
  cout << "Le sommet initial " << valeur << " a obtenu la tache." << endl;
  cout << endl;
  while (horloge<NB_TICS) {
    horloge++;
    graphe->at(valeur).tache=false;
    valeur=graphe->at(valeur).strategie;
    graphe->at(valeur).tache=true;
    graphe->at(valeur).cout_sommet+=COUT;
  }
}

vector<int> verifier_sommet(vector<int> cycles, vector<vector<int>> *composantes, vector<Sommet> v, int s){
  vector<int> circuit;
  vector<int> composante_connexe;
  while ((find(circuit.begin(),circuit.end(),s))==circuit.end()) {
    if((find(composantes->at(0).begin(),composantes->at(0).end(),s))==composantes->at(0).end()){
      composante_connexe.push_back(s);
      circuit.push_back(s);
      s=v.at(s).strategie;
    }
    else{
      circuit.clear();
      break;
    }
  }

  if(!circuit.empty()){
    composantes->at(0).insert(composantes->at(0).end(),composante_connexe.begin(),composante_connexe.end());
    composantes->at(1).push_back(composante_connexe.size());
    circuit.erase(circuit.begin(),find(circuit.begin(),circuit.end(),s));
  }
  else{
    // chercher la valeur de la stratégie.
    auto val = find(composantes->at(0).begin(),composantes->at(0).end(),s);
    // récupere l'indice
    int index = distance(composantes->at(0).begin(), val);
    // compare l'indice avec les tailles
    int i=0;
    while (index>composantes->at(1).at(i)) { // >= si problème
      index-=composantes->at(1).at(i);
      i++;
    }
    // // je remplace la valeur
    composantes->at(0).insert(val,composante_connexe.begin(),composante_connexe.end());
    composantes->at(1).at(i)+= composante_connexe.size();
  }
  return circuit;
}

vector<vector<int>> circuits_graphe(vector<Sommet> v, vector<vector<int>> *composantes){
  vector<vector<int>> circuits;
  vector<int> cycles;
  vector<int> taille_cycles;
  circuits.push_back(cycles);
  circuits.push_back(taille_cycles);

  vector<int> composantes_connexes;
  vector<int> taille_composantes_connexes;
  composantes->push_back(composantes_connexes);
  composantes->push_back(taille_composantes_connexes);

  for(int i = 0;i<v.size();i++){
    if ((find(composantes->at(0).begin(),composantes->at(0).end(),i)==composantes->at(0).end())) {
      std::vector<int> temp;
      temp=verifier_sommet(circuits.at(0),composantes,v,i);
      circuits.at(0).insert(circuits.at(0).end(),temp.begin(),temp.end());
      if(!temp.empty()) circuits.at(1).push_back(temp.size());
    }
  }
  return circuits;
}

void affichage_circuits(vector<vector<int>> v){
  int i = 0;
  int j = 0;
  int k = 0;
  int val;

  while (i<v.at(1).size()) {
    val=v.at(1).at(i);
    cout<< "Circuit " << i+1 << " :" << endl;
    k=j+val;
    while (j<k) {
      cout << v.at(0).at(j) << " ";
      j++;
    }
    cout << endl;
    i++;
  }
  cout << endl;
}

void affichage_connexes(vector<vector<int>> v){
  int i = 0;
  int j = 0;
  int k = 0;
  int val;

  while (i<v.at(1).size()) {
    val=v.at(1).at(i);
    cout<< "Composante Connexe " << i+1 << " :" << endl;
    k=j+val;
    while (j<k) {
      cout << v.at(0).at(j) << " ";
      j++;
    }
    cout << endl;
    i++;
  }
  cout << endl;
}

void calcul_cout_sommet(int sommet, vector<Sommet> *graphe,vector<vector<int>> circuits, vector<vector<int>> composantes_connexes){
  auto val = find(composantes_connexes.at(0).begin(),composantes_connexes.at(0).end(),sommet);
  // récupere l'indice
  int index = distance(composantes_connexes.at(0).begin(), val);
  // compare l'indice avec les tailles
  int taille_composante_connexe=0;
  while (index>=composantes_connexes.at(1).at(taille_composante_connexe)) {
    index-=composantes_connexes.at(1).at(taille_composante_connexe);
    taille_composante_connexe++;
  }



  auto val2 = find(circuits.at(0).begin(),circuits.at(0).end(),sommet);
  if(val2!=circuits.at(0).end()){
  int index2 = distance(circuits.at(0).begin(), val2);
  int taille_cycle=0;
    while (index2>=circuits.at(1).at(taille_cycle)) {
      index2-=circuits.at(1).at(taille_cycle);
      taille_cycle++;
    }
    graphe->at(sommet).cout_sommet = composantes_connexes.at(1).at(taille_composante_connexe)*1.0/(graphe->size()*circuits.at(1).at(taille_cycle));
  }
  else{
    graphe->at(sommet).cout_sommet = 0;
  }
}

void calculs_couts(vector<vector<int>> circuits, vector<vector<int>> composantes_connexes, vector<Sommet> *graphe){
  for (int sommet : circuits.at(0)) {
    calcul_cout_sommet(sommet,graphe,circuits,composantes_connexes);
  }
}

void strategie_optimale(vector<Sommet> *graphe,vector<vector<int>> *circuits, vector<vector<int>> *composantes_connexes){
  vector<vector<int>> composantes_connexes_bis ;
  vector<vector<int>> circuits_bis = *circuits;
  vector<Sommet> graphe_bis = *graphe;
  bool modifie = false;
  int count = 1;
  for (int i = 0;i<circuits->at(0).size();i++) {
    if (modifie==true) {
      i=0;
      modifie=false;
    }
    int j = circuits->at(0).at(i);
    cout << "Stratégie retenue du sommet "<< j <<" : " << graphe->at(j).strategie << endl;
    cout << endl;
    for(int k=0; k<graphe_bis.at(j).voisins.size();k++){
      int voisin = graphe_bis.at(j).voisins.at(k);
      graphe_bis.at(j).strategie=voisin;
      circuits_bis = circuits_graphe(graphe_bis,&composantes_connexes_bis);
      calcul_cout_sommet(j,&graphe_bis,circuits_bis,composantes_connexes_bis);
      if (graphe_bis.at(j).cout_sommet<graphe->at(j).cout_sommet){
        *graphe=graphe_bis;
        *circuits=circuits_bis;
        *composantes_connexes=composantes_connexes_bis;
        calculs_couts(*circuits,*composantes_connexes, graphe);
        affichage_circuits(*circuits);
        affichage_connexes(*composantes_connexes);
        cout << "Stratégie retenue du sommet "<< j <<" : " << graphe->at(j).strategie << endl;

        modifie=true;
        count++;
        k = 0;
       }
      circuits_bis.clear();
      composantes_connexes_bis.clear();

    }
    circuits_bis=*circuits;
    graphe_bis=*graphe;
  }
  affichage_graphe(*graphe);
  cout << "Taille chaine d'amélioration : " << count << endl;
  cout << "Taille maximale chaine : " << ((graphe->size()*graphe->size())/4)+1 << endl;
}



int main(int argc, char const *argv[]) {
  vector<Sommet> graphe;
  vector<vector<int>> circuits;
  vector<vector<int>> composantes_connexes;
  int count;
  cout << "Nombre de sommets : ";
  cin >> count;
  for (int i = 0; i < count; i++) {
    graphe.push_back(initialise_sommet(i,count));
  }
  choix_strategies(&graphe);
  circuits = circuits_graphe(graphe, &composantes_connexes);
  affichage_circuits(circuits);
  affichage_connexes(composantes_connexes);
  calculs_couts(circuits,composantes_connexes, &graphe);
  affichage_graphe(graphe);
  strategie_optimale(&graphe, &circuits, &composantes_connexes);
  //jeu(&graphe);
  return 0;
}
