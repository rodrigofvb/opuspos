#include "SDK30.H"

#include "Globals.h"
#include "Dialogs.h"
#include "GPRS.h"
#include "Conn.h"
#include "HTTP.h"
#include "File.h"
#include "Boleto.h"
#include "Sistema.h"
#include "Menu.h"

struct Produtos produto[MAX_PRODUTOS];
struct Confs config;
struct MsgTela msgtela;

int AtualizarHora()
{
	DATE dt;
	char msg[] = { '4', '6', '3', '6', 'h', 'o', 'r', 'a' };
	char buffer[15];

	if (_EnviaSocket(msg, sizeof(msg), buffer, sizeof(buffer)) > 0)
	{
		if (strstr(buffer, "OK") != NULL)
		{
			dt.day[0] 		= buffer[3+0]; 	dt.day[1] 		= buffer[3+1];
			dt.month[0] 	= buffer[3+2]; 	dt.month[1] 	= buffer[3+3];
			dt.year[0] 		= buffer[3+4]; 	dt.year[1] 		= buffer[3+5];
			dt.hour[0] 		= buffer[3+6]; 	dt.hour[1] 		= buffer[3+7];
			dt.minute[0] 	= buffer[3+8]; 	dt.minute[1] 	= buffer[3+9];
			dt.second[0] 	= buffer[3+10]; dt.second[1] 	= buffer[3+11];

			if (SV_write_date(&dt) != 0)
				return -1;
		}
		else
			return -1;
	}
	else
		return -2;

	return 0;
}

/*
 * Baixa os produtos do servidor e armazena na variável produto
 * retorna 0 se OK, ou -1 para erro de conexão
 */
int AtualizarProdutos()
{
	int linha = 0;
	int i;

	char msg[50];
	char buffer[2048];
	char recstr[MAX_PRODUTOS][100];
	char *token;

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "4636prod|%s", config.cfCod);

	i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

	if (i > 0)
	{
		memset(recstr, 0 , sizeof(recstr));

		//separa o buffer em linhas
		token = strtok(buffer, "\r\n");
		while (token != NULL)
		{
			memset(recstr[linha], 0 , sizeof(recstr[linha]));
			strcpy(recstr[linha], token);
			linha++;

			token = strtok(NULL, "\r\n");
		}

		//separa as linhas em campos
		for (i = 0; i < linha; i++)
		{
			token = strtok(recstr[i], "|");

			if (token != NULL)
			{
				memset(produto[i].produto, 0, sizeof(produto[i]).produto);
				strcpy(produto[i].produto, token);
				token = strtok(NULL, "|");

				if (token != NULL)
				{
					memset(produto[i].valorface, 0, sizeof(produto[i]).valorface);
					strcpy(produto[i].valorface, token);
					token = strtok(NULL, "|");

					if (token != NULL)
					{
						memset(produto[i].operadora, 0, sizeof(produto[i]).operadora);
						strcpy(produto[i].operadora, token);
						token = strtok(NULL, "|");

						if (token != NULL)
						{
							memset(produto[i].rectipo, 0, sizeof(produto[i]).rectipo);
							strcpy(produto[i].rectipo, token);
							token = strtok(NULL, "|");

							if (token != NULL)
							{
								memset(produto[i].validade, 0, sizeof(produto[i]).validade);
								strcpy(produto[i].validade, token);
							}
						}
					}
				}
			}
		}
	}
	else
		return -1;

	return 0;
}

/*
 * Baixa as mensagens de tela do servidor
 * retorna 0 se OK, ou -1 para erro de conexão
 */
int AtualizarMsgTela()
{
	int i;

	char msg[] = { '4', '6', '3', '6', 'm', 's', 'g', 't', 'e', 'l', 'a' };
	char buffer[50];
	char *token;

	i = _EnviaSocket(msg, sizeof(msg), buffer, sizeof(buffer));

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
					strcpy(msgtela.msglin1, token);

					token = strtok(NULL,  "|");
					if (token != NULL)
					{
						strcpy(msgtela.msglin2, token);
					}
				}
			}
		}
	}
	else
		return -1;

	return 0;
}

/*
 * Informa a troca do chip ao sistema caso o chip tenha sido alterado
 * retorna: 0 se o o chip foi trocado com sucesso, -1 chip não cadastrado, -2 Erro
 */
int VerificarTrocaChip()
{
	char iccid[22];
	char msg[50];
	char serial[65];
	char buffer[50];
	int i, ret = 0;

	PegarICCID(iccid);

	if (strcmp(iccid, config.cfICCID) != 0)
	{
		PegarSerial(serial);

		sprintf(msg, "4636trocachip|%s|%s|%s", config.cfCod, serial, iccid);

		i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

		if (i > 0)
		{
			if (strcmp(buffer, "OK") == 0)
			{
				strcpy(config.cfICCID, iccid);
				if (SalvarConfigs())
				{
					DisplaySimpleMessage("Chip alterado", TRUE, TRUE);
					ttestall(0, 200);
					ret = 0;
				}
				else
				{
					goto lblErr;
				}
			}
			else if (strcmp(buffer, "N_SIM") == 0)
			{
				DisplaySimpleMessage("Chip não cadastrado", TRUE, TRUE);
				ttestall(0, 200);
				ret = -1;
			}
			else if (strcmp(buffer, "ERR") == 0)
			{
				goto lblErr;
			}
		}
	}
	goto lblEnd;
lblErr:
	DisplaySimpleMessage("Erro ao alterar chip", TRUE, TRUE);
	ttestall(0, 200);
	ret = -2;
lblEnd:
	return ret;
}

void Inicio()
{
	bool bGRPS = FALSE;


	//cabeçalho
	//EventHeader (_SOCLE_STATE_ | _BATT_STATE_ | _NIV_RECEP_ | _GPRS_STATE_ | _RTC_STATE_ );

	//desabilita Telium Manager
	//PSQ_Manager_F(2);

	//ler configurações
	if (CarregarConfigs() != 0)
	{
		strcpy(config.cfServDominio, "opusapp.no-ip.org");
		strcpy(config.cfServIP, "200.166.40.18");
		strcpy(config.cfServPorta, "8888");
		strcpy(config.cfRecIP, "200.166.40.18");
		strcpy(config.cfRecPorta, "8888");

		StateHeader(0);
		opcaoTipoConexao();
		StateHeader(1);
	}

	if (strcmp(config.cfNetwork, "GPRS") == 0)
	{
		bGRPS = TRUE;
		AbrirGPRS();
	}
	else if (strcmp(config.cfNetwork, "Ethernet Manual") == 0)
	{
		bGRPS = FALSE;
		AbrirEthernetManual();
	}

	//atualiza hora
	AtualizarHora();
	//atualiza msg tela
	AtualizarMsgTela();

	//verifica se o cliente está vinculado
	if (strcmp(config.cfCod, "x") == 0)
	{
		DisplaySimpleMessage("Cliente nao vinculado", TRUE, TRUE);
		ttestall(0, 200);
	}
	else
	{
		if (bGRPS)
		{
			//verifica se o chip foi trocado
			if (VerificarTrocaChip())
			{
				StateHeader(0);
				opcaoTipoConexao();
				StateHeader(1);
			}
		}

		//atualiza produtos
		if (AtualizarProdutos() == 0)
		{
			SalvarProdutos();
		}
		else
		{
			CarregarProdutos();
		}

		//imprime boletos pendentes automaticamento
		ImprimeBoletos("AUTO");
	}
}
