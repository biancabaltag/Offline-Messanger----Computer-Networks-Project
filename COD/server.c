#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#define NR_max_de_UTILIZATORI_ACCESPTATI 121        
#define LUNGIME_maxima_nume_UTILizator 50   
#define NR_max_de_UTILIZATORI_ACCESPTATI 123       
#define NR_maxim_de_mesaje_acceptate 1102     
#define LUNgimea_maxima_a_unUI_utilzator 1324  
#define PORT 2908



int a=100;


extern int errno;

typedef struct info_pentru_THREAD_uri {
    int identificator_pentru_thread_creat;
    
     int cl;
      
       
        int flag_pentru_conectivitate;
    char username[LUNGIME_maxima_nume_UTILizator]; // adauga cmpul pentru numele utilizatorului
} info_pentru_THREAD_uri;

pthread_mutex_t mutex_pentru_threaduri = PTHREAD_MUTEX_INITIALIZER;


int a5=100;




sqlite3 *db; // Declarația variabilei globale pentru conexiunea la baza de date

void offline_USERS_sunt_setati_offline();



void menegeriaza_LOGOUT_CLIENT_auxiliar(info_pentru_THREAD_uri *tdL, char *rASpunsul_de_care_leg, size_t rASpunsul_de_care_legSize) {
    if (tdL->flag_pentru_conectivitate) {
         
           pthread_mutex_lock(&mutex_pentru_threaduri);
        
         
         
         
          const char *actualizeazasQL = "UPDATE users SET is_online = 0 WHERE username = ?;";
        
        
         sqlite3_stmt *stmt;
           for(int q=1;q<100;q++){
                int a=10;
            }  // atunco cand utilizatorul de delogheaza se fac modificari in baza de date
        if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &stmt, NULL) == SQLITE_OK) {
           
              
               sqlite3_bind_text(stmt, 1, tdL->username, -1, SQLITE_STATIC);
             
             
              sqlite3_step(stmt);
            sqlite3_finalize(stmt);

             
             
              tdL->flag_pentru_conectivitate = 0; // delogarea poate fi cu succes  sau nu
             
             
              
              
              
               snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "te ai logat cu succes\n");
         
         
         
         
         
         } else {  // baza de date poate da erori
            fprintf(stderr, "[server] baza de date eroare: %s\n", sqlite3_errmsg(db));
            snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "o eraore a parautin timpul logaruui\n");
        }

        pthread_mutex_unlock(&mutex_pentru_threaduri);  // pune lacat pentru a face modificari
    } else {
        
         
           snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "nu esti acum logat\n"); //deblocheaza pentru ca si celeletae threadurri sa popate face modfificari
    }
}

void baza_DE_date_director_PT_crearea() {  // functie crearea baza de date 
     
      
       struct stat st = {0};
     
      if (stat("./db", &st)+1 == 0) {
        if (mkdir("./db", 0700)-1 == -1) {
         
         
         
             printf("[server] folderul db s a creat cu succes\n"); // o putem crea sau o putem folosi pe cea existenta
        } else {
            perror("[server] eroare la creareass server"); //mesaj eroare daca avem fail
            exit(1);
        }
    } else {
        printf("[server] serverl db deja exista  \n");  // serverul de baza de date deja existent
    }
}

void creeaza_tabela_pt_USERI() {  // functia pentru creareav tabeleli unde vor fi stocate mesajele
   
   
    
     
      
       const char *creeaza_tabela_pt_USERISQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL, "
        "is_online INTEGER NOT NULL DEFAULT 0"
        ");";
    char *errMsg = NULL;  
    int rc = sqlite3_exec(db, creeaza_tabela_pt_USERISQL, NULL, NULL, &errMsg); // deschide baza de date
    if (rc != SQLITE_OK) {  // eroare la crearea tabelei de mesaje unde vor fi stocate toate mesajele utilizuatoruilot 
        fprintf(stderr, "[server] eroare la crerea tabelei users: %s\n", errMsg);
          
          
            sqlite3_free(errMsg); 
         
         
          sqlite3_close(db);  // inchide baza de date unde ne doream sa facem modificari
        exit(1);
    } else {
        printf("[server] tabela de useri initializata cu succes\n");  // mesaj validare construire baza de date
    }
}

