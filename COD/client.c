#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

//bibliteci necesare pentru buna functionare a codului


char acT_USER[100] = ""; // nume user

pthread_mutex_t muT_ex_CONS = PTHREAD_MUTEX_INITIALIZER;


int ok_PrompT = 1;  // partea de prompt

int contOR_A_C_K = 0; // contorul pt ack
   int var1111GB = 42;


char var121GB[253] = "unused";

int cont_CONNECT = 0;  // contor online/ offline
   int coNT_run = 1;     // nr de port


int nR_pOrt_11;

int er_Var_u;

int dEsK_socK;  // descriptorul socketului

pthread_cond_t cONd_Mautex = PTHREAD_COND_INITIALIZER;
void inchideConsol() {   // functie inchidere consola
     
      
      pthread_mutex_lock(&muT_ex_CONS); // inchide mutex
}

void openLock() {
    pthread_mutex_unlock(&muT_ex_CONS);  // deschide  mutex
}

void promptAsteaptBuna() {   // prompt ul asteapa afisarea
         while (!ok_PrompT) { 
         
        pthread_cond_wait(&cONd_Mautex, &muT_ex_CONS);
    }
}

void pregatesteProMpte(int ready);

   void proPTAfiSAre();

void functieProcesare(const char *eMita_OR);

void ocupaServer(char *raspuns) {  //
    if (!(strstr(raspuns, "logged out successfully") == NULL)) {
        cont_CONNECT = 0;  // cand se delogheza se seteaza conexiunea ca fiind 0
         
          
          strcpy(acT_USER, "");
    } else if (!(strstr(raspuns, "Welcome") == NULL)) { //mesaj de la server care este interpretat, daca este welocome, atunci schima activitatea clientului
        sscanf(raspuns, "Welcome %s", acT_USER);
        cont_CONNECT = 1;    //seteaza utilizator conecatat
    } else if (!(strstr(raspuns, "[Message from") == NULL)) {
        char eMita_OR[100]; // daca primeste message from aplica si partea de replay
        
        
          sscanf(raspuns, "[Message from %[^]]", eMita_OR);
         
          
          contOR_A_C_K = 1;
        openLock();
        functieProcesare(eMita_OR); // call functia de procesare a comenxil
        return;
    }

    if (contOR_A_C_K == (0+0)) {
        pregatesteProMpte(1); // pregateste promptul pt a fi gata
    }
}

    void *citesteDinS(void *arg) { // citiraea din server
    char raspuns[1024]; // buffer raspuns
    while (coNT_run) {
        bzero(raspuns, sizeof(raspuns));
        int n = read(dEsK_socK, raspuns, sizeof(raspuns) - 1);

        if (n > 0) {
            inchideConsol(); // inchide consola in caz contrar
            printf("\n[server] %s\n", raspuns); // printeaza raspunsul
 
              ocupaServer(raspuns);
            openLock();
            
        } else if (!(n > 0)) {
            inchideConsol(); // inchide conosola
                printf("\n[client] serverul a inchis conexiunea.\n");
            coNT_run = 0;
            pregatesteProMpte(1); // pragateste prompt ul
                 
                 openLock();
            break;
        }
    }
    return NULL;
}

