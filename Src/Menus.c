#include "SDK30.H"

#include "Globals.h"
#include "Dialogs.h"
#include "HTTP.h"
#include "Conn.h"
#include "File.h"
#include "Backlight.h"
#include "Boleto.h"
#include "Sistema.h"
#include "Recarga.h"
#include "Inicio.h"

extern struct Confs config;

#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))

#define __ENTER_KEY     -1
#define __BACK_KEY      -2
#define __EXIT_KEY      -3

#define CONTINUAR	1
#define VOLTAR		2
#define CANCELAR 	3

char *MenuPrincipal[] =
{
    "Claro",
    "Claro PIN",
    "Oi",
    "Vivo",
    "Menu Adm"
};

char *MenuAdm[] =
{
    "Cobranca",
    "Relatorios",
    "Reimpressao Vendas",
    "Alterar Senha",
    "Atualizar",
    "Teste de Conexao",
    "Luz de Fundo"
};

char *MenuSistema[] =
{
	"Vincular Distribuidor",
	"Fechar Distribuidor",
	"Registrar Atend.",
	"Reiniciar",
	"Rede",
	"Servidor",
	"Informacoes Gerais",
	"Atualizar Sistema",
};

char *MenuConfig[] =
{
	"GPRS",
	"Ethernet Auto",
	"Ethernet Manual"
};

char *MenuServidor[] =
{
    "Nome do dominio",
    "IP servidor",
    "Porta servidor",
    "IP recargas",
    "Porta recarga"
};

char *MenuApn[] =
{
	"Claro - APN Publica",
	"Vivo - APN Publica",
	"Oi - APN Publica",
	"Tim - APN Publica",
	"Vivo - APN Opus",
	"Oi - APN APPs",
	"Outras"
};

char *MenuCobranca[] =
{
	"Listar cobrancas",
	"Imprimir boletos",
	"Credito a vista",
};

char *MenuRelatorios[] =
{
	"Venda p/ Data",
	"Venda p/ Usuario",
	"Consulta Saldo",
	"Consulta NSU"
};

char *MenuReimpressaoVendas[] =
{
	"Ultima venda",
	"Por Codigo",
};

int ManageMenu( const char *szTitle, int bRadioButtons, int nDefaultChoice, int nItems, char* Items[], bool bShowNbLines)
{
    // Menu.
    StructList Menu;
    int nY;
    int nMaxX=0;
    int nMaxY=0;

    ENTRY_BUFFER Entry;

    int i;
    int nInput;
    int nReturn;

    // Get Screen size.
    GetScreenSize( &nMaxY, &nMaxX );

    // For the menu height of the menu,
    nY = 0;

    if ((nDefaultChoice < 0) || (nDefaultChoice >= nItems))
    {
        nDefaultChoice = 0;
    }

    CreateGraphics(_MEDIUM_);

    memset( &Menu, 0, sizeof(Menu) );

    Menu.MyWindow.left   = 0;
    Menu.MyWindow.top    = nY;
    Menu.MyWindow.rigth  = nMaxX - 1;
    Menu.MyWindow.bottom = nMaxY - 1;
    if( nMaxY == 128 )
    {
        Menu.MyWindow.nblines = 10;
    }
    else
    {
        Menu.MyWindow.nblines = 5;
    }
    Menu.MyWindow.fontsize      = _MEDIUM_;
    Menu.MyWindow.type          = _PROPORTIONNEL_;
    Menu.MyWindow.font          = 0;
    Menu.MyWindow.correct       = _ON_;
    Menu.MyWindow.offset        = 1;
    Menu.MyWindow.shortcommand  = (bShowNbLines)	?_ON_:_OFF_;
    Menu.MyWindow.selected 		= (bRadioButtons) 	?_ON_:_OFF_;
    Menu.MyWindow.thickness     = 0;
    Menu.MyWindow.border        = _ON_;
    Menu.MyWindow.popup         = _NOPOPUP_;
    Menu.MyWindow.first         = nDefaultChoice;
    Menu.MyWindow.current       = nDefaultChoice;
    Menu.MyWindow.time_out      = 0;
    Menu.MyWindow.title         = (unsigned char*)szTitle;

    for( i = 0; i < nItems; i++ )
    {
        Menu.tab[i] = (unsigned char*)Items[i];
    }

    G_List_Entry((void*)&Menu);
    ttestall(ENTRY, 0);
    nInput = Get_Entry((void*)&Entry);

    switch( nInput )
    {
		case CR_ENTRY_OK:
			nReturn = Entry.d_entry[0];
			break;

		case CR_ENTRY_NOK:
			nReturn = __EXIT_KEY;
			break;

		default:
			nReturn = __BACK_KEY;
			break;
    }

    return nReturn;
}

