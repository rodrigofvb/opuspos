/** variáveis globais **/

#ifndef GLOBALS_H

//global defines
#define __NOMEEMP__		"Opus Dei Distribuidora"
#define __APPVER__ 		"Alfa 1"

#define MAX_PRODUTOS 	100
#define SERV_IP_PADRAO 	200.166.40.18
#define PORTA_PADRAO 	8888

//global structs
struct Confs
{
	char cfNetwork[20];
	char cfApn[50];
	char cfApnlogin[50];
	char cfApnpwd[50];
	char cfDhcpmode[3];
	char cfIp[16];
	char cfNetmask[16];
	char cfGateway[16];
	char cfDNS1[16];
	char cfDNS2[16];
	char cfBacklight[4];
	char cfServDominio[50];
	char cfServIP[16];
	char cfRecIP[16];
	char cfServPorta[6];
	char cfRecPorta[6];
	char cfSenhaDesb[11];
	char cfSenhaMagn[128];
	char cfCod[10];
	char cfNomeFantasia[100];
	char cfICCID[22];
};

struct Produtos
{
	char produto[20];
	char valorface[20];
	char operadora[10];
	char rectipo[10];
	char validade[4];
} produto[MAX_PRODUTOS];

struct MsgTela
{
	char msglin1[23];
	char msglin2[23];
};

typedef struct Boleto
{
	char nomebanco[10];
	char codbanco[6];
	char linhadigitavel[48];
	char cedente[100];
	char sacado[100];
	char sacadoend[100];
	char sacadocep[100];
	char datavenc[11];
	char dataproc[11];
	char carteira[4];
	char documento[10];
	char nossonumero[15];
	char agencia[5];
	char conta[10];
	char valor[15];
	char instrucao1[100];
	char instrucao2[100];
	char instrucao3[100];
} Tboleto;

typedef struct Recarga
{
	char operadora[20];
	char venda[20];
	char nsu[20];
	char produto[20];
	char valor[20];
	char msisdn[12];
	char f_msisdn[16];
	char validade[10];
} Trecarga;

#endif
