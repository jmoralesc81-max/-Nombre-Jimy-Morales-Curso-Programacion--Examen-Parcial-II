#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

struct NodoDoble {
    int dato;
    string tipo;
    NodoDoble* siguiente;
    NodoDoble* anterior;
};

struct NodoBST {
    int dato;
    NodoBST* izq;
    NodoBST* der;
};

void liberarBST(NodoBST*& r);
void liberarLista(Nodo*& l);
void liberarHistorial(NodoDoble*& h);

Nodo* pila = NULL;
Nodo* cola = NULL;
Nodo* finCola = NULL;
Nodo* negativos = NULL;

NodoDoble* historial = NULL;
NodoDoble* finHistorial = NULL;

NodoBST* raiz = NULL;

int pares=0, impares=0, neg=0, ceros=0;
int mayor=-99999, menor=99999, total=0;

void push(int v){
    Nodo* n = new Nodo{v,pila};
    pila = n;
}

void enqueue(int v){
    Nodo* n = new Nodo{v,NULL};
    if(!cola){
        cola = finCola = n;
    }else{
        finCola->siguiente = n;
        finCola = n;
    }
}

void insertarNeg(int v){
    Nodo* n = new Nodo{v,negativos};
    negativos = n;
}

void insertarHistorial(int v, string tipo){
    NodoDoble* n = new NodoDoble{v,tipo,NULL,finHistorial};

    if(!historial)
        historial = n;
    else
        finHistorial->siguiente = n;

    finHistorial = n;
}

NodoBST* insertarBST(NodoBST* r, int v){
    if(!r) return new NodoBST{v,NULL,NULL};

    if(v < r->dato)
        r->izq = insertarBST(r->izq,v);
    else
        r->der = insertarBST(r->der,v);

    return r;
}

void inOrden(NodoBST* r){
    if(r){
        inOrden(r->izq);
        cout<<r->dato<<" ";
        inOrden(r->der);
    }
}

void preOrden(NodoBST* r){
    if(r){
        cout<<r->dato<<" ";
        preOrden(r->izq);
        preOrden(r->der);
    }
}

void postOrden(NodoBST* r){
    if(r){
        postOrden(r->izq);
        postOrden(r->der);
        cout<<r->dato<<" ";
    }
}

bool buscarBST(NodoBST* r,int v){
    if(!r) return false;
    if(r->dato==v) return true;
    if(v<r->dato) return buscarBST(r->izq,v);
    return buscarBST(r->der,v);
}

void mostrar(Nodo* l){
    if(!l){
        cout<<"Estructura vacia\n";
        return;
    }

    while(l){
        cout<<l->dato<<" -> ";
        l=l->siguiente;
    }
    cout<<"NULL\n";
}

void mostrarHistorial(){
    if(!historial){
        cout<<"Historial vacio\n";
        return;
    }

    NodoDoble* t=historial;
    while(t){
        cout<<t->dato<<" ("<<t->tipo<<") -> ";
        t=t->siguiente;
    }
    cout<<"NULL\n";
}

void ingresar(){
    int n,v;
    cout<<"Cantidad: ";
    cin>>n;

    if(cin.fail() || n<=0){
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Entrada invalida\n";
        return;
    }

    for(int i=0;i<n;i++){
        cout<<"Numero: ";
        cin>>v;

        if(cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Valor invalido\n";
            i--;
            continue;
        }

        if(v==0){
            ceros++;
        }
        else if(v>0 && v%2==0){
            enqueue(v);
            insertarHistorial(v,"COLA");
            pares++;
        }
        else if(v>0){
            push(v);
            insertarHistorial(v,"PILA");
            impares++;
        }
        else{
            insertarNeg(v);
            insertarHistorial(v,"NEGATIVO");
            neg++;
        }

        if(v!=0){
            total++;
            if(v>mayor) mayor=v;
            if(v<menor) menor=v;
        }
    }
}

