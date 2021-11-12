#include "SDK30.h"

#include "Globals.h"

const char dir[] = "/HOST";
const char inf_cfg_filename[] = "OPUSCFG.RES";
const char inf_prod_filename[] = "OPUSPROD.RES";

extern struct Confs config;
extern struct Produtos produto[MAX_PRODUTOS];

/*
 * Carrega as configurações do arquivo salvo
 * retorna 0 se OK, -1 se erro
 */
int CarregarConfigs()
{
	char file_path[128];
	char buffer[2048];  		//buffer máximo do arquivo 2KB
	char *token;
	char linha[50][200];		//máximo 50 linhas no arquivo, tamanho máx. da linha 200 bytes
	int ret;
	int numlinhas = 0;
	int i;
	long filelen = 0;
	S_FS_FILE *hFile;
	unsigned int nMode = FS_WRITEMANY;

	memset(file_path, 0, sizeof(file_path));
	memset(buffer, 0, sizeof(buffer));

	sprintf(file_path, "%s/%s", dir, inf_cfg_filename);

	ret = FS_mount(dir, &nMode);

	if (ret == FS_OK )
	{
		//lê o arquivo
		hFile = FS_open(file_path, "r");

		if (hFile != NULL)
		{
			filelen = FS_length(hFile);
			FS_read(buffer, 1, filelen, hFile);
			FS_close(hFile);

			buffer[filelen + 1] = '\0';

			//armazena as linhas em array
			token = strtok(buffer, "\r\n");
			while (token != NULL)
			{
				memset(linha[numlinhas], 0, sizeof(linha[numlinhas]));
				strcpy(linha[numlinhas], token);
				numlinhas++;

				token = strtok(NULL, "\r\n");
			}

			//coloca os valores das linhas nas variáveis globais
			for (i = 0; i <= numlinhas; i++)
			{
				token = strtok(linha[i], "=");

				if (token != NULL)
				{
					if (strcmp(token, "NETWORK")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfNetwork, 0, sizeof(config.cfNetwork));
						strcpy(config.cfNetwork, token);
					}
					else if (strcmp(token, "APN")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfApn, 0, sizeof(config.cfApn));
						strcpy(config.cfApn, token);
					}
					else if (strcmp(token, "APNLOGIN")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfApnlogin, 0, sizeof(config.cfApnlogin));
						strcpy(config.cfApnlogin, token);
					}
					else if (strcmp(token, "APNPWD")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfApnpwd, 0, sizeof(config.cfApnpwd));
						strcpy(config.cfApnpwd, token);
					}
					else if (strcmp(token, "DHCPMODE")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfDhcpmode, 0, sizeof(config.cfDhcpmode));
						strcpy(config.cfDhcpmode, token);
					}
					else if (strcmp(token, "IP")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfIp, 0, sizeof(config.cfIp));
						strcpy(config.cfIp, token);
					}
					else if (strcmp(token, "NETMASK")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfNetmask, 0, sizeof(config.cfNetmask));
						strcpy(config.cfNetmask, token);
					}
					else if (strcmp(token, "GATEWAY")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfGateway, 0, sizeof(config.cfGateway));
						strcpy(config.cfGateway, token);
					}
					else if (strcmp(token, "DNS1")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfDNS1, 0, sizeof(config.cfDNS1));
						strcpy(config.cfDNS1, token);
					}
					else if (strcmp(token, "DNS2")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfDNS2, 0, sizeof(config.cfDNS2));
						strcpy(config.cfDNS2, token);
					}
					else if (strcmp(token, "BACKLIGHT")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfBacklight, 0, sizeof(config.cfBacklight));
						strcpy(config.cfBacklight, token);
					}
					else if (strcmp(token, "SERV_DOMINIO")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfServDominio, 0, sizeof(config.cfServDominio));
						strcpy(config.cfServDominio, token);
					}
					else if (strcmp(token, "SERV_IP")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfServIP, 0, sizeof(config.cfServIP));
						strcpy(config.cfServIP, token);
					}
					else if (strcmp(token, "SERV_PORTA")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfServPorta, 0, sizeof(config.cfServPorta));
						strcpy(config.cfServPorta, token);
					}
					else if (strcmp(token, "REC_IP")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfRecIP, 0, sizeof(config.cfRecIP));
						strcpy(config.cfRecIP, token);
					}
					else if (strcmp(token, "REC_PORTA")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfRecPorta, 0, sizeof(config.cfRecPorta));
						strcpy(config.cfRecPorta, token);
					}
					else if (strcmp(token, "CLIENTE")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfCod, 0, sizeof(config.cfCod));
						strcpy(config.cfCod, token);
					}
					else if (strcmp(token, "NOME_FANT")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfNomeFantasia, 0, sizeof(config.cfNomeFantasia));
						strcpy(config.cfNomeFantasia, token);
					}
					else if (strcmp(token, "SENHA_DESB")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfSenhaDesb, 0, sizeof(config.cfSenhaDesb));
						strcpy(config.cfSenhaDesb, token);
					}
					else if (strcmp(token, "SENHA_MAGN")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfSenhaMagn, 0, sizeof(config.cfSenhaMagn));
						strcpy(config.cfSenhaMagn, token);
					}
					else if (strcmp(token, "ICCID")==0)
					{
						token = strtok(NULL, "=");
						memset(config.cfICCID, 0, sizeof(config.cfICCID));
						strcpy(config.cfICCID, token);
					}
				}
			}

			ret = 0;
		}
		else
			ret = -1;
	}
	else
		ret = -1;

	return ret;
}

