#include "SDK30.h"
#include "IP_.h"

#include "Globals.h"
#include "Dialogs.h"
#include "GPRS.h"
#include "Conn.h"

extern struct Confs config;

#define __30_SECONDS__   	3000
#define __INFINITE__ 		0x00FFFFFF
#define __100_MSECONDS__ 	10

/*	envia uma mensagem ao servidor web e armazena o retorno (sem o cabeçalho HTTP) em response
 *  response (saída) = saída da mensagem
 *  retorna o tamanho em bytes da mensagem ou
 *  -1 erro de conexão
 *  -2 erro de configuração (ip do servidor)
 */
int HttpResponse(const char *httpmsg, char *response)
{
	int nHandle = 0;
	int nResult = 0;
	int nReceived = 0;
	int nTotalReceived = 0;
	int ret = 0;
	int tambuffer = 64; 	//buffer de recepção de 64 bytes
	int nPort;

	char host_ip[16];
	char *buffer;
	char *p1, *p2;

	unsigned char pucData[tambuffer];

	buffer = umalloc(2048);		//Aloca 2 Kb

	PegaIPServidor(host_ip, FALSE);
	PegaPortaServidor(&nPort);

	nResult = tcp_connect_to_server( host_ip, nPort, __30_SECONDS__ );

	//tenta conectar com o ip, se não funcionar faz um
	if( nResult <= 0 )
	{
		if (PegaIPServidor(host_ip, TRUE) == 0)
		{
			nResult = tcp_connect_to_server( host_ip, nPort, __30_SECONDS__ );
		}
	}

	if( nResult >= 0 )
	{
		nHandle = nResult;

		//
		// Send HTTP request.
		//
		while(( nResult = tcp_send_to_server(
				nHandle,
				(unsigned char *)httpmsg,
				strlen(httpmsg),
				__INFINITE__ )) == ERROR_TIMEOUT )
		{
			ttestall( 0, __100_MSECONDS__ );
		}

		if( nResult == strlen(httpmsg) )
		{
			memset(pucData, 0, sizeof(pucData));

			while(( nResult = tcp_recv_from_server( nHandle, pucData, tambuffer, __30_SECONDS__ )) > 0 )
			{
				nReceived = nResult;
				nTotalReceived += nReceived;

				if (nTotalReceived > 2048)
					buffer = urealloc(buffer, nTotalReceived); //realoca memória se passar de 2 Kb

				strncat(buffer, (char *)pucData, nReceived);

				memset(pucData, 0, sizeof(pucData));
			}

			if (nResult < 0 )
				ret = -1;

			buffer[nTotalReceived] = '\0';
		}

		// Disconnect and release the allocated handle.
		tcp_disconnect_from_server( nHandle );

		//separando a mensagem do cabecalho
		p1 = buffer;
		p2 = strstr(buffer, "\r\n\r\n");

		while(NULL != p2)
		{
			p1 = p2;
			p2 = strstr(p2+4, "\r\n\r\n");
		}

		sprintf(response, "%s", p1+4);

		ret = strlen(response);
	}
	else
		ret = -1;

	ufree(buffer);

	return ret;
}

/*
 * envia uma mensagem HTTP POST ao servidor web
 * - url (entrada) = url de destino
 * - params (entrada) = dados a serem enviados
 * - response (saida) = resposta do servidor web
 */
int HttpPost(char *url, char *params, char *response)
{
	int prec;
	int ret = 0;

	char szTemp[128];
	char httpmsg[512];

	LimpaTela();

	prec = StateHeader(1);

	DisplaySimpleMessage("Conectando...", TRUE, TRUE);

	//montagem do cabeçalho HTTP
	memset(httpmsg, 0, sizeof(httpmsg));

	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "POST %s HTTP/1.0\r\n", url);
	strncat(httpmsg, szTemp, strlen(szTemp));

	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "Content-Type: application/x-www-form-urlencoded\r\n");
	strncat(httpmsg, szTemp, strlen(szTemp));

	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "Content-Length: %d\r\n\r\n", (int)strlen(params));
	strncat(httpmsg, szTemp, strlen(szTemp));

	strncat(httpmsg, params, strlen(params));
	strncat(httpmsg, "\r\n\r\n",  4);
	// fim do cabeçalho HTTP

	if (HttpResponse(httpmsg, response) <= 0)
	{
		DisplaySimpleMessage("Erro de conexao", TRUE, TRUE);
		ttestall(0, 200);
		ret = -1;
	}

	StateHeader(prec);

	return ret;
}

