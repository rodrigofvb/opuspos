//Protótipos em Dialog.c
void Bip();
void LimpaTela();
void DisplaySimpleMessage(char *msg, bool centroX, bool centroY);
void DisplaySimpleMessage2lines(char *linha1, char *linha2, bool centroX, bool centroY);
void ShowDialogBox(char *message, char *title, int timeout);
int PegaInteiro(char titulo[20], int tamanho_max, char *val_init);
int PegaSenha(char *titulo, char *senha);
int PegaAlfa(char titulo[20], char *val_init);
int PegaEnderecoIP(char titulo[20], char *val_init);
int PegaData(char *diadisp);
int PegaMSISDN(char *operadora, char *msisdn, char *m_msisdn);
void DisplayLogo();
void PressioneTecla(int timeout);
void PrintSimpleMessage(char *msg);