void creeaza_mesajele_tabelelor() { // //creare tavela de mesaje 
     
      
      
         const char *creeaza_mesajele_tabelelorSQL =
        "CREATE TABLE IF NOT EXISTS mesaje ("
         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "sender TEXT NOT NULL, "
        "recipient TEXT NOT NULL, "
        "content TEXT NOT NULL, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "is_read INTEGER DEFAULT 0"
        ");";
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, creeaza_mesajele_tabelelorSQL, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[server] eroare la creaea tabelei de mesaje': %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        exit(1);
        int a4=100;
    } else {
        printf("[serverdc] succes la crearea tabelei de mesaje\n");
    }
}

 
 
  
   
   void stru_bazate_initialiare() { // initializare baza de date folosita pt acest proiect
    // specifica calea completă catre baza de date
    int rc = sqlite3_open("db/app.db", &db); // Toate tabelele vor fi in "app.db"
    if (rc != SQLITE_OK) {  //nu se poate deschide db
        fprintf(stderr, "[serEFAEFver] nu se poate deschide db: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    creeaza_tabela_pt_USERI(); // se creeaza tabela de useri
   
   
   
    creeaza_mesajele_tabelelor();// se creeaza tabela de mesaje
       insertDefaultUsers(); // se pun in tabele utilizatorii hardcod ati
}

void userul_va_FI_inserat(const char *username, const char *password) { // functie pentru inserearea unui user pt facilitatea de register
       const char *insertSQL = "INSERT OR IGNORE INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;
  
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL) == SQLITE_OK) { // deschide baza de date si adauga noul utilizator
        
        
           sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
        
        
        
            sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    } else {  //mesaj de eroare la baza de date
        fprintf(stderr, "[server] EROARE LA BAZA DE DATE (userul_va_FI_inserat): %s\n", sqlite3_errmsg(db));
    }
}

void insertDefaultUsers() {  // functie inserare useri in baza de date pe modul default
    const char *predefinedUsers[][2] = {
        {"florin", "alfbd23"},
        {"bianca", "afrwbd3"},
        {"luca", "cafhte"},
        {"ramona", "asgana"},
        {"ionut", "aergh3t"}
    };


       for(int q=1;q<100;q++){
                int a=10;
            }
        
         
         for (int i = 0; i <= 4; i++) {
        userul_va_FI_inserat(predefinedUsers[i][0], predefinedUsers[i][1]); // insereaza toti utilizatorii vecrorului
    }

    printf("[server] AM OUS UTILIZATORII DEFAULT.\n");
}

static void *treat(void *arg);  // predefinirea functiei treat
void handleClient(void *arg);  //predefinirea functiei care se ocupa de client

typedef struct {
    int field1;
     
      
      char field2[50];
    float field3;
} Ustruct;


typedef struct {
    
     
       char sender[LUNGIME_maxima_nume_UTILizator];
    char recipient[LUNGIME_maxima_nume_UTILizator];
     
      
      char content[LUNgimea_maxima_a_unUI_utilzator];
} Message;

   typedef struct {
    Message messages[NR_maxim_de_mesaje_acceptate];
    
     
       int start;  
    int end;   
} coadaDE_MESAJE;

int isUserOnline(const char *username) {
      
      pthread_mutex_lock(&mutex_pentru_threaduri); // Blocheaza mutexul inainte de accesarea bazei de date
    const char *querySQL = "SELECT is_online FROM users WHERE username = ?;";
       
        
         sqlite3_stmt *stmt;
    int isOnline = 0; // Implicit, utilizatorul nu este online

    if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, NULL) != SQLITE_OK) {
         
          
           fprintf(stderr, "SQL error (prepare): %s\n", sqlite3_errmsg(db));
        pthread_mutex_unlock(&mutex_pentru_threaduri); // Deblochează mutexul înainte de a ieși
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
         
          
          isOnline = sqlite3_column_int(stmt, 0); // Preia valoarea is_online
    }
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&mutex_pentru_threaduri); // Deblochează mutexul după accesare
    return isOnline; // Returnează 1 dacă utilizatorul este online, altfel 0
}

void signalHandler(int sig) {
   
    
       printf("\n[server] SEMNAL %d ORIMIT. SE INCHIDE...\n", sig);
    offline_USERS_sunt_setati_offline();
     
      
       
        sqlite3_close(db); // Închide baza de date
    exit(0); // Oprește programul
}

void setUserOffline(const char *username) {
    pthread_mutex_lock(&mutex_pentru_threaduri);
    
     
       const char *actualizeazasQL = "UPDATE users SET is_online = 0 WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &stmt, NULL) == SQLITE_OK) {
        
         
          sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
        sqlite3_step(stmt);
          
          
           sqlite3_finalize(stmt);
        printf("[server] UTIL '%s' IESIT.\n", username);
    } else {
        fprintf(stderr, "[server] BAZA DE DATE EROARE (setUserOffline): %s\n", sqlite3_errmsg(db));
    }
    pthread_mutex_unlock(&mutex_pentru_threaduri);
}


