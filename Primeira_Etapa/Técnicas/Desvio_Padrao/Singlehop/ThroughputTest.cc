/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "ThroughputTest.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <simtime.h>




Define_Module(ThroughputTest);

int contadorleitura[54] = {1};
int contador[54] = {0};
int numero[54] = {0};
double janela[54][1000];
int pacotes[54][1000];
double tempo_atual;
double timer;
double tempo_inicial[5];
int cont_pacotes[5] = {0};

double ThroughputTest::Desvio_Padrao(double janela[], int contagem){
	
	double soma = 0.0, media, desvio_padrao = 0.0;
	
    int i;

    for(i = 0; i < contagem; ++i)
    {
        soma += janela[i];
    }

    media = soma/contagem;

    for(i = 0; i < contagem; ++i)
        desvio_padrao += pow(janela[i] - media, 2);

    return sqrt(desvio_padrao/contagem);
	
	
	}

void ThroughputTest::calculo(double janela[], int pacotes[], int no, int contagem){
	 
	 double media, soma = 0, limite_superior, limite_inferior, desvio_padrao, z_score;
	 int i;
	 
	 desvio_padrao = Desvio_Padrao(janela,contagem);
	
	 for(i = 0; i < contagem; ++i){
        soma += janela[i];
		}
		
		
	 media = soma/contagem;
	 
	
	 
	limite_superior = media + 3*desvio_padrao;
	limite_inferior = media - 3*desvio_padrao;
	 // se os valores da janela estiverem fora dos limites, entao o dado é anômalo
	 		 trace() << "Limite Superior: " << limite_superior << " " << "Limite Inferior: " << limite_inferior;

	 for (i = 0; i<contagem;i++){
		 
		 trace() << "Janela: " << janela[i] << " " << "Pacote: " << pacotes[i] << " " << "Nó: " << no;
		 
		 } 
	 for (i = 0; i<contagem;i++){
			if (janela[i] > limite_superior || janela[i] < limite_inferior){
               trace() << "Anomalia: " << janela[i] << " " << "Nó: " << no << " " << "Pacote: " << pacotes[i]  ;
                    
		                     } }
			
	                        
		


          }
                        
		

		                  



void ThroughputTest::startup()
{
//	srand(time(NULL));
	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	startupDelay = par("startupDelay");
	tipo_janela = par("tipo_janela"); // 0 -> tempo fixo  1-> numero de amostras fixo
	num_nodes = par("num_nodes");
	
	if(tipo_janela == 0) {
		tempo_janela = new int[7];
		for(int i = 0; i < 5; i++)
			tempo_janela[i] = par("tempo_janela");
	}
	else {
		tamanho_janela = new int[7];
		for(int i = 0; i < 5; i++)
			tamanho_janela[i] = par("tamanho_janela");
	}
	
	double startupDelay_rand = 0;//rand()%10;
	
	packet_spacing = packet_rate > 0 ? 5.0 / float (packet_rate) : -1;
	dataSN = 1;
	

	
	int nodeId = abs(recipientAddress.compare(SELF_NETWORK_ADDRESS));
	

	
	tempo_inicial[nodeId] =  simTime().dbl();
	
	

    

	if (packet_spacing > 0) {
		setTimer(SEND_PACKET, packet_spacing + startupDelay_rand + 1.5);
	}
	else
		// Diego
		trace() << "Not sending packets";
	
	
	

	declareOutput("Packets received per node");
	for(int i = 0; i < 5; i++)new_value_flqe[i]=false;
}