int OperManageMenu( const char *szTitle, int bRadioButtons, int nDefaultChoice, int nItems, char Items[][20], bool bShowNbLines)
{
    // Menu.
    StructList Menu;
    int nY;
    int nMaxX=0;
    int nMaxY=0;

    ENTRY_BUFFER Entry;

    int i;
    int nInput;
    int nReturn;

    // Get Screen size.
    GetScreenSize( &nMaxY, &nMaxX );

    // For the menu height of the menu,
    nY = 0;

    if ((nDefaultChoice < 0) || (nDefaultChoice >= nItems))
    {
        nDefaultChoice = 0;
    }

    CreateGraphics(_MEDIUM_);

    memset( &Menu, 0, sizeof(Menu) );

    Menu.MyWindow.left   = 0;
    Menu.MyWindow.top    = nY;
    Menu.MyWindow.rigth  = nMaxX - 1;
    Menu.MyWindow.bottom = nMaxY - 1;
    if( nMaxY == 128 )
    {
        Menu.MyWindow.nblines = 10;
    }
    else
    {
        Menu.MyWindow.nblines = 5;
    }
    Menu.MyWindow.fontsize      = _MEDIUM_;
    Menu.MyWindow.type          = _PROPORTIONNEL_;
    Menu.MyWindow.font          = 0;
    Menu.MyWindow.correct       = _ON_;
    Menu.MyWindow.offset        = 1;
    Menu.MyWindow.shortcommand  = (bShowNbLines)	?_ON_:_OFF_;
    Menu.MyWindow.selected 		= (bRadioButtons) 	?_ON_:_OFF_;
    Menu.MyWindow.thickness     = 0;
    Menu.MyWindow.border        = _ON_;
    Menu.MyWindow.popup         = _NOPOPUP_;
    Menu.MyWindow.first         = nDefaultChoice;
    Menu.MyWindow.current       = nDefaultChoice;
    Menu.MyWindow.time_out      = 0;
    Menu.MyWindow.title         = (unsigned char*)szTitle;

    for( i = 0; i < nItems; i++ )
    {
        Menu.tab[i] = (unsigned char*)Items[i];
    }

    G_List_Entry((void*)&Menu);
    ttestall(ENTRY, 0);
    nInput = Get_Entry((void*)&Entry);

    switch( nInput )
    {
		case CR_ENTRY_OK:
			nReturn = Entry.d_entry[0];
			break;
		case CR_ENTRY_NOK:
			nReturn = __EXIT_KEY;
			break;
		default:
			nReturn = __BACK_KEY;
			break;
    }

    return nReturn;
}

void imprimirRelatorio(char linha[1000][50], int lin, char *cabec) 	//suporta relatórios de máx. 1000 linhas
{
	unsigned char Status;
	int j;

	FILE *hPrn = NULL;
	DATE dt;

	SV_read_date(&dt);

lblInicioImpressao:
	hPrn = fopen("PRINTER","w");

	pprintf("\xF");

	//cabeçalho
	pprintf(" Data..: %.2s/%.2s/20%.2s Hora: %.2s:%.2s\n", dt.day, dt.month, dt.year, dt.hour, dt.minute);
	pprintf(" Cliente: %s - %s\n", config.cfCod, config.cfNomeFantasia);
	pprintf(" ----------------------------------------\n");
	pprintf(" %s\n", cabec);

	//corpo
	for (j=0; j < lin; j++)
	{
		pprintf(" %s\n", linha[j]);
	}

	pprintf("\n\n\n");
	pprintf("\x1B""@");
	pprintf(" ---------------------\n\n\n\n\n");

	ttestall(PRINTER, 0);

	status(hPrn, &Status);

	fclose(hPrn);

	if ((Status & (PRINT_ERROR | PAPER_OUT)) != 0)
		goto lblErroImpressao;

	goto lblEnd;

lblErroImpressao:
	//tratamento de falta de papel ou erro da impressora
	if ((Status & PAPER_OUT) != 0)
	{
		DisplaySimpleMessage ("Favor colocar papel", TRUE, TRUE);

		while ((Status & PAPER_OUT) != 0)
		{
			ttestall(0, 100);
			status(hPrn, &Status);
		}

		PressioneTecla(0);

		goto lblInicioImpressao;
	}

	if ((Status & PRINT_ERROR) != 0)
	{
		DisplaySimpleMessage ("Falha na impressora", TRUE, TRUE);
	}
	goto lblEnd;

lblEnd:
	return;
}

void opcaoReiniciar()
{
	OEM_exit(0);
}

int opcaoVincularDistrib()
{
	int i;
	int ret = CONTINUAR;

	char ticket[20];
	char serial[65];
	char modelo[20];
	char iccid[22];
	char msg[256];
	char response[256];
	char *token;

	memset(ticket, 0, sizeof(ticket));
	memset(serial, 0, sizeof(serial));
	memset(modelo, 0, sizeof(modelo));
	memset(iccid, 0, sizeof(iccid));
	memset(msg, 0, sizeof(msg));

	if (PegaInteiro("Ticket de liberacao", 6, ticket) != 0)
		goto lblCancelado;

	PegarSerial(serial);
	PegarModelo(modelo);
	PegarICCID(iccid);

	sprintf(msg, "4636vinc|%s|%s|%s|%s|%s|%s", ticket, serial, modelo, iccid, __APPVER__, config.cfApn);

	i = EnviaSocket(msg, strlen(msg), response, sizeof(response));

	if (i > 0)
	{
		LimpaTela();

		if (strstr(response, "OK") != NULL)
		{
			token = strtok(response, "|");

			if (token != NULL)
			{
				token = strtok(NULL,  "|");
				if (token != NULL)
				{
					strcpy(config.cfCod, token);

					token = strtok(NULL,  "|");

					if (token != NULL)
					{
						strcpy(config.cfNomeFantasia, token);

						token = strtok(NULL,  "|");

						if (token != NULL)
						{
							strcpy(config.cfSenhaDesb, token);

							token = strtok(NULL,  "|");

							if (token != NULL)
							{
								strcpy(config.cfSenhaMagn, token);
							}
						}
					}
				}
			}

			SalvarConfigs();

			//atualiza produtos
			if (AtualizarProdutos() == 0)
			{
				SalvarProdutos();
			}

			DisplaySimpleMessage("Cliente Vinculado", TRUE, TRUE);

			PrintNetworkInfos();

			ret = CANCELAR;
		}
		else if(strcmp(response, "N_AUT") == 0)
		{
			DisplaySimpleMessage("Nao Autorizado", TRUE, TRUE);
			ret = CONTINUAR;
		}
		else if(strcmp(response, "N_SIM") == 0)
		{
			DisplaySimpleMessage("Chip nao cadastrado", TRUE, TRUE);
			ret = CONTINUAR;
		}

		ttestall(0, 200);
	}
	else
	{
		LimpaTela();
		DisplaySimpleMessage("Erro de conexao...", TRUE, TRUE);
		ttestall(0, 200);
		ret = CONTINUAR;
	}
	goto lblEnd;

lblCancelado:
	LimpaTela();
	DisplaySimpleMessage("Cancelado...", TRUE, TRUE);
	ttestall(0, 200);
	ret = CONTINUAR;

lblEnd:
	return ret;
}

