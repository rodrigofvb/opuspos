//Protótipos Conn.c
void PrintNetworkInfos();
void AtivarDHCP(int bOn);
bool Validate_ip4(char *buffer);
void AbrirEthernetManual();
int PegaIPServidor(char *ip, bool resolucaoDNS);
void PegaPortaServidor(int *nPort);