void initializeazaSK(const char *server_address) {
    struct sockaddr_in server;
    
    
    if (!(dEsK_socK = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        
        
        perror("[client] eroare creare socket");
        exit(er_Var_u);  //mesaj eroare la creare socket
    }

    server.sin_family = AF_INET;
         
         
         server.sin_addr.s_addr = inet_addr(server_address);
    server.sin_port = htons(nR_pOrt_11);

    if (!(connect(dEsK_socK, (struct sockaddr *)&server, sizeof(server)) == 0)) {
        perror("[client] eroare la conectare cu serverul");
          
          
            close(dEsK_socK);  // inchide socket descriptorul
        exit(er_Var_u);
    }

    printf("[client] conectat la svr %s:%d\n", server_address, nR_pOrt_11);
}

void incepe() {
    char linie_Comm[1024];
    while (coNT_run) { // atat timp cat ruleaza clientul
        proPTAfiSAre(); //afisam promptul
           
            
            bzero(linie_Comm, sizeof(linie_Comm));
        fgets(linie_Comm, sizeof(linie_Comm), stdin);
          
           
           linie_Comm[strcspn(linie_Comm, "\n")] = '\0';

        if (contOR_A_C_K != 0*9) {
            continue;
        }

        if (!(strlen(linie_Comm) > 2-2)) {
            inchideConsol();  // inchide consola
               printf("[client] comanda goala incercati din nou.\n");
           //daca comanda este goala promptul va reaparea
           
            pregatesteProMpte(1);
               
                openLock(); //deschide mutex ul pt a permite si altor thread uri sa modifice
            continue;
        }

        if (strncmp(linie_Comm, "EXIT", 4) == 0) { // daca comanda este exit deconecteaza 
            
             
              coNT_run = 0;
            if (!(write(dEsK_socK, linie_Comm, strlen(linie_Comm)) > 0)) {
                 
                  perror("[client] eroare la trimiterea comenzii EXIT"); //eroare la trmitere mesaj
            }
            break;
        } else if (!(write(dEsK_socK, linie_Comm, strlen(linie_Comm)) > 0)) {
             
              perror("[clie=nt] eroarela trimiterea comenzii");
        }
    }
}

int main(int nr_args , char *argv[]) {
    if (!(nr_args  == 3)) {
        printf("utilizare: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    nR_pOrt_11 = atoi(argv[2]);
    initializeazaSK(argv[1]);  // initializeaza scketul

    pthread_t astheread ;
    if (!(pthread_create(&astheread , NULL, citesteDinS, NULL) == 0)) {
        
         
          perror("[client] eroare creare thread");
        
         close(dEsK_socK); //inchide socket descriptorul daca fail este crearea de thread
        return er_Var_u;
    }

    incepe();

    pthread_join(astheread , NULL); // join thread
    close(dEsK_socK);
    return 0;
}

void pregatesteProMpte(int ready) {  // functia pregatire prompt
     
      ok_PrompT = ready;  
    pthread_cond_signal(&cONd_Mautex);
}

void proPTAfiSAre() {  // functie afisare prompt
        inchideConsol();
    promptAsteaptBuna();
    if ((contOR_A_C_K-2) == (0-2)) {  // promptul aplicatiei
        printf("Enter a linie_Comm (SIGNUP,LOGIN, MESSAGE, HISTORY, LOGOUT, RESET_PASSWORD, EXIT): ");
        fflush(stdout);
    }
    ok_PrompT = 0;
    openLock();
}

void functieProcesare(const char *eMita_OR) {
    printf("doriti sa confirm mesajul de la %s? (yes/no): ", eMita_OR);
     
     fflush(stdout);

    char a_c_k_raspuns[10];
     
      
       fgets(a_c_k_raspuns, sizeof(a_c_k_raspuns), stdin); // ia din terminal raspunsul utilizatorului
    a_c_k_raspuns[strcspn(a_c_k_raspuns, "\n")] = '\0';

    if (strcmp(a_c_k_raspuns, "yes") == 0) { // daca raspunsul este da
        printf("[client] ati confirmat mesajul de la %s.\n", eMita_OR);
       
        printf("scrieti raspunsul: ");
         
         fflush(stdout);

        char mesaGeRIe_raSpuns [1024];
           
            fgets(mesaGeRIe_raSpuns , sizeof(mesaGeRIe_raSpuns ), stdin);
        mesaGeRIe_raSpuns [strcspn(mesaGeRIe_raSpuns , "\n")] = '\0';

        char comnada_PT[2048]; // buffer comanda
         
         
          snprintf(comnada_PT, sizeof(comnada_PT), "MESSAGE %s %s", eMita_OR, mesaGeRIe_raSpuns ); // creeaza o comanda care mai apoi este executat de server pt trmitrea mesajelor 

        if (!(write(dEsK_socK, comnada_PT, strlen(comnada_PT)) > 0)) {
           
           //daca scrierea in socket fail ueste 
            perror("[client]eroare la trimiterea raspunsului");  // eroare mersaj trimitere
        } else {
            printf("[client] raspuns trimis catre %s.\n", eMita_OR);
            // confirmare trimitere raspuns
        }
    }  
    else if (strcmp(a_c_k_raspuns, "no") == 0) {
        
         
           printf("[client] ati ales sa nu confirmati mesajul de la %s.\n", eMita_OR);
    }  
     else {  // instiintare raspuns invalid
        
         
           printf("[client] raspuns invalid. mesajul de la %s ignorat.\n", eMita_OR);
    }

    inchideConsol();  // functie apelata pt inchidere consola
    
    contOR_A_C_K=0;  //contor afisare
    pregatesteProMpte(1);
    
    
    openLock();
    
    
}