int opcaoFechaDistrib()
{
	//TODO opcaoFechaDistrib

	return CONTINUAR;
}

int opcaoRegistrarAtend()
{
	FILE *pxMag2=NULL;
	FILE *pKeyb=NULL;
	byte ucLen=0;
	int iSta;
	char cKey;
	char tcTrk2[128];
	int iRet;

	//Abrir leitor lateral track 2
	pxMag2 = fopen("SWIPE2", "r*");

	pKeyb = fopen("KEYBOARD", "r");

	while(1)
	{
		DisplaySimpleMessage("Passe o cartao ==>", TRUE, TRUE);

		iSta = ttestall(KEYBOARD | SWIPE2, 30*100);

		if(iSta & KEYBOARD)
		{
			cKey = getchar();
			if (cKey == T_ANN)
				break;
		}
		else if(iSta & SWIPE2)
		{
			memset(tcTrk2, 0, sizeof(tcTrk2));

			iRet = is_iso2(pxMag2, &ucLen, (byte *)tcTrk2);
			if (iRet == ISO_OK)
			{
				if (strcmp(tcTrk2, config.cfSenhaMagn)==0)
				{
					//registrar o atendimento no servidor
					char msg[256];
					char resp[256];

					memset(msg, 0, sizeof(msg));
					memset(resp, 0, sizeof(resp));

					sprintf(msg, "4636atend|%s|%s", config.cfCod, config.cfSenhaMagn);

					if (EnviaSocket(msg, strlen(msg), resp, sizeof(resp)) > 0)
					{
						if (strcmp(resp, "OK") == 0)
						{
							DisplaySimpleMessage2lines("Atendimento", "Registrado", TRUE, TRUE);
							ttestall(0, 300);
						}
						else
						{
							DisplaySimpleMessage2lines("Erro de conexao", "Nao Registrado", TRUE, TRUE);
							ttestall(0, 300);
						}
					}
					else
					{
						DisplaySimpleMessage2lines("Erro de conexao", "Nao Registrado", TRUE, TRUE);
						ttestall(0, 300);
					}
					break;
				}
				else
				{
					DisplaySimpleMessage("Cartao invalido!", TRUE, TRUE);
					ttestall(0, 300);
				}
			}
			else
			{
				DisplaySimpleMessage("Erro de leitura!", TRUE, TRUE);
				ttestall(0, 300);
			}
		}
		else
			break;
	}

	fclose(pxMag2);
	fclose(pKeyb);

	return CONTINUAR;
}