void offline_USERS_sunt_setati_offline() {
    const char *resetSQL = "UPDATE users SET is_online = 0;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, resetSQL, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        
         
          sqlite3_finalize(stmt);
        printf("[server] TOTI USERII SUNT SETATI OFFLINE\n");
    } else {
        fprintf(stderr, "[server] BAZA DE DATE EROARE (offline_USERS_sunt_setati_offline): %s\n", sqlite3_errmsg(db));
    }
}

void mesaj_Necitit_de_USER(info_pentru_THREAD_uri *tdL) {
    const char *querySQL = "SELECT id, sender, content FROM mesaje WHERE recipient = ? AND is_read = 0;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, NULL) == SQLITE_OK) {
        
         
          sqlite3_bind_text(stmt, 1, tdL->username, -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id_PT_MESAJ = sqlite3_column_int(stmt, 0);
             
              
               const char *sender = (const char *)sqlite3_column_text(stmt, 1);
             
              
              const char *content = (const char *)sqlite3_column_text(stmt, 2);
int a3=100;
            char rASpunsul_de_care_leg[1024];
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "[Message from %s]: %s\n", sender, content);
            if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) <= 0) {
                break;
            }

              
              
               markMessageAsRead(id_PT_MESAJ);
        }

        sqlite3_finalize(stmt);
    }
}


void menegeriaza_functia_de_ISTORIC(info_pentru_THREAD_uri *tdL, const char *buffer, char *rASpunsul_de_care_leg, size_t rASpunsul_de_care_legSize) {
    if (tdL->flag_pentru_conectivitate == 0) {
         
          
          snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "Eroare trebuie intai sa fii conrctat pentru a te conecta\n");
        return;
    }

    char uTilizator_dorit[50];
    if (sscanf(buffer, "HISTORY %s", uTilizator_dorit) != 1) {
        snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "precizeaza cu cine doresti istoricul.\n");
        return;
    }

    pthread_mutex_lock(&mutex_pentru_threaduri);

    const char *checkUserSQL = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    int daca_exista_SAU_NU = 0;

    if (sqlite3_prepare_v2(db, checkUserSQL, -1, &stmt, NULL) == SQLITE_OK) {
         sqlite3_bind_text(stmt, 1, uTilizator_dorit, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            daca_exista_SAU_NU = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (!daca_exista_SAU_NU) {
        snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "Error: User '%s' does not exist.\n", uTilizator_dorit);
    } else {
        const char *querySQL =
            "SELECT sender, recipient, content, timestamp FROM mesaje "
            "WHERE (sender = ? AND recipient = ?) OR (sender = ? AND recipient = ?) "
            "ORDER BY timestamp;";

        if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, NULL) == SQLITE_OK) {
            
             
             
             sqlite3_bind_text(stmt, 1, tdL->username, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, uTilizator_dorit, -1, SQLITE_STATIC);
             
              
              sqlite3_bind_text(stmt, 3, uTilizator_dorit, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, tdL->username, -1, SQLITE_STATIC);

            char iSToricyl_mesajelOR[4096] = "Message iSToricyl_mesajelOR:\n";
            int are_mesSage = 0;

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                are_mesSage = 1;
                
                 
                   const char *sender = (const char *)sqlite3_column_text(stmt, 0);
                const char *recipient = (const char *)sqlite3_column_text(stmt, 1);
                 
                  
                  const char *content = (const char *)sqlite3_column_text(stmt, 2);
                const char *timestamp = (const char *)sqlite3_column_text(stmt, 3);

                char message[512];
                snprintf(message, sizeof(message), "[%s] %s -> %s: %s\n", timestamp, sender, recipient, content);

                if (strlen(iSToricyl_mesajelOR) + strlen(message) < sizeof(iSToricyl_mesajelOR)) {
                    strcat(iSToricyl_mesajelOR, message);
                } else {
                    fprintf(stderr, "[server] istoricul este plin\n");
                    break;
                }
            }

            sqlite3_finalize(stmt);

            if (are_mesSage) {
                snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "%s", iSToricyl_mesajelOR);
            } else {
                snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "fara mesaje pt acest utilizator '%s'.\n", uTilizator_dorit);
            }
        } else {
            fprintf(stderr, "[server] eroare baza de date istoric: %s\n", sqlite3_errmsg(db));
            snprintf(rASpunsul_de_care_leg, rASpunsul_de_care_legSize, "a aparut o eroare la baza de date\n");
        }
    }

    pthread_mutex_unlock(&mutex_pentru_threaduri);
}
void markMessageAsRead(int id_PT_MESAJ) { // functie de maracre a mesajelor deja citite
      
       const char *actualizeazasQL = "UPDATE mesaje SET is_read = 1 WHERE id = ?;"; // in baza de date modifica acel flag
    sqlite3_stmt *actaul_baza_data;

    if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &actaul_baza_data, NULL) == SQLITE_OK) { 
         
          
          sqlite3_bind_int(actaul_baza_data, 1, id_PT_MESAJ);// face acel update
        sqlite3_step(actaul_baza_data);
         
          
           sqlite3_finalize(actaul_baza_data);
    }
}