void ThroughputTest::fromNetworkLayer(DataPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();
	double* data = rcvPacket->getDataPacket();
	
   

	//SINK NODE
	string addr(SELF_NETWORK_ADDRESS);
	if (addr.compare("0") == 0) {
		int index = data[5];
		
		if (tipo_janela == 0){
			//trace() << "Index" << index;
			if((simTime().dbl() - tempo_inicial[index]) >= tempo_janela[index]){
				
				
				
				int contagem = cont_pacotes[index];
				calculo(janela[index], pacotes[index], index, contagem);
				cont_pacotes[index] = 0;
				tempo_inicial[index] = simTime().dbl();
				
				
				}else{
					
					if (cont_pacotes[index] == 0){
					janela[index][cont_pacotes[index]] = data[3];
					pacotes[index][cont_pacotes[index]] = data[6];
					cont_pacotes[index]++;
					}
					
					if (cont_pacotes[index] != 0 and pacotes[index][cont_pacotes[index]-1] != data[6]){
						janela[index][cont_pacotes[index]] = data[3];
						pacotes[index][cont_pacotes[index]] = data[6];
						cont_pacotes[index]++;
						
						}
					
					
					}
			
			
			
			
			
			}
		
		
		
		
		
		
		
		
		
		
		
		
		if (tipo_janela == 1){
		
		
			if (numero[index] < tamanho_janela[index]){
				if (numero[index] == 0){
					janela[index][numero[index]] = data[3];
					pacotes[index][numero[index]] = data[6];
				
					
					numero[index]++; 
					
					}
				
			if (numero[index] > 0){
				if (pacotes[index][numero[index]-1] < data[6]){
					janela[index][numero[index]] = data[3];
					pacotes[index][numero[index]] = data[6];
					numero[index]++; 
						
						
						} 
					
					}
                }
                
                
          
         
            if(numero[index] == tamanho_janela[index] && contador[index] > 0){
				
			
				
				
				for (int i = 0; i<tamanho_janela[index]; i++){
                     
                        if (i != tamanho_janela[index] - 1 ){
                                      
                                janela[index][i] = janela[index][i+1]; 
                                pacotes[index][i] = pacotes[index][i+1];   
      
                                  }
      
                        else if (pacotes[index][numero[index]-1] < sequenceNumber and i == tamanho_janela[index] - 1 ){
							   trace() << "Aqui";
                               janela[index][i] = data[3];
                               pacotes[index][i] = data[6];
                                                  
                                                    }
    
                         }
                   
				
					calculo(janela[index], pacotes[index], index, numero[index]); 	
				
				} 
				
				         
			if(numero[index] == tamanho_janela[index] && contador[index] == 0){
		   
					calculo(janela[index], pacotes[index], index, numero[index]); 
					contador[index]++;
					//numero[index] = 0;

            }
	
                 	
		packets_received++;
		
	} 
    
  }

	if (addr.compare("5") == 0){
		
		//trace() << "Sending packet do nó 1: " << dataSN;
		packets_sent++; 
		DataPacket* data_packet = createDataPacket(data, dataSN, 80);
		toNetworkLayer(data_packet, (const char*)("0"));
		dataSN++;     
} 
	
	
	if (addr.compare("6") == 0){
		
//trace() << "Sending packet do nó 2: " << dataSN;
		packets_sent++; 
		DataPacket* data_packet = createDataPacket(data, dataSN, 80);
		toNetworkLayer(data_packet, (const char*)("0"));
		dataSN++;     
} 


}
















   //******************************************************************************************************************************






void ThroughputTest::timerFiredCallback(int index)
{
	
	switch (index){
		case SEND_PACKET: {
			 
			string addr(SELF_NETWORK_ADDRESS);
			
			int num;
			//is an END NODE
			if (addr.compare("0") != 0 && addr.compare("5") != 0 && addr.compare("6") != 0) {
				string arquivo;
				int no = abs(recipientAddress.compare(SELF_NETWORK_ADDRESS));
				
				
				
				if (addr.compare("1") == 0) {
					arquivo = "/home/miqueas/Dados_Encom/SingOut.txt";
					}
				
				if (addr.compare("2") == 0){
					
					arquivo = "/home/miqueas/Dados_Encom/SingIn.txt";
					
					}
				
				if (addr.compare("3") == 0){
					
					arquivo = "/home/miqueas/Dados_Encom/SingOut2.txt";
					
					}
					
				if (addr.compare("4") == 0){
					
					arquivo = "/home/miqueas/Dados_Encom/SingIn2.txt";
					
					}
				
				trace() << "Sending packet (nó"<< " " << addr << ")" << " " <<  dataSN;
				
				packets_sent++;
                FILE *file;
                file = fopen(arquivo.c_str(),"r");
               
                
                double *data = new double[10];
                int leitura, ano, mes, dia, hora, minuto, epoca, node;
                
                double segundo,temperatura, umidade;
                char frase[100];
				
		        while(fgets(frase,100,file) != NULL)	 { 
							
					
                            //fscanf(file,"%d,%d-%d-%d,%d:%d:%lf,%d, %d, %lf,%lf", &leitura, &ano, &mes, &dia, &hora, &minuto, &segundo, &epoca, &no, &temperatura, &umidade);
							 fscanf(file,"%i %i %lf %lf %lf", &leitura, &node, &umidade, &temperatura, &segundo);
		                     if (leitura == contadorleitura[no]+1){
									
                                //     trace()<< "entrou aqui dentro do leitura";
                                     data[0] = 0;
				                     data[1] = prr_beacon_end;
				                     data[2] = RNP;
                                     data[3] = umidade;
                                     data[4] = temperatura;
                                     data[5] = no;
                                     data[6] = leitura;
                                     
									
                                    DataPacket* data_packet = createDataPacket(data, dataSN, 80);
                               
									
                                    toNetworkLayer(data_packet, (const char*)("0"));
                                    dataSN++;
                                    cont_tx++;
									contadorleitura[no]++;
									
									
									
									
									
									break;
									
                                    
								}
							
						
                             
	                 }

					 
					
					
					fclose(file);
					
					std::random_device rd;
					//std::mt19937 gen(rd());
					std::poisson_distribution<> d(5);
					num = d(rd);
					
					setTimer(SEND_PACKET,num);
				
				
			    }

			
			  
 
			
			 
			   
			 
			
			
			
            
           
                    
			
            
		}
	}
}

void ThroughputTest::sendAck(int packet_id)
{
	//trace() << "Sending ack for packet " << packet_id;
    double *data = new double[10];
	data[0] = packet_id;
	DataPacket* data_packet = createDataPacket(data, ack_seq, 5);  
	toNetworkLayer(data_packet, (const char*)("1"));
	acks_sent++;
	//trace() << "Acks sent: " << acks_sent;
}


// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ThroughputTest::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}