void eliminarNegativos(){
    Nodo* t=negativos;
    Nodo* ant=NULL;
    int elim=0;

    while(t){
        if(abs(t->dato)<10){
            if(!ant)
                negativos=t->siguiente;
            else
                ant->siguiente=t->siguiente;

            Nodo* aux=t;
            t=t->siguiente;
            delete aux;
            elim++;
        }else{
            ant=t;
            t=t->siguiente;
        }
    }

    cout<<"Eliminados: "<<elim<<endl;
}

void construirBST(){
    liberarBST(raiz);

    Nodo* t=pila;
    while(t){
        raiz=insertarBST(raiz,t->dato);
        t=t->siguiente;
    }

    t=cola;
    while(t){
        raiz=insertarBST(raiz,t->dato);
        t=t->siguiente;
    }

    cout<<"BST construido\n";
}

void stats(){
    cout<<"Pares: "<<pares<<endl;
    cout<<"Impares: "<<impares<<endl;
    cout<<"Negativos: "<<neg<<endl;
    cout<<"Ceros: "<<ceros<<endl;
    cout<<"Total validos: "<<total<<endl;
    cout<<"Mayor: "<<mayor<<endl;
    cout<<"Menor: "<<menor<<endl;
}

void reorganizar(){
    Nodo* lista1=NULL;
    Nodo* lista2=NULL;

    while(pila){
        Nodo* t=pila;
        pila=pila->siguiente;
        t->siguiente=lista1;
        lista1=t;
    }

    while(cola){
        Nodo* t=cola;
        cola=cola->siguiente;
        t->siguiente=lista2;
        lista2=t;
    }

    cout<<"Lista desde pila: ";
    mostrar(lista1);

    cout<<"Lista desde cola: ";
    mostrar(lista2);

    liberarBST(raiz);

    Nodo* t=lista1;
    while(t){
        raiz=insertarBST(raiz,t->dato);
        t=t->siguiente;
    }

    t=lista2;
    while(t){
        raiz=insertarBST(raiz,t->dato);
        t=t->siguiente;
    }

    cout<<"BST reconstruido\n";
}

void liberarLista(Nodo*& l){
    while(l){
        Nodo* t=l;
        l=l->siguiente;
        delete t;
    }
}

void liberarHistorial(NodoDoble*& h){
    while(h){
        NodoDoble* t=h;
        h=h->siguiente;
        delete t;
    }
}

void liberarBST(NodoBST*& r){
    if(r){
        liberarBST(r->izq);
        liberarBST(r->der);
        delete r;
        r=NULL;
    }
}

int main(){
    int op,v;

    do{
        cout<<"\n===== MENU =====\n";
        cout<<"Nombre: Jimy Aroldo Morales Carranza\n";
        cout<<"1. Ingresar\n2. Pila\n3. Cola\n4. Negativos\n5. Historial\n6. BST\n7. Eliminar negativos\n8. Buscar BST\n9. Estadisticas\n10. Reorganizar\n0. Salir\n";
        cout<<"Opcion: ";
        cin>>op;

        if(cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Entrada invalida\n";
            continue;
        }

        switch(op){
            case 1: ingresar(); break;
            case 2: mostrar(pila); break;
            case 3: mostrar(cola); break;
            case 4: mostrar(negativos); break;
            case 5: mostrarHistorial(); break;
            case 6:
                construirBST();
                cout<<"InOrden: "; inOrden(raiz);
                cout<<"\nPreOrden: "; preOrden(raiz);
                cout<<"\nPostOrden: "; postOrden(raiz);
                cout<<endl;
                break;
            case 7: eliminarNegativos(); break;
            case 8:
                cout<<"Buscar: ";
                cin>>v;
                cout<<(buscarBST(raiz,v)?"Existe\n":"No existe\n");
                break;
            case 9: stats(); break;
            case 10: reorganizar(); break;
        }

    }while(op!=0);

    liberarLista(pila);
    liberarLista(cola);
    liberarLista(negativos);
    liberarHistorial(historial);
    liberarBST(raiz);

    return 0;
}