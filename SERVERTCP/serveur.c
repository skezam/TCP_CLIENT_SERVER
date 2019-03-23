/* les includes */ 
 #include <stdio.h>
 #include <stdlib.h> /* pour exit */ 
 #include <sys/types.h> 
 #include <sys/socket.h> 
 #include <string.h> /* pour memset */ 
 #include <netinet/in.h> /* pour struct sockaddr_in */ 
 #include <arpa/inet.h> /* pour htons et inet_aton */ 
 #include <unistd.h> /* pour sleep */

  /* Les defines */
  #define PORT IPPORT_USERRESERVED // = 5000 le port 
  #define LG_MESSAGE 256
  #define BLEU "\033[00;34m"
  #define ROUGE "\033[00;31m"
  #define NORMAL "\033[00m"
  #define BLEUCLAIR "\033[01;34m"

/* Le corp du programme */

int main() {
  /* Integer */  
  int socketEcoute;
  int MLus; /* octets MLus */
  int MEcrits; /*  octets ecrit */
  int socketRecuEnvoi;/* octet de discuction */
  /* Structures de données */
  struct sockaddr_in pointDeRencontreLocal;
  struct sockaddr_in pointDeRencontreDistant;
  /* chaine de caracteres des messages */
  char messageEnvoi[LG_MESSAGE]; /* le message envoyer */
  char messageRecu[LG_MESSAGE]; /* le message recu */
  socklen_t longueurAdresse;
  //int retour;
  // Crée un socket de communication
  socketEcoute = socket(PF_INET, SOCK_STREAM, 0); /* 0 indique que l’on utilisera le protocole par défaut TCP */

  // Teste la valeur renvoyée par l’appel système socket()

  if (socketEcoute < 0) {
    perror("socket"); // Affiche le message d’erreur
    exit(-1); // On sort en indiquant un code erreur
  }
  printf("Socket créée avec succès ! (%d)\n", socketEcoute);
  // On prépare l’adresse d’attachement locale
  longueurAdresse = sizeof(struct sockaddr_in);
  memset( & pointDeRencontreLocal, 0x00, longueurAdresse);
  pointDeRencontreLocal.sin_family = PF_INET;
  pointDeRencontreLocal.sin_addr.s_addr = htonl(INADDR_ANY); // toutes les interfaces locales disponibles
  pointDeRencontreLocal.sin_port = htons(PORT);
  // On demande l’attachement local de la socket
  if ((bind(socketEcoute, (struct sockaddr * ) & pointDeRencontreLocal, longueurAdresse)) < 0) {
    perror("bind"); // bind permet de nommer un socket
    exit(-2);
  }
  printf("Socket attachée avec succès !\n");
  // On fixe la taille de la file d’attente à 5 (pour les demandes de connexion non encore traitées)
  if (listen(socketEcoute, 5) < 0) { // permet d'accepter les connexion en approche par le client
    perror("listen");
    exit(-3);
  }
  printf("Socket placée en écoute passive ...\n");

  // On commmence une boucle while pour l'attente de connexion
  // On initialise à 0 les messages 
  while (1) { // Remplir une zone mémoire avec un octet donné 
    memset(messageEnvoi, 0x00, LG_MESSAGE * sizeof(char));
    memset(messageRecu, 0x00, LG_MESSAGE * sizeof(char));

    /* Attente de connexion d'un client */
    printf(BLEU "Attente de connexion d'un client.\n\n" NORMAL); 
    /* Socket de Discution accepter la connexion est effectué */
    socketRecuEnvoi = accept(socketEcoute, (struct sockaddr * ) & pointDeRencontreDistant, & longueurAdresse);
    if (socketRecuEnvoi < 0) {
      perror("accept"); // accept permet d'accepter la premiere connexion en attente pour la mettre en mode passive
      close(socketRecuEnvoi);
      close(socketEcoute);
      exit(-4);
    }
    // On recupére les données des clients 
    MLus = read(socketRecuEnvoi, messageRecu, LG_MESSAGE * sizeof(char)); // read permet la reception et l'envoi d'octets sur un socket
    
    switch (MLus) {
    case -1:
      /* une erreur lecture  */
      perror("read");
      close(socketRecuEnvoi);
      exit(-5);
    case 0:
      /* la socket se ferme */
      fprintf(stderr, "La socket a été fermée par le client !\n\n"); // le socket se ferme en cas d'erreur faite par le client
      close(socketRecuEnvoi);
      return 0;
    default:
      /* réception de n octets */
      printf(ROUGE"Message reçu : %s (%d octets)\n\n" NORMAL, messageRecu, MLus); // on affiche le message recu et son nombre d'octets( message envoyé par le client)
    }
    
    // Envoi d'un message vers le client 
    printf("Repondre au client : ");
    scanf("%s",messageEnvoi);
  
    MEcrits = write(socketRecuEnvoi, messageEnvoi, strlen(messageEnvoi)); // ecrit un message et l'envoi au client

    switch (MEcrits) {
    case -1:
      /* une erreur ecriture */
      perror("write");
      close(socketRecuEnvoi);
      exit(-6);
    case 0:
      /* la socket se ferme */
      fprintf(stderr,ROUGE "La socket a été fermée par le client !\n\n" NORMAL);
      close(socketRecuEnvoi);
      return 0;
    default:
      printf(BLEUCLAIR "Message %s envoyé au client (%d octets)\n\n" NORMAL, messageEnvoi, MEcrits);// on affiche le message envoyé et son nombre d'octets
    }

    // On ferme la socket de discution
    close(socketRecuEnvoi);
  }
  // On arette le programme
  close(socketEcoute);
  return 0;
}
