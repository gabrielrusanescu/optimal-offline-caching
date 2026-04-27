/*
Grupa: 161
Problema: 4 / Optimal Offline Caching (Greedy)
Nume: Rusănescu Gabriel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define MAXSTRING 75

int gaseste_urmatoarea_aparitie(char **cereri, int m, int pozstart, char *numeresursa){
    for(int i=pozstart; i<m; i++){
        if (strcmp(cereri[i], numeresursa)==0){
            return i;
        }
    }
    return INT_MAX; 
}

int rezolva_cache(const char *nume_fisier){
    FILE *fin=fopen(nume_fisier, "r");
    if(!fin) return -1;
    int n, k, m;
    if(fscanf(fin, "%d", &n)!=1){ fclose(fin); return -1; }
    char **resurse=(char **)malloc(n * sizeof(char *));
    for(int i=0; i<n; i++){
        resurse[i]=(char *)malloc(MAXSTRING * sizeof(char));
        fscanf(fin, "%s", resurse[i]);
    }
    if(fscanf(fin, "%d", &k)!=1){ fclose(fin); return -1; }
    char **cache=(char **)malloc(k * sizeof(char *));
    for(int i=0; i<k; i++) {
        cache[i]=(char *)malloc(MAXSTRING * sizeof(char));
        fscanf(fin, "%s", cache[i]);
    }
    if(fscanf(fin, "%d", &m)!=1){ fclose(fin); return -1; }
    char **cereri=(char **)malloc(m * sizeof(char *));
    for(int i=0; i<m; i++){
        cereri[i]=(char *)malloc(MAXSTRING * sizeof(char));
        fscanf(fin, "%s", cereri[i]);
    }
    int cachemisses=0;
    printf("Se ruleaza pentru fisierul %s\n", nume_fisier);
    for(int i=0; i<m; i++){
        int gasit=-1;
        for(int j=0; j<k; j++){
            if(strcmp(cache[j], cereri[i])==0){
                gasit=j;
                break;
            }
        }

        if(gasit==-1){
            cachemisses++;
            int index_de_eliminat=0;
            int maxnextpoz=-1;
            for(int j=0; j<k; j++){
                int nextpoz=gaseste_urmatoarea_aparitie(cereri, m, i+1, cache[j]);
                if(nextpoz>maxnextpoz){
                    maxnextpoz=nextpoz;
                    index_de_eliminat=j;
                }
            }
            strcpy(cache[index_de_eliminat], cereri[i]);
            printf("Pasul %d, cerere %s = miss (eliminat din cache pentru a face loc)\n", i+1, cereri[i]);
        } else {
            printf("Pasul %d, cerere %s = hit\n", i+1, cereri[i]);
        }
        printf("Cache actual: ");
        for(int j=0; j<k; j++) printf("%s ", cache[j]);
        printf("\n");
    }

    printf("Total Cache Misses: %d\n\n", cachemisses);
    for(int i=0; i<n; i++) free(resurse[i]); free(resurse);
    for(int i=0; i<k; i++) free(cache[i]); free(cache);
    for(int i=0; i<m; i++) free(cereri[i]); free(cereri);
    fclose(fin);
    return cachemisses;
}

int main() {
    assert(rezolva_cache("test1.txt")==2);
    assert(rezolva_cache("test2.txt")==5);
    assert(rezolva_cache("test3.txt")==4);
    printf("\nToate verificarile cu assert au fost cu succes. \n");
    return 0;
}