int SalvarConfigs()
{
	unsigned int nMode = FS_WRITEMANY;
	S_FS_FILE *hFile;
	char file_path[128];
	char buffer[2048];  				//buffer máximo do arquivo 2KB
	char linha[200];					//tamanho máximo da linha 200 bytes
	int	ret = 0;

	char fmt[] = "%s=%s\r\n";

	memset(buffer, 0, sizeof(buffer));
	memset(linha, 0, sizeof(linha));
	memset(file_path, 0, sizeof(file_path));

	sprintf(file_path, "%s/%s", dir, inf_cfg_filename);

	if ( FS_mount( dir, &nMode ) == FS_OK )
	{
		//montar buffer do arquivo
		sprintf(linha, fmt, "NETWORK", config.cfNetwork);
		strcat(buffer,linha);
		sprintf(linha, fmt, "APN", config.cfApn);
		strcat(buffer,linha);
		sprintf(linha, fmt, "APNLOGIN", config.cfApnlogin);
		strcat(buffer,linha);
		sprintf(linha, fmt, "APNPWD", config.cfApnpwd);
		strcat(buffer,linha);
		sprintf(linha, fmt, "DHCPMODE", config.cfDhcpmode);
		strcat(buffer,linha);
		sprintf(linha, fmt, "IP", config.cfIp);
		strcat(buffer,linha);
		sprintf(linha, fmt, "NETMASK", config.cfNetmask);
		strcat(buffer,linha);
		sprintf(linha, fmt, "GATEWAY", config.cfGateway);
		strcat(buffer,linha);
		sprintf(linha, fmt, "DNS1", config.cfDNS1);
		strcat(buffer,linha);
		sprintf(linha, fmt, "DNS2", config.cfDNS2);
		strcat(buffer,linha);
		sprintf(linha, fmt, "BACKLIGHT", config.cfBacklight);
		strcat(buffer,linha);
		sprintf(linha, fmt, "SERV_DOMINIO", config.cfServDominio);
		strcat(buffer,linha);
		sprintf(linha, fmt, "SERV_IP", config.cfServIP);
		strcat(buffer,linha);
		sprintf(linha, fmt, "SERV_PORTA", config.cfServPorta);
		strcat(buffer,linha);
		sprintf(linha, fmt, "REC_IP", config.cfRecIP);
		strcat(buffer,linha);
		sprintf(linha, fmt, "REC_PORTA", config.cfRecPorta);
		strcat(buffer,linha);
		sprintf(linha, fmt, "CLIENTE", config.cfCod);
		strcat(buffer,linha);
		sprintf(linha, fmt, "NOME_FANT", config.cfNomeFantasia);
		strcat(buffer,linha);
		sprintf(linha, fmt, "SENHA_DESB", config.cfSenhaDesb);
		strcat(buffer,linha);
		sprintf(linha, fmt, "SENHA_MAGN", config.cfSenhaMagn);
		strcat(buffer,linha);
		sprintf(linha, fmt, "ICCID", config.cfICCID);
		strcat(buffer,linha);

		//deletar arquivo existente
		FS_unlink(file_path);

		//salvar o buffer em arquivo
		hFile = FS_open(file_path, "a");

		if (hFile != NULL)
		{
			ret = FS_write(buffer, strlen(buffer), 1, hFile);
			FS_close( hFile );
		}

		FS_unmount(dir);

		if (ret > 0) ret = TRUE;
	}

	return ret;
}