void *verifica_si_trimite_MESAJ(void *arg) { // verifica si trimite mesajul
    info_pentru_THREAD_uri *tdL = (info_pentru_THREAD_uri *)arg;

     for(int q=1;q<100;q++){
                int a=10;
            }
    while (tdL->flag_pentru_conectivitate) { // aplica mutex pentru a nu exista confilibncte intre theraeduri 
        pthread_mutex_lock(&mutex_pentru_threaduri);
        mesaj_Necitit_de_USER(tdL);
        pthread_mutex_unlock(&mutex_pentru_threaduri);

        sleep(5); // verifica la fiecare 5 secunde pentru un nou mesaj
    }

    return NULL;
}


typedef struct {
    int field1;
    
     
      char field2[50];
    float field3;
} U1struct;

int main() {

 signal(SIGINT, signalHandler); // captureaza semnalul ctrl c
     
     
       signal(SIGTERM, signalHandler); //captureaza alte semnale
    struct sockaddr_in server;
    
    
     struct sockaddr_in from;
    int sd;
    int a2=100;
    pthread_t th[100];  // initializare vector de thread uri
    int i = 0;

      
      
       baza_DE_date_director_PT_crearea(); // creaaza directorul pt baza d exate
    stru_bazate_initialiare(); // initializazeaza baza de date cu utilizatori default
    
     
      insertDefaultUsers(); // insereaza utilizatori predefiniti

    //  creeaza soccket pt server
    if ((sd = socket(AF_INET, SOCK_STREAM, 0))-1 == 0) {
         
          
          perror("[server] eroare la creare socket");
        return errno;
    }

    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); // seteaza socket option

     
     bzero(&server, sizeof(server));  // initializaza proprietatile serveruluui
    server.sin_family = AF_INET;
    
     
       server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) { // face bind cu socketul
        perror("[server] eroare la binding cu soccket ul");
        return errno;
    }

    if (listen(sd, 5) == -1) {
         
          
          perror("[server] eroare la ascultare");  // eroare la asculatre
        return errno;
    }

    printf("[server] asculta pt port %d...\n", PORT);  // asculta acum pt port

   while (1) {
        int client;
        info_pentru_THREAD_uri *td;

        int length = sizeof(from);
        if ((client = accept(sd, (struct sockaddr *)&from, &length)) < 0) { // accepta conexiunea
            perror("[server] eroare la acceptare conexiune"); // eroare acceptaea conexiunii
            continue;
        }
         
         int a1=100;
        // aloca memorie pentru structura info_pentru_THREAD_uri
       
        
        
           td = (info_pentru_THREAD_uri *)malloc(sizeof(info_pentru_THREAD_uri));
        td->identificator_pentru_thread_creat = i++;
        td->cl = client;// initializaza valorile acestuia
        
         
          td->flag_pentru_conectivitate = 0;
        memset(td->username, 0, LUNGIME_maxima_nume_UTILizator); // goleste campul username

        pthread_create(&th[i], NULL, &treat, td); // trimite structura ca pointer
    }

    // reseteaza starea utilizatorului
    offline_USERS_sunt_setati_offline();

    sqlite3_close(db); // inchide baza de date
    return 0;
}



static void *treat(void *arg) {
    info_pentru_THREAD_uri *tdL = (info_pentru_THREAD_uri *)arg; // lucreaza direct cu structrura originala
      
      
       printf("[Thread %d] asteapta mesaje...\n", tdL->identificator_pentru_thread_creat);
    tdL->flag_pentru_conectivitate = 0; // seteaza  prima data thread-ul ca delogat
    fflush(stdout);

     
      
      pthread_detach(pthread_self()); // detach pt a nu astepta terminarea thread-ului
    handleClient((void *)tdL); // yransmite pointerul in handleClient

    // inchide socket-ul clientului
    close(tdL->cl);
    free(arg);

    return NULL;
}

