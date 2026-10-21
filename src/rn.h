#ifndef RN_H
#define RN_H

typedef enum { Vermelho, Preto } Cor;

typedef struct RnNo {
  struct RnNo *pai;
  struct RnNo *esquerda;
  struct RnNo *direita;
  int valor;
  Cor cor;
} RnNo;

typedef struct {
  RnNo *raiz;
  RnNo *nulo;
} Rn;

Rn *rn_criar(void);
void rn_adicionar(Rn *arvore, int valor);
void rn_destruir(Rn *arvore);

#endif