/*
 * Carrega as configurações do arquivo salvo
 * retorna 0 se OK, -1 se erro
 */
int CarregarProdutos()
{
	unsigned int nMode = FS_WRITEMANY;
	S_FS_FILE *hFile;
	char file_path[128];
	int ret;
	long filelen = 0;
	char buffer[2048];  				//buffer máximo do arquivo 2KB
	char *token;
	char linha[MAX_PRODUTOS][100];		//máximo 100 linhas no arquivo, tamanho máx. da linha 100 bytes
	int numlinhas = 0;
	int i;

	memset(file_path, 0, sizeof(file_path));

	sprintf(file_path, "%s/%s", dir, inf_prod_filename);

	if ( FS_mount(dir, &nMode) == FS_OK )
	{
		//lê o arquivo
		hFile = FS_open(file_path, "r");

		if (hFile != NULL)
		{
			filelen = FS_length(hFile);
			FS_read(buffer, 1, filelen, hFile);
			FS_close(hFile);

			buffer[filelen + 1] = '\0';

			//armazena as linhas em array
			memset(linha, 0, sizeof(linha));

			token = strtok(buffer, "\r\n");
			while (token != NULL)
			{
				strcpy(linha[numlinhas], token);
				numlinhas++;

				token = strtok(NULL, "\r\n");
			}

			//coloca os valores das linhas nas variáveis globais
			//converte as linhas em campos
			memset(produto, 0, sizeof(produto));

			for (i = 0; i <= numlinhas; i++)
			{
				token = strtok(linha[i], "|");

				if (token != NULL)
				{
					strcpy(produto[i].produto, token);
					token = strtok(NULL, "|");

					if (token != NULL)
					{
						strcpy(produto[i].valorface, token);
						token = strtok(NULL, "|");

						if (token != NULL)
						{
							strcpy(produto[i].operadora, token);
							token = strtok(NULL, "|");

							if (token != NULL)
							{
								strcpy(produto[i].rectipo, token);
								token = strtok(NULL, "|");

								if (token != NULL)
								{
									strcpy(produto[i].validade, token);
								}
							}
						}
					}
				}
			}

			ret = 0;
		}
		else
			ret = -1;
	}
	else
		ret = -1;

	return ret;
}

int SalvarProdutos()
{
	unsigned int nMode = FS_WRITEMANY;
	S_FS_FILE *hFile;
	char file_path[128];
	char *buffer;
	char linha[100];			//tamanho máximo da linha 100 bytes
	int	ret = 0;
	int i = 0;

	buffer = umalloc(2048 * sizeof(char)); 		//buffer máximo do arquivo 2KB

	if ( FS_mount( dir, &nMode ) == FS_OK )
	{
		//montar buffer do arquivo
		i = 0;

		while (i < MAX_PRODUTOS && strlen(produto[i].produto) != 0)
		{
			memset(linha, 0, sizeof(linha));

			sprintf(linha, "%s|%s|%s|%s|%s\r\n", produto[i].produto, produto[i].valorface,
					produto[i].operadora, produto[i].rectipo, produto[i].validade);

			strcat(buffer, linha);

			i++;
		}

		memset(file_path, 0, sizeof(file_path));
		sprintf(file_path, "%s/%s", dir, inf_prod_filename);

		//deletar arquivo existente
		FS_unlink(file_path);

		//salvar o buffer em arquivo
		hFile = FS_open(file_path, "a");

		if (hFile != NULL)
		{
			ret = FS_write(buffer, strlen(buffer), 1, hFile);
			FS_close( hFile );
		}

		FS_unmount(dir);

		if (ret > 0) ret = TRUE;
	}

	ufree(buffer);

	return ret;
}
