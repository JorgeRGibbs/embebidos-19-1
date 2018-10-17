#ifndef PROC_H
#define PROC_H

void procesos(float seno[]);
void procesoPadre(int pipefd[NUM_PROC][2]);
void procesoHijo(int np, int pipefd[],float seno[]);

#endif