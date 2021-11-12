#include "SDK30.H"

#include "Globals.h"
#include "Dialogs.h"
#include "HTTP.h"
#include "Sistema.h"

extern struct Confs config;

#define NUMBER_OF_ITEMS(a) (sizeof(a)/sizeof((a)[0]))

#define __ENTER_KEY     -1
#define __BACK_KEY      -2
#define __EXIT_KEY      -3

#define CONTINUAR	1
#define VOLTAR		2
#define CANCELAR 	3

extern struct Produtos produto[MAX_PRODUTOS];

int RecManageMenu( const char *szTitle, int bRadioButtons, int nDefaultChoice, int nItems, char Items[][20], bool bShowNbLines)
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

bool ImprimirComprovante(Trecarga *dadosRecarga)
{
	FILE *hPrn = NULL;
	DATE dt;
	unsigned char Status;
	int ret = FALSE;
	int proxVia = 1;

	SV_read_date(&dt);

lblInicioImpressao:
	if (proxVia == 1)
	{
		DisplaySimpleMessage2lines ("Aguarde, imprimindo", "comprovante...", TRUE, TRUE);

		hPrn = fopen("PRINTER","w");

		pprintf("\xF");

		pprintf("    Data..: %.2s/%.2s/20%.2s Hora: %.2s:%.2s\n", dt.day, dt.month, dt.year, dt.hour, dt.minute);
		pprintf("    Loja..: %s - %s\n", config.cfCod, config.cfNomeFantasia);
		pprintf("    Venda.: %s\n", dadosRecarga->venda);
		pprintf("    Nsu...: %s\n", dadosRecarga->nsu);
		pprintf("    Este cupom nao tem valor fiscal\n");
		pprintf("    --------------------------------\n");
		pprintf("    Produto.....: %s\n", dadosRecarga->produto);
		pprintf("    Valor.......: R$ %s\n", dadosRecarga->valor);
		pprintf("    Telefone....: %s\n", dadosRecarga->f_msisdn);
		pprintf("    --------------------------------\n");
		pprintf("    Val.:  %s dias.\n\n", dadosRecarga->validade);
		pprintf("    --------------------------------\n");
		pprintf("\n\n\n");
		pprintf("        %s\n\n", __NOMEEMP__);
		pprintf("\n\n\n");

		pprintf("\x1B""@");

		pprintf(" ---------------------\n\n\n\n\n");

		ttestall(PRINTER, 0);

		status(hPrn, &Status);

		fclose(hPrn);

		if ((Status & (PRINT_ERROR | PAPER_OUT)) != 0)
			goto lblErro;

		proxVia = 2;

		PressioneTecla(30*100);
	}

	if (proxVia == 2)
	{
		DisplaySimpleMessage2lines ("Aguarde, imprimindo", "comprovante", TRUE, TRUE);

		hPrn = fopen("PRINTER","w");

		pprintf("\xF");

		pprintf("    2a. VIA - ESTABELECIMENTO\n");
		pprintf("    Data..: %.2s/%.2s/20%.2s Hora: %.2s:%.2s\n", dt.day, dt.month, dt.year, dt.hour, dt.minute);
		pprintf("    Loja..: %s - %s\n", config.cfCod, config.cfNomeFantasia);
		pprintf("    Venda.: %s  Nsu...: %s\n\n", dadosRecarga->venda, dadosRecarga->nsu);
		pprintf("    Produto.....: %s\n", dadosRecarga->produto);
		pprintf("    Valor.......: R$ %s\n", dadosRecarga->valor);
		pprintf("    Telefone....: %s\n\n", dadosRecarga->f_msisdn);

		pprintf("\x1B""@");

		pprintf("\n\n\n\n\n\n");

		ttestall(PRINTER, 0);

		status(hPrn, &Status);

		fclose(hPrn);

		if ((Status & (PRINT_ERROR | PAPER_OUT)) != 0)
			goto lblErro;
	}

	goto lblEnd;

lblErro:
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

	ret = FALSE;
	goto lblEnd;

lblEnd:
	return ret;
}

