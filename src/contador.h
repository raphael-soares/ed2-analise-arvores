#ifndef CONTADOR_H
#define CONTADOR_H

typedef struct {
  unsigned long long busca;
  unsigned long long ajuste;
} Esforco;

extern Esforco esforco;

void esforco_zerar(void);

#endif