int opcaoGPRSPublicoOutros()
{
	int bContinue = CONTINUAR;
	StructList Menu;
	ENTRY_BUFFER Entry;
	int MAXX, MAXY;
	int nReturn;

	char apn[50], opapn[10+50];
	char senha[50], oplogin[10+50];
	char login[50], opsenha[10+50];

	strcpy(apn, config.cfApn);
	strcpy(login, config.cfApnlogin);
	strcpy(senha, config.cfApnpwd);

	GetScreenSize(&MAXY, &MAXX);

	Menu.MyWindow.left         = 0;
	Menu.MyWindow.top          = 0;
	Menu.MyWindow.rigth        = (MAXX-1);
	Menu.MyWindow.bottom       = (MAXY-1);
	Menu.MyWindow.nblines      = 4;
	Menu.MyWindow.fontsize     = _MEDIUM_;
	Menu.MyWindow.type         = _PROPORTIONNEL_;
	Menu.MyWindow.font         = 0;
	Menu.MyWindow.shortcommand = _OFF_;
	Menu.MyWindow.selected     = _OFF_;
	Menu.MyWindow.offset       = 0;
	Menu.MyWindow.thickness    = 0	;
	Menu.MyWindow.border       = _ON_;
	Menu.MyWindow.popup        = _NOPOPUP_;
	Menu.MyWindow.first        = 0;
	Menu.MyWindow.time_out     = 0;
	Menu.MyWindow.current      = 0;
	Menu.MyWindow.title        = (unsigned char *) "OUTRAS";

	do
	{
		memset(opapn, 0, sizeof(opapn));
		memset(oplogin, 0, sizeof(oplogin));
		memset(opsenha, 0, sizeof(opsenha));

		sprintf(opapn, "APN> %s", apn);
		sprintf(opsenha, "Login> %s", senha);
		sprintf(oplogin, "Senha> %s", login);

		Menu.tab[0] = (unsigned char *) opapn;
		Menu.tab[1] = (unsigned char *) oplogin;
		Menu.tab[2] = (unsigned char *) opsenha;
		Menu.tab[3] = (unsigned char *) "=> Confirmar";
		Menu.tab[4] = NULL;

		G_List_Entry((void*) &Menu);
		ttestall(ENTRY, 0);

		switch( Get_Entry((void*) &Entry) )
		{
			case CR_ENTRY_OK:
				nReturn = Entry.d_entry[0];
				break;
			case CR_ENTRY_NOK:
				nReturn = __EXIT_KEY;
				break;
			default:
				nReturn = __BACK_KEY;
				break;
		}

		switch(nReturn)
		{
			case 0:	PegaAlfa("Digite a APN", apn); break;
			case 1: PegaAlfa("Digite o Login", login); break;
			case 2: PegaAlfa("Digite a Senha", senha); break;
			case 3:
				strcpy(config.cfApn, apn);
				strcpy(config.cfApnlogin, login);
				strcpy(config.cfApnpwd, senha);

				if (SalvarConfigs())
				{
					printf("Configurado...");
					ttestall(0, 2*100);
					opcaoReiniciar();
				}
				else
				{
					printf("Erro");
					ttestall(0, 2*100);
				}
				break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoGPRSPublico()
{
	int bContinue = CONTINUAR;
	int salvar = FALSE;
	char sem_ip[] =  "0.0.0.0";

	strcpy(config.cfNetwork, "GPRS");
	strcpy(config.cfIp, sem_ip);
	strcpy(config.cfNetmask, sem_ip);
	strcpy(config.cfGateway, sem_ip);
	strcpy(config.cfDNS1, sem_ip);
	strcpy(config.cfDNS2, sem_ip);

	do
	{
		switch(ManageMenu("Configuracoes", 0, 0, NUMBER_OF_ITEMS(MenuApn), MenuApn, TRUE))
		{
			case 0:
				//Claro - APN Pública
				strcpy(config.cfApn, "generica.claro.com.br");
				strcpy(config.cfApnlogin, "claro");
				strcpy(config.cfApnpwd, "claro");
				salvar = TRUE;
				break;
			case 1:
				//Vivo - APN Pública
				strcpy(config.cfApn, "zap.vivo.com.br");
				strcpy(config.cfApnlogin, "vivo");
				strcpy(config.cfApnpwd, "vivo");
				salvar = TRUE;
				break;
			case 2:
				//Oi - APN Pública
				strcpy(config.cfApn, "gprs.oi.com.br");
				strcpy(config.cfApnlogin, "oi");
				strcpy(config.cfApnpwd, "oi");
				salvar = TRUE;
				break;
			case 3:
				//Tim - APN Pública
				strcpy(config.cfApn, "tim.br");
				strcpy(config.cfApnlogin, "tim");
				strcpy(config.cfApnpwd, "tim");
				salvar = TRUE;
				break;
			case 4:
				//Vivo - APN Opus
				strcpy(config.cfApn, "opus.vivo.com.br");
				strcpy(config.cfApnlogin, "vivo");
				strcpy(config.cfApnpwd, "vivo");
				salvar = TRUE;
				break;
			case 5:
				//Oi - APN APPs
				strcpy(config.cfApn, "aplic.oi.com.br");
				strcpy(config.cfApnlogin, "oi");
				strcpy(config.cfApnpwd, "oi");
				salvar = TRUE;
				break;
			case 6:
				bContinue = opcaoGPRSPublicoOutros();
				break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR && !salvar);

	if (salvar)
	{
		if (SalvarConfigs())
		{
			printf("Configurado...");
			ttestall(0, 2*100);
			opcaoReiniciar();
		}
		else
		{
			printf("Erro");
			ttestall(0, 2*100);
		}
	}

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoEthernetManual()
{
	int bContinue = CONTINUAR;
	StructList Menu;
	ENTRY_BUFFER Entry;
	int MAXX, MAXY;
	int nReturn;

	char ip[16], opip[60];
	char netmask[16], opnetmask[10+16];
	char gateway[16], opgateway[10+16];
	char dns1[16], opdns1[10+16];
	char dns2[16], opdns2[10+16];

	strcpy(config.cfNetwork, "Ethernet Manual");

	strcpy(ip, config.cfIp);
	strcpy(netmask, config.cfNetmask);
	strcpy(gateway, config.cfGateway);
	strcpy(dns1, config.cfDNS1);
	strcpy(dns2, config.cfDNS2);

	GetScreenSize(&MAXY, &MAXX);

	Menu.MyWindow.left         = 0;
	Menu.MyWindow.top          = 0;
	Menu.MyWindow.rigth        = (MAXX-1);
	Menu.MyWindow.bottom       = (MAXY-1);
	Menu.MyWindow.nblines      = 4;
	Menu.MyWindow.fontsize     = _MEDIUM_;
	Menu.MyWindow.type         = _PROPORTIONNEL_;
	Menu.MyWindow.font         = 0;
	Menu.MyWindow.shortcommand = _OFF_;
	Menu.MyWindow.selected     = _OFF_;
	Menu.MyWindow.offset       = 0;
	Menu.MyWindow.thickness    = 0	;
	Menu.MyWindow.border       = _ON_;
	Menu.MyWindow.popup        = _NOPOPUP_;
	Menu.MyWindow.first        = 0;
	Menu.MyWindow.time_out     = 0;
	Menu.MyWindow.current      = 0;
	Menu.MyWindow.title        = (unsigned char *) "Ethernet Manual";

	do
	{
		memset(opip, 0, sizeof(opip));
		memset(opnetmask, 0, sizeof(opnetmask));
		memset(opgateway, 0, sizeof(opgateway));
		memset(opdns1, 0, sizeof(opdns1));
		memset(opdns2, 0, sizeof(opdns2));

		sprintf(opip, "IP> %s", ip);
		sprintf(opnetmask, "Netmask> %s", netmask);
		sprintf(opgateway, "Gateway> %s", gateway);
		sprintf(opdns1, "DNS1> %s", dns1);
		sprintf(opdns2, "DNS2> %s", dns2);

		Menu.tab[0] = (unsigned char *) opip;
		Menu.tab[1] = (unsigned char *) opnetmask;
		Menu.tab[2] = (unsigned char *) opgateway;
		Menu.tab[3] = (unsigned char *) opdns1;
		Menu.tab[4] = (unsigned char *) opdns2;
		Menu.tab[5] = (unsigned char *) "=> Confirmar";
		Menu.tab[6] = NULL;

		G_List_Entry((void*) &Menu);
		ttestall(ENTRY, 0);

		switch( Get_Entry((void*) &Entry) )
		{
			case CR_ENTRY_OK:
				nReturn = Entry.d_entry[0];
				break;
			case CR_ENTRY_NOK:
				nReturn = __EXIT_KEY;
				break;
			default:
				nReturn = __BACK_KEY;
				break;
		}

		switch(nReturn)
		{
			case 0:	PegaEnderecoIP("Digite o IP", ip); break;
			case 1: PegaEnderecoIP("Digite a Netmask", netmask); break;
			case 2: PegaEnderecoIP("Digite o Gateway", gateway); break;
			case 3: PegaEnderecoIP("Digite o DNS 1", dns1); break;
			case 4: PegaEnderecoIP("Digite o DNS 2", dns2); break;
			case 5:
				strcpy(config.cfIp, ip);
				strcpy(config.cfNetmask, netmask);
				strcpy(config.cfGateway, gateway);
				strcpy(config.cfDNS1, dns1);
				strcpy(config.cfDNS2, dns2);

				if (SalvarConfigs())
				{
					printf("Configurado...");
					ttestall(0, 2*100);
					opcaoReiniciar();
				}
				else
				{
					printf("Erro");
					ttestall(0, 2*100);
				}
				break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoTipoConexao()
{
	int bContinue = CONTINUAR;

	do
	{
		switch(ManageMenu("Configuracoes", 0, 0, NUMBER_OF_ITEMS(MenuConfig), MenuConfig, TRUE))
		{
			case 0: bContinue = opcaoGPRSPublico(); break;
			case 1:
				strcpy(config.cfNetwork, "Ethernet Auto");
				if (SalvarConfigs())
				{
					printf("Configurado...");
					ttestall(0, 2*100);
					AtivarDHCP(TRUE);
					opcaoReiniciar();
				}
				else
				{
					printf("Erro");
					ttestall(0, 2*100);
				}
				bContinue = CANCELAR;
				break;
			case 2: bContinue = opcaoEthernetManual(); break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoConfServidor()
{
	int bContinue = CONTINUAR;
	char buffer[50];

	do
	{
		switch(ManageMenu("Conf. do Servidor", 0, 0, NUMBER_OF_ITEMS(MenuServidor), MenuServidor, FALSE))
		{
			case 0:
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, config.cfServDominio);
				if (PegaAlfa("Dominio do Servidor", buffer) == 0)
				 {
					 strcpy(config.cfServDominio, buffer);
					 SalvarConfigs();
				 }
				 break;
			case 1:
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, config.cfServIP);
				if (PegaEnderecoIP("IP do servidor", buffer) == 0)
				 {
					 strcpy(config.cfServIP, buffer);
					 SalvarConfigs();
				 }
				 break;
			case 2:
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, config.cfServPorta);
				if (PegaInteiro("Porta do servidor", 5, buffer) == 0)
				 {
					 strcpy(config.cfServPorta, buffer);
					 SalvarConfigs();
				 }
				 break;
			case 3:
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, config.cfRecIP);
				if (PegaEnderecoIP("IP de recargas", buffer) == 0)
				 {
					 strcpy(config.cfRecIP, buffer);
					 SalvarConfigs();
				 }
				 break;
			case 4:
				memset(buffer, 0, sizeof(buffer));
				strcpy(buffer, config.cfRecPorta);
				if (PegaInteiro("Porta de recargas", 5, buffer) == 0)
				 {
					 strcpy(config.cfRecPorta, buffer);
					 SalvarConfigs();
				 }
				 break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoInformacoesGerais()
{
	//TODO complementar
	PrintNetworkInfos();

	return CONTINUAR;
}

int opcaoAtualizarVersao()
{
	AtualizarSistema();

	return CANCELAR;
}

/*
 * inicia o menu do sistema
 * senha (entrada): 0 = pede senha, 1 = não pede a senha
 */
void iniciarMenuSistema(int pedirsenha)
{
	int prec;
	int bContinue = CONTINUAR;

	char senhatec[11];
	int cr;

	prec = StateHeader(0);

	if (pedirsenha == 0)
	{
		cr = PegaSenha("Senha Tecnica", senhatec);

		if (cr == 0)
		{
			if (strcmp(senhatec, "159636") == 0)	//TODO Implementar senha diária ou invés de senha fixa
			{
				goto lblMenuSistema;
			}
			else
			{
				LimpaTela();
				ShowDialogBox("Senha invalida", NULL, 2);
			}
		}
		else if (cr == 1)  	//CR_ENTRY_NOK
		{
			LimpaTela();
			ShowDialogBox("Senha invalida", NULL, 2);
		}
		goto lblEnd;
	}

lblMenuSistema:
	do
	{
		switch(ManageMenu("Menu Sistema", 0, 0, NUMBER_OF_ITEMS(MenuSistema), MenuSistema, TRUE))
		{
			case 0: bContinue = opcaoVincularDistrib(); break;
			case 1: bContinue = opcaoFechaDistrib(); break;
			case 2: bContinue = opcaoRegistrarAtend(); break;
			case 3: opcaoReiniciar(); break;
			case 4: bContinue = opcaoTipoConexao(); break;
			case 5: bContinue = opcaoConfServidor(); break;
			case 6: bContinue = opcaoInformacoesGerais(); break;
			case 7: bContinue = opcaoAtualizarVersao(); break;
			default: bContinue = CANCELAR; break;
		}
	} while(bContinue == 1);

lblEnd:
	StateHeader(prec);
}

int opcaoImprimirBoletos()
{
	/*
	//TRECHO DE CÓDIGO COMENTADO UTILIZADO PARA TESTES DE BOLETO (PODE SER APAGADO)
	Tboleto dadosboleto;

	strcpy(dadosboleto.nomebanco, "ITAU");
	strcpy(dadosboleto.codbanco, "341-7");
	strcpy(dadosboleto.linhadigitavel, "34191090080266702618720566780001162300000282606");
	strcpy(dadosboleto.cedente, "OPUS DEI DIST CARTOES TEL LIV JOR E REVISTAS LTDA");
	strcpy(dadosboleto.sacado, "BANCA DO JOSE");
	strcpy(dadosboleto.sacadoend, "RUA JOSE DA SILVA, 376");
	strcpy(dadosboleto.sacadocep, "25730-724 - PETROPOLIS - RJ");
	strcpy(dadosboleto.agencia, "6182");
	strcpy(dadosboleto.conta, "05667-8");
	strcpy(dadosboleto.carteira, "109");
	strcpy(dadosboleto.documento, "26670");
	strcpy(dadosboleto.nossonumero, "109/00026670-2");
	strcpy(dadosboleto.datavenc, "28/10/2014");
	strcpy(dadosboleto.dataproc, "27/10/2014");
	strcpy(dadosboleto.valor, "      2.826,06");
	strcpy(dadosboleto.instrucao1, "SUJEITO A PROSTESTO SE NAO FOR PAGO NO VENCIMENTO");
	strcpy(dadosboleto.instrucao2, "REEMISSAO DE COBRANCA ATRASADA");
	strcpy(dadosboleto.instrucao3, "VENCIMENTO ORIGINAL 27/10/2014 VALOR ORIGINAL R$ 2.826,06");

	ImprimeBoleto(&dadosboleto);
	*/

	ImprimeBoletos("MANUAL");

	return CONTINUAR;
}

int opcaoListarCobrancas()
{
	char linha[1000][50]; 		//suporta relatórios de máx. 1000 linhas
	char msg[50];
	char buffer[2048];
	char *token;

	int lin = 0;

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "4636listacob|%s", config.cfCod);

	int i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

	if (i > 0)
	{
		if (strcmp(buffer, "ERR") != 0)
		{
			//separa o buffer em linhas
			token = strtok(buffer, "\r\n");
			while (token != NULL)
			{
				strcpy(linha[lin], token);
				lin++;
				token = strtok(NULL, "\r\n");
			}

			imprimirRelatorio(linha, lin, "");
		}
		else
		{
			DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
			ttestall(0, 200);
		}
	}

	return CONTINUAR;
}

int opcaoCreditoVista()
{
	//TODO opcaoCreditoVista

	return CONTINUAR;
}

int opcaoCobranca()
{
	int bContinue = CONTINUAR;

	do
	{
		switch(ManageMenu("Menu Cobrancas", 0, 0, NUMBER_OF_ITEMS(MenuCobranca), MenuCobranca, TRUE))
		{
			case 0: bContinue = opcaoListarCobrancas(); break;
			case 1: bContinue = opcaoImprimirBoletos(); break;
			case 2: bContinue = opcaoCreditoVista(); break;
			default: bContinue = VOLTAR; break;                                  // Abort key pressed
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return CONTINUAR;
}

int opcaoVendaData()
{
	char dia[11];
	char linha[1000][50]; 		//suporta relatórios de máx. 1000 linhas
	int lin = 0;

	if (PegaData(dia) == 0)
	{
		char msg[50];
		char buffer[2048];
		char *token;
		char cabec[50];

		char diafrmt[] = { dia[6], dia[7], dia[8], dia[9], '/', dia[3], dia[4], '/', dia[0], dia[1], '\0' };

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "4636relatorio|%s|%s|%s", "VendaData", config.cfCod, diafrmt);

		int i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

		if (i > 0)
		{
			if (strcmp(buffer, "ERR") != 0)
			{
				//separa o buffer em linhas
				token = strtok(buffer, "\r\n");
				while (token != NULL)
				{
					strcpy(linha[lin], token);
					lin++;
					token = strtok(NULL, "\r\n");
				}

				sprintf(cabec, " RESUMO DO DIA %s\n", dia);

				imprimirRelatorio(linha, lin, cabec);
			}
			else
			{
				DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
				ttestall(0, 200);
			}
		}
	}
	return CONTINUAR;
}


int opcaoVendaUsuario()
{
	//TODO opcaoVendaRS

	return CONTINUAR;
}

int opcaoConsultaSaldo()
{
	//TODO opcaoConsultaSaldo

	return CONTINUAR;
}

int opcaoConsultaNSU()
{
	//TODO opcaoConsultaNSU

	return CONTINUAR;
}

int opcaoRelatorios()
{
	int bContinue = CONTINUAR;

	do
	{
		switch(ManageMenu("Menu Relatorios", 0, 0, NUMBER_OF_ITEMS(MenuRelatorios), MenuRelatorios, TRUE))
		{
			case 0: bContinue = opcaoVendaData(); break;
			case 1: bContinue = opcaoVendaUsuario(); break;
			case 2: bContinue = opcaoConsultaSaldo(); break;
			case 3: bContinue = opcaoConsultaNSU(); break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return CONTINUAR;
}

int opcaoUltimaVenda()
{
	char msg[50];
	char buffer[2048];
	char *token;

	Trecarga recarga;

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "4636reimpvenda|%s|%s", config.cfCod, "");

	int i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

	if (i > 0)
	{
		if (strstr(buffer, "OK") != NULL)
		{
			token = strtok(buffer, "|");

			if (token != NULL)
			{
				token = strtok(NULL,  "|");
				if (token != NULL)
				{
					strcpy(recarga.operadora, token);

					token = strtok(NULL,  "|");
					if (token != NULL)
					{
						strcpy(recarga.venda, token);

						token = strtok(NULL,  "|");
						if (token != NULL)
						{
							strcpy(recarga.nsu, token);

							token = strtok(NULL,  "|");
							if (token != NULL)
							{
								strcpy(recarga.produto, token);

								token = strtok(NULL,  "|");
								if (token != NULL)
								{
									strcpy(recarga.valor, token);

									token = strtok(NULL,  "|");

									if (token != NULL)
									{
										strcpy(recarga.msisdn, token);

										sprintf(recarga.f_msisdn, "(%c%c) %c%c%c%c%c-%c%c%c%c",
												token[0], token[1], token[2], token[3], token[4], token[5],
												token[6], token[7], token[8], token[9], token[10]);

										token = strtok(NULL,  "|");

										if (token != NULL)
										{
											strcpy(recarga.validade, token);
										}
									}
								}
							}
						}
					}
				}
			}

			ImprimirComprovante(&recarga);
		}
		else if (strcmp(buffer, "ERR") == 0)
		{
			DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
			ttestall(0, 200);
		}
	}

	return CONTINUAR;
}

int opcaoPorCodigo()
{

	char codVenda[10];

	if (PegaInteiro("Codigo da Venda", 10, codVenda) == CR_ENTRY_OK)
	{
		char msg[50];
		char buffer[2048];
		char *token;

		Trecarga recarga;

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "4636reimpvenda|%s|%s", config.cfCod, codVenda);

		int i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

		if (i > 0)
		{
			if (strstr(buffer, "OK") != NULL)
			{
				token = strtok(buffer, "|");

				if (token != NULL)
				{
					token = strtok(NULL,  "|");
					if (token != NULL)
					{
						strcpy(recarga.operadora, token);

						token = strtok(NULL,  "|");
						if (token != NULL)
						{
							strcpy(recarga.venda, token);

							token = strtok(NULL,  "|");
							if (token != NULL)
							{
								strcpy(recarga.nsu, token);

								token = strtok(NULL,  "|");
								if (token != NULL)
								{
									strcpy(recarga.produto, token);

									token = strtok(NULL,  "|");
									if (token != NULL)
									{
										strcpy(recarga.valor, token);

										token = strtok(NULL,  "|");

										if (token != NULL)
										{
											strcpy(recarga.msisdn, token);

											sprintf(recarga.f_msisdn, "(%c%c) %c%c%c%c%c-%c%c%c%c",
													token[0], token[1], token[2], token[3], token[4], token[5],
													token[6], token[7], token[8], token[9], token[10]);

											token = strtok(NULL,  "|");

											if (token != NULL)
											{
												strcpy(recarga.validade, token);
											}
										}
									}
								}
							}
						}
					}
				}

				ImprimirComprovante(&recarga);
			}
			else if (strcmp(buffer, "ERR") == 0)
			{
				DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
				ttestall(0, 200);
			}
		}
	}

	return CONTINUAR;
}

int opcaoReimpressaoVendas()
{
	int bContinue = CONTINUAR;

	do
	{
		switch(ManageMenu("Reimpressao Venda", 0, 0, NUMBER_OF_ITEMS(MenuReimpressaoVendas), MenuReimpressaoVendas, TRUE))
		{
			case 0: bContinue = opcaoUltimaVenda(); break;
			case 1: bContinue = opcaoPorCodigo(); break;
			default: bContinue = VOLTAR; break;                                  // Abort key pressed
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

int opcaoAlterarSenha()
{
	char senhaant[11];
	char senhanova[11];
	char senhaconf[11];
	char serial[64];
	char msg[128];
	char resp[10];
	int ret;

	LimpaTela();

	if (!PegaSenha("Senha antiga", senhaant) == 0)
		goto lblErro;

	if (strcmp(senhaant, config.cfSenhaDesb) != 0)
		goto lblErro;

lblNovaSenha:
	LimpaTela();

	if (!PegaSenha("Senha nova (4-10 dig.)", senhanova) == 0)
		goto lblErro;

	if (!PegaSenha("Confirme a senha nova", senhaconf) == 0)
		goto lblErro;

	if (strcmp(senhanova, senhaconf) != 0)
	{
		LimpaTela();
		ShowDialogBox("Senhas nao conferem", NULL, 2);
		goto lblNovaSenha;
	}

	//envia nova senha para o servidor
	memset(msg, 0, sizeof(msg));
	memset(serial, 0, sizeof(serial));

	PegarSerial(serial);
	sprintf(msg, "4636senha|%s|%s|%s", config.cfCod, serial, senhanova);

	if (EnviaSocket(msg, strlen(msg), resp, sizeof(resp)) > 0)
	{
		if (strcmp(resp, "OK") == 0)
		{
			strcpy(config.cfSenhaDesb, senhanova);
			SalvarConfigs();

			LimpaTela();
			ShowDialogBox("Senha alterada", NULL, 2);
		}
		else
		{
			LimpaTela();
			ShowDialogBox("Erro de conexao", NULL, 2);
		}
	}

	ret = CANCELAR;
	goto lblEnd;

lblErro:
	_clrscr();
	PaintGraphics();
	ShowDialogBox("Senha invalida", NULL, 2);
	ret = CONTINUAR;
	goto lblEnd;

lblEnd:
	return ret;
}

int opcaoAtualizar()
{

	StateHeader(1);

	if (strcmp(config.cfNetwork, "GPRS") == 0)
	{
		DisplaySimpleMessage("Aguarde...", TRUE, TRUE);
		gprs_ppp_disconnect();
		gprs_stop();
		ttestall(0, 200);
	}

	Inicio();

	return CANCELAR;
}

int opcaoTesteConexao()
{
	TesteConexao();

	return CANCELAR;
}

int opcaoLuzFundo()
{
	DefinirLuzFundo();

	return CONTINUAR;
}

int iniciarMenuAdm()
{
	int bContinue = CONTINUAR;

	do
	{
		switch(ManageMenu("Menu Adm", 0, 0, NUMBER_OF_ITEMS(MenuAdm), MenuAdm, TRUE))
		{
			case 0: bContinue = opcaoCobranca(); break;
			case 1: bContinue = opcaoRelatorios(); break;
			case 2: bContinue = opcaoReimpressaoVendas(); break;
			case 3: bContinue = opcaoAlterarSenha(); break;
			case 4: bContinue = opcaoAtualizar(); break;
			case 5: bContinue = opcaoTesteConexao(); break;
			case 6: bContinue = opcaoLuzFundo(); break;
			default: bContinue = VOLTAR; break;
		}
	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}

void iniciarMenuPrincipal()
{
	int prec;
	int bContinue = CONTINUAR;
	int cr;
	int i;
	int op = 0;
	bool insere;

	char senhadesb[11];
	char MenuOperadora[MAX_PRODUTOS][20];
	char operadora[MAX_PRODUTOS][10];
	char rectipo[MAX_PRODUTOS][10];

	prec = StateHeader(0);

	//Senha de Desbloqueio
	cr = PegaSenha("Senha desbloqueio", senhadesb);

	if (cr == 0)
	{
		if (strcmp(senhadesb, config.cfSenhaDesb) == 0)
		{
			//resume as operadoras com base em produtos
			for (i = 0; i < MAX_PRODUTOS; i++)
			{
				if (produto[i].produto[0] == '\0')
					break;

				insere = FALSE;

				if (i == 0)
				{
					insere = TRUE;
				}
				else
				{
					if (strcmp(operadora[op-1], produto[i].operadora) != 0 ||
							strcmp(rectipo[op-1], produto[i].rectipo) != 0)
						insere = TRUE;
				}

				if (insere)
				{
					memset(operadora[op], 0, sizeof(operadora[op]));
					memset(rectipo[op], 0, sizeof(rectipo[op]));
					memset(MenuOperadora[op], 0, sizeof(MenuOperadora[op]));

					strcpy(operadora[op], produto[i].operadora);
					strcpy(rectipo[op], produto[i].rectipo);
					strcpy(MenuOperadora[op], produto[i].operadora);

					if (strcmp(rectipo[op], "ONLINE") != 0)
					{
						strcat(MenuOperadora[op], " ");
						strcat(MenuOperadora[op], rectipo[op]);
					}

					op++;
				}
			}

			memset(MenuOperadora[op], 0, sizeof(MenuOperadora[op]));
			strcpy(MenuOperadora[op], "MENU ADM");
			op++;

			//exibe o menu de operadoras
			do
			{
				i = OperManageMenu("OPCOES DE RECARGA", 0, 0, op, MenuOperadora, TRUE);

				if (i >= 0 && i < (op-1))
				{
					bContinue = iniciarMenuRecarga(operadora[i], rectipo[i]);
				}
				else if (i == (op-1))
				{
					iniciarMenuAdm();
				}
				else
				{
					bContinue = VOLTAR;
				}

			} while(bContinue == CONTINUAR);
		}
		else
		{
			LimpaTela();
			ShowDialogBox("Senha invalida", NULL, 2);
		}
	}
	else if (cr == 1)  	//CR_ENTRY_NOK
	{
		LimpaTela();
		ShowDialogBox("Senha invalida", NULL, 2);
	}

	StateHeader(prec);
}