int opcaoRecarga(char *operadora, char *produto, char *rectipo, char *valor, char *validade)
{
	char msisdn[12];
	char f_msisdn[16];
	char strmsg[30];
	char cKey;
	char msg[128];
	char response[128];
	char serial[65];
	char idvenda[15];
	char nsu[15];
	char *token;

	int iRet = 0;
	int bContinue = CONTINUAR;
	int i;

	bool vContinue = TRUE;
	bool procRecarga = FALSE;

	FILE *hKeyb = NULL;

	Trecarga dadosrec;

	while (vContinue)
	{
		memset(msisdn, 0, sizeof(msisdn));
		memset(f_msisdn, 0, sizeof(f_msisdn));

		if (PegaMSISDN(operadora, msisdn, f_msisdn) == 0)
		{
			_clrscr();

			memset(strmsg, 0, sizeof(strmsg));
			sprintf(strmsg, "Confirma a recarga");
			_DrawExtendedString(0, 0, strmsg, _OFF_, _NORMALE_, _PROPORTIONNEL_);

			memset(strmsg, 0, sizeof(strmsg));
			sprintf(strmsg, "%s", produto);
			_DrawExtendedString(0, 10, strmsg, _OFF_, _NORMALE_, _PROPORTIONNEL_);

			memset(strmsg, 0, sizeof(strmsg));
			sprintf(strmsg, "para o numero abaixo?");
			_DrawExtendedString(0, 20, strmsg, _OFF_, _NORMALE_, _PROPORTIONNEL_);

			_DrawExtendedString(0, 35, f_msisdn, _OFF_, _XLARGE_, _PROPORTIONNEL_);
			PaintGraphics();

			hKeyb = fopen("KEYBOARD", "r");

			while(1)
			{
				cKey = 0;
				iRet = ttestall(KEYBOARD, 0);
				if (iRet & KEYBOARD)
					cKey = getchar();

				Bip();

				if (cKey == T_VAL) 			//botão verde (confirmar)
				{
					procRecarga = TRUE;
					vContinue = FALSE;
					break;
				}
				else if (cKey == T_ANN) 	//botão vermelho (cancelar)
				{
					vContinue = FALSE;
					break;
				}
				else if (cKey == T_CORR) 	//botão amarelo (corrigir)
					break;
			}

			fclose(hKeyb);
		}
		else
			vContinue = FALSE;
	}

	if (procRecarga)
	{
		PegarSerial(serial);

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "4636venda|%s|%s|%s|%s|%s|%s|%s|%s",
				config.cfCod, produto, valor, validade, msisdn, serial, operadora, rectipo);

		i = EnviaSocket(msg, strlen(msg), response, sizeof(response));

		if (i > 0)
		{
			if (strstr(response, "OK") != NULL)
			{
				memset(idvenda, 0, sizeof(idvenda));
				memset(nsu, 0, sizeof(nsu));

				token = strtok(response, "|");

				if (token != NULL)
				{
					token = strtok(NULL,  "|");
					if (token != NULL)
					{
						strcpy(idvenda, token);

						token = strtok(NULL,  "|");

						if (token != NULL)
						{
							strcpy(nsu, token);
						}
					}
				}

				strcpy(dadosrec.operadora, operadora);
				strcpy(dadosrec.produto, produto);
				strcpy(dadosrec.msisdn, msisdn);
				strcpy(dadosrec.f_msisdn, f_msisdn);
				strcpy(dadosrec.venda, idvenda);
				strcpy(dadosrec.nsu, nsu);
				strcpy(dadosrec.valor, valor);
				strcpy(dadosrec.validade, validade);

				ImprimirComprovante(&dadosrec);
			}
			else if (strcmp(response, "N_LIM") == 0)
			{
				DisplaySimpleMessage("Limite ultrapassado", TRUE, TRUE);
				ttestall(0, 200);
			}
			else if (strcmp(response, "ERR") == 0)
			{
				DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
				ttestall(0, 200);
			}
		}
		else
		{
			DisplaySimpleMessage("Falha de conexao", TRUE, TRUE);
			ttestall(0, 200);
		}
	}

	return bContinue;
}

int iniciarMenuRecarga(char *operadora, char *rectipo)
{
	int bContinue = CONTINUAR;
	int i;
	int j = 0;

	char RecMenu[MAX_PRODUTOS][20];
	char RecValor[MAX_PRODUTOS][20];
	char RecValidade[MAX_PRODUTOS][4];

	for (i = 0; i < MAX_PRODUTOS; i++)
	{
		if (strlen(produto[i].produto) != 0)
		{
			if (strcmp(produto[i].operadora, operadora) == 0 && strcmp(produto[i].rectipo, rectipo) == 0)
			{
				memset(RecMenu[j], 0, sizeof(RecMenu[j]));
				memset(RecValor[j], 0, sizeof(RecValor[j]));
				memset(RecValidade[j], 0, sizeof(RecValidade[j]));

				strcpy(RecMenu[j], produto[i].produto);
				strcpy(RecValor[j], produto[i].valorface);
				strcpy(RecValidade[j], produto[i].validade);
				j++;
			}
		}
		else
			break;
	}

	do
	{
		i = RecManageMenu("ESCOLHA O PRODUTO", 0, 0, j, RecMenu, TRUE);

		if (i >= 0)
			bContinue = opcaoRecarga(operadora, RecMenu[i], rectipo, RecValor[i], RecValidade[i]);
		else
			bContinue = VOLTAR;

	} while(bContinue == CONTINUAR);

	if (bContinue == VOLTAR) bContinue = CONTINUAR;

	return bContinue;
}