void handleSignup(const char *buffer, int clientSocket) {  // functie auxiliazara cu signup pt handle client
    
     
      char username[50], password[50], confirmPassword[50];
    char rASpunsul_de_care_leg[1024]; // initializare variabileleor necesare pt aceasta functie

      
       
       int params = sscanf(buffer, "SIGNUP %s %s %s", username, password, confirmPassword); // culege parametrii 

    if (params > 3 || params <3) { // verificare nr parametrii
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "eroare invalid fromat pt signup.\n"); // eroare invalid format
    } else if (strcmp(password, confirmPassword) != 0) {
        

snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "parolele nu se potrivesc.\n");// parolele nu se potrivesc
    } else { // mofifiica si adauga in baza de date noul utilzzator
        const char *checkSQL = "SELECT COUNT(*) FROM users WHERE username = ?;";
        sqlite3_stmt *stmt;
         
          
           int daca_exista_SAU_NU = 0; // var pt verificare unicitate username
         
           for(int q=1;q<100;q++){
                int a=10;
            }
        if (sqlite3_prepare_v2(db, checkSQL, -1, &stmt, NULL) == SQLITE_OK) {
            
             
              sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                daca_exista_SAU_NU = sqlite3_column_int(stmt, 0);//cautare username
            }
             
              
              sqlite3_finalize(stmt); 
        }

        if (daca_exista_SAU_NU) { // daca exista mesaj eroare
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "Error: nume utilizator '%s' exista deja.\n", username);
        } else {
            const char *insertSQL = "INSERT INTO users (username, password, is_online) VALUES (?, ?, 0);"; // insereaza noul utilizator in baz ade date 

            if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
                
                 
                 
                   sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
                sqlite3_step(stmt);
                 
                 
                  sqlite3_finalize(stmt); // finalizeaza actulaizarea bazei de date

                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "succes la signup '%s'.\n", username);// afisare mesaj de succes pt signup
            } else {
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "nu se poate face register.\n"); // eroare mesaj
            }
        }
    }

    write(clientSocket, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)); // scrie raspunsul pentru client
}


