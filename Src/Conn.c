#include "SDK30.H"
#include "IP_.h"

#include "Globals.h"
#include "Dialogs.h"
#include "Sistema.h"

extern struct Confs config;

bool Validate_ip4(char *buffer)
{
    if (NULL == buffer) return FALSE;

    register const char *pos     	= buffer;
    register unsigned char ch      	= *pos;
    register unsigned short count   = 0;

    while (ch != NULL)
    {
        if (!((ch >= '0' && ch <= '9') || ch == '.')) return FALSE;

        if (ch == '.')
            if (++count > 3) return FALSE;

        ch = *++pos;
    }

    if (count == 3 && *--pos != '.')
    	if (__inet_addr(buffer) != -1) return TRUE;

    return FALSE;
}

int StatusDHCP()
{
	int nDHCP;
	int ret = 0;

	int nError = __DFL_OK;
	DFLCONF_HANDLE hConf = __LoadDefaultOptions( &nError );

	if( ( hConf != NULL ) && ( nError == __DFL_OK ) )
	{
		if( __GetDefaultOption( hConf, __DFL_ETH_BOOT_PROTOCOL, &nDHCP ) == __DFL_OK )
		{
			 ret = nDHCP;
		}
	}

	__UnloadDefaultOptions( hConf );

	return ret;
}

void PrintNetworkInfos()
{
	char szTemp[128];
	FILE *hPrinter;

	unsigned char *p;

	unsigned int local_addr = 0;
	unsigned int netmask_addr = 0;
	unsigned int gateway_addr = 0;
	unsigned int dns1_addr = 0;
	unsigned int dns2_addr = 0;
	int nDHCP;

	//dados terminal
	char serial1[65];
	byte identif[65];
	char modelo[20];
	char iccid[22];

	hPrinter  = fopen( "PRINTER",  "w" );

	pprintf("\x1B""E");
	pprintf("   Informacoes Gerais\n\n");
	pprintf("\x1b""F");

	pprintf("\x0F");

	//-----

	pprintf("\n*** Dados Terminal ***\n");
	pprintf("Cliente Vinculado: %s\n", config.cfCod);
	pprintf("Nome Fantasia: %s\n", config.cfNomeFantasia);
	memset(serial1, 0, sizeof(serial1));
	PegarSerial(serial1);
	pprintf("Serial: %s\n", serial1);
	memset(identif, 0, sizeof(identif));
	PSQ_Give_Full_Serial_Number(identif);
	pprintf("Serial completo: %s\n", identif);
	memset(modelo, 0, sizeof(modelo));
	PegarModelo(modelo);
	pprintf("Modelo: %s\n", modelo);
	memset(iccid, 0, sizeof(iccid));
	PegarICCID(iccid);
	pprintf("ICCID: %s\n", iccid);

	//-----

	pprintf("\n*** Dados da Rede ***\n");
	sprintf( szTemp, "Tipo de Conexao: %s", config.cfNetwork );
	pprintf("%s\n", szTemp);
	sprintf( szTemp, "APN: %s", config.cfApn);
	pprintf("%s\n", szTemp);

	EthernetGetOption( ETH_IFO_ADDR, &local_addr );
	EthernetGetOption( ETH_IFO_NETMASK, &netmask_addr );
	EthernetGetOption( ETH_IFO_GATEWAY, &gateway_addr );
	EthernetGetOption( ETH_IFO_DNS1, &dns1_addr );
	EthernetGetOption( ETH_IFO_DNS2, &dns2_addr );

	nDHCP = StatusDHCP();

	if (nDHCP == __DFL_ETH_BOOT_PROTOCOL_DHCP)
		strcpy( szTemp, "DHCP: Ativado");
	else if (nDHCP == __DFL_ETH_BOOT_PROTOCOL_NONE)
		strcpy( szTemp, "DHCP: Desativado");
	else
		sprintf( szTemp, "DHCP: %s", config.cfDhcpmode );

	pprintf("%s\n", szTemp);

	p = (unsigned char *) &local_addr;
	sprintf( szTemp, "IP: %d.%d.%d.%d", p[0], p[1], p[2], p[3] );
	pprintf("%s\n", szTemp);

	p = (unsigned char *) &netmask_addr;
	sprintf( szTemp, "Netmask: %d.%d.%d.%d", p[0], p[1], p[2], p[3] );
	pprintf("%s\n", szTemp);

	p = (unsigned char *) &gateway_addr;
	sprintf( szTemp, "Gateway: %d.%d.%d.%d", p[0], p[1], p[2], p[3] );
	pprintf("%s\n", szTemp);

	p = (unsigned char *) &dns1_addr;
	sprintf( szTemp, "DNS 1: %d.%d.%d.%d", p[0], p[1], p[2], p[3] );
	pprintf("%s\n", szTemp);

	p = (unsigned char *) &dns2_addr;
	sprintf( szTemp, "DNS 2: %d.%d.%d.%d", p[0], p[1], p[2], p[3] );
	pprintf("%s\n", szTemp);

	//-----
	pprintf("\x12");

	pprintf("\x1B""E");
	pprintf("************************\n\n");
	pprintf("\x1b""F");
	pprintf("\n\n\n");

	ttestall(PRINTER, 0);
	fclose(hPrinter);
}