/*
 * Envia uma mensagem socket e armazena a resposta do servidor em buffer
 * - msg (entrada): mensagem a ser enviada ao servidor
 * - msglen(entrada): tamanho da mensagem
 * - buffer (saída): buffer de resposta
 * - bufferlen (entrada), tamanho do buffer
 * retorna a quantidade de bytes recebidos ou -1 para erro de conexão
 */
int _EnviaSocket(char *msg, size_t msglen, char *response, size_t responselen)
{
	char host_ip[16];
	int nPort, s;
	int ret = 0;
	int nReceived = 0;
	int nTotalReceived = 0;

	struct sockaddr_in server_addr;

	char *buffer;
	char pucData[512]; 		//buffer de recepção de 512 bytes

	PegaIPServidor(host_ip, FALSE);
	PegaPortaServidor(&nPort);

	struct timeval tv;
	tv.tv_sec = 30;			//timeout de 30 segundos
	tv.tv_usec = 0;

	memset(response, 0, responselen);

	buffer = umalloc(responselen * sizeof(char));

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s != -1)
	{
		memset( &server_addr, 0, sizeof( server_addr ));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = __inet_addr(host_ip);
		server_addr.sin_port = htons(nPort);

		//configura timeouts de envio e recebimento
		setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
		setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

		if (connect(s, (struct sockaddr *)&server_addr,  sizeof(server_addr)) == 0)
		{
			//envia mensagem ao servidor
			ret = send(s, msg, msglen, 0);

			if (ret > 0)
			{
				//recebe o retorno do servidor em partes (no tamanho máximo de pucData)
				//para evitar bloqueio
				do
				{
					memset(pucData, 0, sizeof(pucData));

					nReceived = recv(s, pucData, sizeof(pucData), 0);

					if (nReceived > 0)
					{
						nTotalReceived += nReceived;
						strncat(buffer, pucData, nReceived);
					}
				} while (nReceived > 0);

				if (nReceived == 0)			//recepção OK
				{
					strncpy(response, buffer, nTotalReceived);
					ret = nTotalReceived;
				}
				else if (nReceived < 0) 	//recepção com falha
				{
					ret = nReceived;
				}
			}
		}
	}
	else
		ret = -1;

	ufree(buffer);

	closesocket(s);

	return ret;
}

/*
 * Envia uma mensagem socket e armazena a resposta do servidor em buffer
 * com exibição de telas de conexão
 * - msg (entrada): mensagem a ser enviada ao servidor
 * - msglen(entrada): tamanho da mensagem
 * - buffer (saída): buffer de resposta
 * - bufferlen (entrada), tamanho do buffer
 * retorna a quantidade de bytes recebidos ou -1 para erro de conexão
 */
int EnviaSocket(char *msg, size_t msglen, char *response, size_t responselen)
{
	int tentativas = 3;
	int i, ret, prec;
	char str[15];

	prec = StateHeader(1);

	for (i = 0; i < tentativas; i++)
	{
		if (i == 0)
		{
			DisplaySimpleMessage("Conectando...", TRUE, TRUE);
		}
		else
		{
			memset(str, 0, sizeof(str));
			sprintf (str, "Tentativa %d", i + 1);
			DisplaySimpleMessage2lines("Conectando...", str, TRUE, TRUE);
		}

		if (i == 0)
		{
			ret = _EnviaSocket(msg, msglen, response, responselen);
		}
		else
		{
			ret = _EnviaSocket(msg, msglen, response, responselen);
		}

		if (ret > 0)
			break;
		else
			ret = -1;

		ttestall(0, 200);
	}

	StateHeader(prec);

	return ret;
}

/*
 * Teste de conexão com o servidor da Opus
 */
void TesteConexao()
{
	FILE *hPrinter;
	char resp[10];
	int i;

	char msg[] = "4636teste";

	hPrinter  = fopen( "PRINTER",  "w" );
	pprintf("\x1b""F");
	pprintf("**** Dados do Teste ****\n\n");

	i = EnviaSocket(msg, strlen(msg), resp, sizeof(resp));

	if (i > 0)
	{
		if (strcmp(resp, "OK") == 0)
		{
			pprintf("Host Opus: OK\n\n");
		}
		else
		{
			pprintf("Host Opus: Falha\n\n");
		}
	}
	else
	{
		pprintf("Host Opus: Falha\n\n");
	}

	pprintf("************************\n");
	pprintf("\n\n\n\n\n\n");

	ttestall(PRINTER, 0);
	fclose(hPrinter);
}