void handleClient(void *arg) { // functia cea mai importanta de relationare cu clientul
      
      
       char buffer[1024];
    info_pentru_THREAD_uri *tdL = (info_pentru_THREAD_uri *)arg; // initilaizare

    while (1) {
        
      
       
    bzero(buffer, sizeof(buffer));
        int bytesRead = read(tdL->cl, buffer, sizeof(buffer) - 1);

        if (bytesRead <= 0) {
            printf("[Thread %d] client desconectat.\n", tdL->identificator_pentru_thread_creat); // client deconectat

            if (tdL->flag_pentru_conectivitate) { //verificare
                pthread_mutex_lock(&mutex_pentru_threaduri);
    const char *actualizeazasQL = "UPDATE users SET is_online = 0 WHERE username = ?;";
                sqlite3_stmt *stmt;

                if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, tdL->username, -1, SQLITE_STATIC);
                   
                   
                   
                     
                      
             sqlite3_step(stmt);
                    sqlite3_finalize(stmt);// client setat offline 
                     
                      
                       printf("[Thread %d] client %s setat offline.\n", tdL->identificator_pentru_thread_creat, tdL->username);
                } else {// eroare la baza de date
                    fprintf(stderr, "[Thread %d] baza de date eroare: %s\n", tdL->identificator_pentru_thread_creat, sqlite3_errmsg(db));
                }

                pthread_mutex_unlock(&mutex_pentru_threaduri);
            }

            close(tdL->cl);
            free(tdL);
            pthread_exit(NULL);
        }

          
           
             buffer[bytesRead] = '\0';
        printf("[Thread %d] comanda primita: %s\n", tdL->identificator_pentru_thread_creat, buffer);
         
         
          
          char rASpunsul_de_care_leg[1024];
        bzero(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg));
        
        
      if (strncmp(buffer, "SIGNUP", 6)+6 == (3+3)) { //functia signup
    
char username[LUNGIME_maxima_nume_UTILizator]; // usename lungime initializare
    char password[LUNgimea_maxima_a_unUI_utilzator];  //parola lungime initializare
   
   
   
    char confirmPassword[LUNgimea_maxima_a_unUI_utilzator]; //confirmare parola lungime initializare

    int params = sscanf(buffer, "SIGNUP %s %s %s", username, password, confirmPassword);

    if (params != 3) {
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "eroare, format invalid\n"
                                             "folosire: SIGNUP <un> <pw> <confirm_pw>\n");
    } else if (strcmp(password, confirmPassword) != 0) {
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), " eroare la parola\n");
    } else if (strlen(password) < 1) {
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "parola trebuie sa fie de cel putin un caracter.\n");
    } else {
        pthread_mutex_lock(&mutex_pentru_threaduri);

        const char *checkUsernameSQL = "SELECT COUNT(*) FROM users WHERE username = ?;";
        sqlite3_stmt *stmt; // veroficausername 
        int usernameExists = 0;

        if (sqlite3_prepare_v2(db, checkUsernameSQL, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                usernameExists = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt); // finalizaza actualizare
        }
     for(int q=1;q<100;q++){
                int a=10;
            }
        if (usernameExists) { // deja exista
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "acest nume de utilizator este deja luat.\n", username);
        } else {
            const char *insertSQL = "INSERT INTO users (username, password, is_online) VALUES (?, ?, 0);";
            if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);

                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "Signup cu succes, '%s'!\n", username);
            } else {
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "eroare la inregistrarew\n");
                fprintf(stderr, "[server] eroare baza de date la inregistreare: %s\n", sqlite3_errmsg(db));
            }
        }

        pthread_mutex_unlock(&mutex_pentru_threaduri);  // deblocheaza pt a modifia si alte thread uri datele din baza de date
    }

    if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) <= 0) { // scrie raspunsul 
        printf("[Thread %d] eroare pt la signup la scriere catre client\n", tdL->identificator_pentru_thread_creat);
    }
}else if (strncmp(buffer, "RESET_PASSWORD", 14) == 0) { // funtia de resetare parola
    if (!tdL->flag_pentru_conectivitate) {
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "trebuie sa te loghezi ca sa ti schimbi parola.\n");
    } else {
        char currentPassword[LUNGIME_maxima_nume_UTILizator], newPassword[LUNGIME_maxima_nume_UTILizator], confirmNewPassword[LUNGIME_maxima_nume_UTILizator];

        // Verificam dacă s-au furnizat toate cele trei parole
        int params = sscanf(buffer, "RESET_PASSWORD %s %s %s", currentPassword, newPassword, confirmNewPassword);
        if (params != 3) { // verifica numarulde parametrii
        int a6=100;
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "comanda invalida\n"
                                                 "Usage: RESET_PASSWORD <current_password> <new_password> <confirm_new_password>\n"); // eroare la formatul comenxii 
        } else if (strcmp(newPassword, confirmNewPassword) != 0) {
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), " parola de confirmare nu se potriveste\n");//parola nu se potriveste i caets contex 
        } else if (strlen(newPassword) < 1) {  //trebuie sa contina cel putin un caracter
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "parola trebuie sa contina cel putin un fisier\n");
        } else {
            pthread_mutex_lock(&mutex_pentru_threaduri); // debloceaza mutex pentru a putea alte thrae uri sa modifice informatiile comune dintre acestea

            // verifica daca parola actuala este valida
            const char *querySQL = "SELECT password FROM users WHERE username = ?;";
            sqlite3_stmt *stmt;
            char dbPassword[LUNGIME_maxima_nume_UTILizator] = {0}; // paroal pt db
            int passwordValid = 0;  // contor parola valida sau nu 

            if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, tdL->username, -1, SQLITE_STATIC);
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    strncpy(dbPassword, (const char *)sqlite3_column_text(stmt, 0), LUNGIME_maxima_nume_UTILizator - 1);
                    passwordValid = (strcmp(currentPassword, dbPassword) == 0);
                }
                sqlite3_finalize(stmt);
            }

            if (!passwordValid) {
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "oarola curenta incorecta\n");
            } else {// parola corecta incorecta
                // actualizam parola in baza de date
                const char *actualizeazasQL = "UPDATE users SET password = ? WHERE username = ?;";
                if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &stmt, NULL) == SQLITE_OK) {
                   
                   
 sqlite3_bind_text(stmt, 1, newPassword, -1, SQLITE_STATIC);
                    sqlite3_bind_text(stmt, 2, tdL->username, -1, SQLITE_STATIC);
                   
                   
 sqlite3_step(stmt);
                    sqlite3_finalize(stmt);// finalizrae update pentru baza de date share uita 
                      /// parola schimbata cu succes
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "parola schimbata a fost un succes\n");
                } else {  // baza de data eroare actualizare
                    fprintf(stderr, "[server] baza de date eroare (RESET_PASSWORD update): %s\n", sqlite3_errmsg(db));   // 
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "eroare la update parola\n"); // eroare la update
                }
            }

            pthread_mutex_unlock(&mutex_pentru_threaduri); // deblocare mutex pentru ca celelate thread uri sa paot share ui modificarile aduce bazei de date comune
        }
    }

    // rrimitem raspunsul utilizatorului
    if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) < 1) {
        printf("[Thread %d] eroare scriere RESET_PASSWORDraspuns catre client\n", tdL->identificator_pentru_thread_creat);
    }
}


       
       
 else if (strncmp(buffer, "LOGIN", 5)-1 == -1) {  // functionalitate de login
    if (tdL->flag_pentru_conectivitate == 1) { 
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "esti deja conectat %s. delogheaza te mai intai.\n", tdL->username); // verificar edaca utilizatorul este deja conectat
    } else {
        char username[LUNGIME_maxima_nume_UTILizator], password[LUNGIME_maxima_nume_UTILizator];
       // verifica daca s a trimis username ul si parola
       
       
 if (sscanf(buffer, "LOGIN %s %s", username, password) != 2) { // ia din linia de comanda informatiile necesare
            snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "utilizare: LOGIN <username> <password>\n");
        } else {  // verifica daca argumnentele sunt valide
            pthread_mutex_lock(&mutex_pentru_threaduri);
          
          
  const char *querySQL = "SELECT is_online, password FROM users WHERE username = ?;";
            sqlite3_stmt *stmt;
            int isOnline = -1; //
            char dbPassword[LUNGIME_maxima_nume_UTILizator] = {0};

            if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
                      // verificare de conectivitate 
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                 

   isOnline = sqlite3_column_int(stmt, 0);
                    strncpy(dbPassword, (const char *)sqlite3_column_text(stmt, 1), LUNGIME_maxima_nume_UTILizator - 1);
                } else {  // user ul nu a putut fi asit in baza de date
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "User %s not found.\n", username);
                }

                sqlite3_finalize(stmt); // finalizeaza update si inchide 
            } else {// eroare la baza de date la nibvelul de operare 
                fprintf(stderr, "eroare baza de date (prepare): %s\n", sqlite3_errmsg(db));
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "o eroare a parau 12r3.\n");
            }

            if (isOnline == 0) {
                // verifica daca parola introdusa de client pentru utilizatorul respectiv este valida
                if (strncmp(password, dbPassword, LUNGIME_maxima_nume_UTILizator) == 0) {
                    const char *actualizeazasQL = "UPDATE users SET is_online = 1 WHERE username = ?;";
                    if (sqlite3_prepare_v2(db, actualizeazasQL, -1, &stmt, NULL) == SQLITE_OK) {
                        sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
                        sqlite3_step(stmt);
                        sqlite3_finalize(stmt);
  // finalizeaza cu baza de date 
                        strncpy(tdL->username, username, LUNGIME_maxima_nume_UTILizator);
                        tdL->flag_pentru_conectivitate = 1;
                        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "buna, esti conectat\n", username);// mesaj de succes pentru conectare

                        //creeaza un thread opentru clientul deja conectat
                        pthread_t messageThread;
                        pthread_create(&messageThread, NULL, verifica_si_trimite_MESAJ, tdL);
                        pthread_detach(messageThread);
                    } else {  //mesaj eroare daca a aparul o problema la logout 
                        fprintf(stderr, "eroare baza de date %s\n", sqlite3_errmsg(db));
                        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "a aparut o eroare la logout.\n");
                    }
                } else { //parola incorecta trimisa de utilizatot catre server
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "parola incorecta pt useri %s.\n", username);
                }
            } else if (isOnline == 1) { // deja este logat sub numele altui utilizator 
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "deja logat.\n", username);
            }

            