// qualquer valor diferente de 0 ativa DHCP, 0 = desativa DHCP
void AtivarDHCP(int bOn)
 {
	int bDHCP, nDHCP;

	int nError = __DFL_OK;
	DFLCONF_HANDLE hConf = __LoadDefaultOptions( &nError );

	if (bOn==FALSE)
		bDHCP = __DFL_ETH_BOOT_PROTOCOL_NONE;
	else
		bDHCP = __DFL_ETH_BOOT_PROTOCOL_DHCP;

	if( ( hConf != NULL ) && ( nError == __DFL_OK ) )
	{
		if( __GetDefaultOption( hConf, __DFL_ETH_BOOT_PROTOCOL, &nDHCP ) == __DFL_OK )
		{
			 if( nDHCP != bDHCP )
			 {
				 __SetDefaultOption( hConf, __DFL_ETH_BOOT_PROTOCOL, &bDHCP );
				 __SaveDefaultOptions( hConf );
			 }
		}
	}

	__UnloadDefaultOptions( hConf );
}

void AbrirEthernetManual()
{
	//definir ip, netmask, gateway, dns1 e dns2

	doubleword uiLocalAddr = __inet_addr(config.cfIp);
	doubleword uiNetmask = __inet_addr(config.cfNetmask);
	doubleword uiGateway = __inet_addr(config.cfGateway);
	doubleword uiDNS1 = __inet_addr(config.cfDNS1);
	doubleword uiDNS2 = __inet_addr(config.cfDNS2);

	EthernetSetOption(ETH_IFO_ADDR, &uiLocalAddr);
	EthernetSetOption(ETH_IFO_NETMASK, &uiNetmask);
	EthernetSetOption(ETH_IFO_GATEWAY, &uiGateway);
	EthernetSetOption(ETH_IFO_DNS1, &uiDNS1);
	EthernetSetOption(ETH_IFO_DNS2, &uiDNS2);
}

/*
 * 	Pega Endereço IP do Servidor
 * 	Params:
 * 	 - ip = saída do ip correto
 * 	 - resolucaoDNS = se TRUE faz resolução DNS do Domínio
 * 	 Retorna 0 se OK, -1 de o ip for incorreto
 */
int PegaIPServidor(char *ip, bool resolucaoDNS)
{
	int ret = 0;
	char dns_ip[16];

	if (strlen(config.cfServIP) > 0)
	{
		if (!resolucaoDNS) {
			strcpy(ip, config.cfServIP);
		}
		else
		{
			if (strlen(config.cfServDominio) > 0)
			{
				if (DNS_GetIpAddress(config.cfServDominio, dns_ip, sizeof(dns_ip)) == DNS_OK )
				{
					if (Validate_ip4(dns_ip))
					{
						strcpy(config.cfServIP, dns_ip);
						strcpy(ip, dns_ip);
					}
					else
					{
						strcpy(ip, config.cfServIP);
						ret = -2;
					}
				}
				else
				{
					strcpy(ip, config.cfServIP);
					ret = -1;
				}
			}
			else
			{
				strcpy(ip, config.cfServIP);
			}
		}
	}
	else
		ret = -2;

	return ret;
}

void PegaPortaServidor(int *nPort)
{
	int Porta = 0;

	Porta = atoi(config.cfServPorta);

	if (Porta == 0)
		Porta = PORTA_PADRAO;

	*nPort = Porta;
}