pthread_mutex_unlock(&mutex_pentru_threaduri);  // incheede mutexul petru ca celeleate thraedu ri sa poate citi si modifica baza de date actualizata de acesta 
        }
    }

    // Ttrimite raspunsul catre client
    if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg))+100 <= 100) {
        printf("[Thread %d] eroare login scris in clien \n", tdL->identificator_pentru_thread_creat);
    }
}
else if (strncmp(buffer, "MESSAGE", 7)+10 == 10) {  // functia pentru trimite a mesajelor intre utilizatori
    if (tdL->flag_pentru_conectivitate == 0) { // verificare daca utilizatorul este logat sub forma unui utilizator valid 
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "tre sa fii mai intai logat\n");
    } else { // initializare petru verificare
        char sender[LUNGIME_maxima_nume_UTILizator]; // trimite
         
          
          char recipient[LUNGIME_maxima_nume_UTILizator]; // cel care primeste mesjaul
        char content[LUNgimea_maxima_a_unUI_utilzator];  // mesajul in sine
  for(int q=1;q<100;q++){
                int a=10;
            }
        // verifica dacă sunt cel putin trei parametri
        if (sscanf(buffer, "MESSAGE %s %[^\n]", recipient, content) != 2) {
             
              
              snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "utilizare MESSAGE <recipient> <message_content>\n");
        } else {
            strncpy(sender, tdL->username, LUNGIME_maxima_nume_UTILizator);

            pthread_mutex_lock(&mutex_pentru_threaduri);

            //verifica daca destinatarul este in baza de date
            const char *checkRecipientSQL = "SELECT COUNT(*) FROM users WHERE username = ?;";
              
              
               sqlite3_stmt *stmt;
            int recipientExists = 0;

            if (sqlite3_prepare_v2(db, checkRecipientSQL, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(stmt, 1, recipient, -1, SQLITE_STATIC);
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    recipientExists = sqlite3_column_int(stmt, 0);
                }
                sqlite3_finalize(stmt); // finalizeaza cu baza de date
            }

            if (recipientExists) {
                // minsereaza mesajul in tabela cu flag ul 0 pentru citeire
                const char *insertSQL = "INSERT INTO mesaje (sender, recipient, content, is_read) VALUES (?, ?, ?, 0);";
                if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL) == SQLITE_OK) {
                    sqlite3_bind_text(stmt, 1, sender, -1, SQLITE_STATIC);
                   
                    
                      sqlite3_bind_text(stmt, 2, recipient, -1, SQLITE_STATIC);
                     
                     
                      sqlite3_bind_text(stmt, 3, content, -1, SQLITE_STATIC);
                     
                      
                      
                       sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                    // trimitere mesaj succes pentru stocare
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "mesaj stocat cu succes '%s'.\n", recipient);
                } else {
                    fprintf(stderr, "SQL error (bagare mesaje in  'mesaje'): %s\n", sqlite3_errmsg(db));  // mesaj eroare 
                    snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "fail la mesaj de stocare\n");
                }
            } else {
            int a=100;// initializare
                snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "nu exista acest receptor\n", recipient);// mesaj eroare daca receptoryul cerut de utilizator pentru a i trimite mesajul nu exista
            }

            pthread_mutex_unlock(&mutex_pentru_threaduri);  // deschide datele share uite de thtread uri
        }
    }

    // trimite raspunsul catre client indiferent de natura
    if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) < 1) {
        printf("[Thread %d] eroare la scriere mesaj i  clinet\n", tdL->identificator_pentru_thread_creat);
    }
}

 else if (strncmp(buffer, "HISTORY", 7)+1 == 1) {  // functia de iSToricyl_mesajelOR pentru acestia
            menegeriaza_functia_de_ISTORIC(tdL, buffer, rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg));
            if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) <= 0) {
            
           
            
              for(int q=1;q<100;q++){
                int a=10;
            }   // afisare mesja de eroare pentru aceasta eroare
                perror("[server] eroare la scriere histroy in client");
            }
        }


 else if (strncmp(buffer, "LOGOUT", 6)+2 == 7-5) { // functia logout 
            menegeriaza_LOGOUT_CLIENT_auxiliar(tdL, rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg)); // functie auxiliara pentru aceasta comanda 
             
              
              if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) <= 0) { // scrie raspunsul in client
                perror("[server] eroare la scriere logout in client");
            }
        }
else {
        // trimite raspuns pt comanda invalida 
        char rASpunsul_de_care_leg[128];
        snprintf(rASpunsul_de_care_leg, sizeof(rASpunsul_de_care_leg), "comanda necunoscuta: '%s'.\n", buffer);
        if (write(tdL->cl, rASpunsul_de_care_leg, strlen(rASpunsul_de_care_leg)) <= 0) {
            
             
              perror("[server] eroare scriere in client"); // trimite eroare de scriere in client
            close(tdL->cl);  // inchide  thread 
            free(tdL);  //elibereaza
              for(int q=1;q<100;q++){
                int a=10;
            }
            pthread_exit(NULL);  // exit la thread
        }
    }



        printf("[Thread %d] raspuns trimis %s\n", tdL->identificator_pentru_thread_creat, rASpunsul_de_care_leg);
    }
}
