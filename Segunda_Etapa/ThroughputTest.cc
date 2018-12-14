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
#include <vector>

Define_Module(ThroughputTest);
int verificador[55];
int verificador2[55];
std::vector<int> vizinhanca[10];
std::vector<double> janela_no[55];
std::vector<double> pacotes_no[55];
int deslize_janela[55] = {0};
int gambiarra = 0;
int contador_no_defeituoso[55];



double ThroughputTest::similaridade_cosseno(vector<double> A,vector<double> B, int size ){
	
	
	
	double mul = 0.0, d_a = 0.0, d_b = 0.0 ;

    for(unsigned int i = 0; i < size; ++i) 
    {
        mul += A[i] * B[i] ;
        d_a += A[i] * A[i] ;
        d_b += B[i] * B[i] ;
    }
    
    
    
    
    
    
    
    return mul / (sqrt(d_a) * sqrt(d_b)) ;

	
	}

double ThroughputTest::mediana(vector<double> janela, int contagem){
	double vetor2[contagem];
	double mediana;
	for (int i = 0; i < contagem; i++){
		
		vetor2[i] = janela[i]; 
	}
		
		
	sort(vetor2, vetor2+contagem);
	
	if (contagem % 2 == 0){
			 mediana = ((vetor2[contagem/2] + vetor2[contagem/2 - 1])/2.0);
	}else {
			 mediana = vetor2[contagem/2];
	}
	 
	 
	 return mediana;
	
	}
	
//Função que calcula o MAD 	
double ThroughputTest::median_deviation_absolute(vector<double> janela, double median, int contagem){
	
	 double *vetor2 = new double[contagem];
	 double MAD;
	 for(int i = 0; i < contagem; i++){
		 vetor2[i] = abs((janela[i] - median));
		}	  
			 
	
		   
		   
			sort(vetor2, vetor2+contagem); 
			
				
			
				
			
			if (contagem % 2 == 0){
						MAD = ((vetor2[contagem/2] + vetor2[contagem/2 - 1])/2.0);
			}else {
						MAD = vetor2[contagem/2];
			}
	 
	 
            
            
           return MAD;
	}
	
float ThroughputTest::confianca_sensor(vector<double> janela, int indice){
	
	
	
	double ZS_modificado;
	int tamanho = janela.size();
	double median = mediana(janela, tamanho);    
	double MAD = median_deviation_absolute(janela, median, tamanho);                
    float quantidade_anomalias = 0;
	float confianca;
	 
	 for (int i= 0; i < janela.size(); i++){
		
		if (MAD == 0){
			MAD == 2.2250738585072014e-308;
			ZS_modificado = abs((0.6745*(janela[i] - median))/MAD);
			}
		
		else{
			
			ZS_modificado = abs((0.6745*(janela[i] - median))/MAD);
			
			}
		
		
		if (ZS_modificado > 3.5){
			
			 //trace() << "Anomalia: " << janela[i] << " " << "Nó: " << no << " " << "Pacote: " << pacotes[i]  ;
			 quantidade_anomalias++;
			
			}
		
		
		
		}
	                     
		

	if (quantidade_anomalias > 0){ 
     
     
     
     confianca = (1 - quantidade_anomalias/tamanho_janela[1])*100;
     
     
     }
     
     
     else{
		 confianca = 100;
		 }
	
	
	return confianca;
	
	
	}

void ThroughputTest::calculo(vector<int> vect){
	

	
	 int tamanho_vizinhanca = vect.size();
	 double taxa_anomalias[tamanho_vizinhanca][2];
	 int contador = 0;
	 for (int i = 0; i < vect.size(); i++){
          int indice = vect[i];
        
          float  confianca = confianca_sensor(janela_no[indice], indice);
          trace() << "Confiança: " << confianca << " " << "Nó: " << indice;
          taxa_anomalias[contador][0] = indice;
          taxa_anomalias[contador][1] = confianca;
		  contador++;
         }     
					
	
		
	  vector<double> vetor;
      
      
      for (int i = 0; i < tamanho_vizinhanca; i++){
		
		 vetor.push_back(taxa_anomalias[i][1]);
		 
		}
		
		 
		
	   
     
     
      
     
	  for (int i= 0; i < tamanho_vizinhanca; i++){
		  
		  if(taxa_anomalias[i][1] < 80){
		  
		  trace() << "No anômalo pela grandeza: " << taxa_anomalias[i][0];
		
			
			}else{
				
				
				trace() << "Nó NÃO anômalo pela grandeza: " << taxa_anomalias[i][0];
				
				}
		
		
		
		}
		
		
		
 //Similaridade -------------------------------------------------------------------------------------------------------
          
        
        for (int i = 0; i < vect.size(); i++){
			int quantidade_similaridade = 0;
				int ind = vect[i];
				for (int h = 0; h < vect.size(); h++){
			 
					if (ind != vect[h]){
						
						double similaridade = similaridade_cosseno(janela_no[ind],janela_no[vect[h]], janela_no[ind].size());
						trace() << "Similaridade do Nó: " << ind << " " << "Com o Nó: " <<  vect[h] << " " << "Resultado: " << similaridade;
						if (similaridade < 0.7){

							quantidade_similaridade++;
							}

						}
			 
			 }
			int tamanho_vetor = vect.size();
			
			
			if (quantidade_similaridade == tamanho_vetor-1){

					trace() << "Nó defeituoso Pela Similaridade: " << ind;
						
						
						}
			else{
				
				trace() << "Nó NÃO defeituoso Pela Similaridade: " << ind;
				
				}
         
			  

				}
				
				 
		
		
		
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
		tempo_janela = new int[55];
		for(int i = 0; i < 55; i++)
			tempo_janela[i] = par("tempo_janela");
	}
	else {
		tamanho_janela = new int[55];
		for(int i = 0; i < 55; i++)
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
		if (tipo_janela == 1){
		
				
				if (janela_no[index].size() < tamanho_janela[index]){
					
					if (janela_no[index].size() == 0){
						
						janela_no[index].push_back(data[3]);
						pacotes_no[index].push_back(data[6]);
			
						}
				
					if (janela_no[index].size() > 0){
						int numero_no = janela_no[index].size();
						int teste = numero_no - 1;
						
						if (pacotes_no[index][teste] != data[6]){
							janela_no[index].push_back(data[3]);
							pacotes_no[index].push_back(data[6]);
						
						
						
							} 
					
						}
                }

            if(janela_no[index].size() == tamanho_janela[index] and contador[index] > 0){

				
				for (int i = 0; i<tamanho_janela[index]; i++){
						int numero_no = janela_no[index].size();
                        if (i != tamanho_janela[index] - 1 ){
                                      
                                janela_no[index][i] = janela_no[index][i+1]; 
                                pacotes_no[index][i] = pacotes_no[index][i+1];   
      
                                  }
      
                        else if (pacotes_no[index][numero_no-1] != data[6] and i == tamanho_janela[index] - 1 ){
                              
                               janela_no[index][i] = data[3];
                               pacotes_no[index][i] = data[6];
                                                  
                                                    }
    
                         }
                   
                  
				deslize_janela[index]++;
					
					for (int j = 0; j < 10; j++){
      
						for (int i = 0; i < vizinhanca[j].size(); i++){
          
							int indice = vizinhanca[j][i];
						//	trace() << "Deslize: " << deslize_janela[indice] << " " << "Indice: " << indice;
							if (janela_no[indice].size() == tamanho_janela[index] and  deslize_janela[indice] == 1){
							//		trace() << "indice entrou depois" << indice;
									verificador2[j]++;
									deslize_janela[indice]++;
									
										}
            

								}
									
									
									if (verificador2[j] == vizinhanca[j].size()){
									//		trace() << "chamou pela segunda vez";
											calculo(vizinhanca[j]); 
											verificador2[j] = 0;
											for (int i = 0; i < vizinhanca[j].size(); i++){
												
												int indice_deslize = vizinhanca[j][i];
												
												deslize_janela[indice_deslize] = 0;
												
												
												}
										}
								}
						
				
				}
				
				         
			if(janela_no[index].size() == tamanho_janela[index] and contador[index] == 0){
					
					
					for (int j = 0; j < 10; j++){
							
						for (int i = 0; i < vizinhanca[j].size(); i++){
							
							int indice = vizinhanca[j][i];
						
							if (janela_no[indice].size() == tamanho_janela[index] and contador[indice] == 0){
								//trace() << "indice entrou" << indice;
									verificador[j]++;
									
										}
            

								}
									
									
									if (verificador[j] == vizinhanca[j].size()){
										//	trace() << "chamou pela primeira vez";
											calculo(vizinhanca[j]); 
											verificador[j] = 0;
          
										}
								}
					
							
							contador[index]++;
					
					
            }
            
	
                
		packets_received++;
		
	} 
    
  }


/*if (addr.compare("55") == 0){
	
	  
	  //trace() << "Sending packet do nó 1: " << dataSN;
		packets_sent++; 
		DataPacket* data_packet = createDataPacket(data, dataSN, 80);
		toNetworkLayer(data_packet, (const char*)("0"));
		dataSN++;     
	  
	  }*/


}




   //******************************************************************************************************************************




void ThroughputTest::timerFiredCallback(int index)
{
	
	if (gambiarra == 0){
	//------ Vizinhanca 1 ------------ ///	
	vizinhanca[0].push_back(48);
	vizinhanca[0].push_back(49);
	vizinhanca[0].push_back(50);
	vizinhanca[0].push_back(51);
	vizinhanca[0].push_back(52);
	
	//------ Vizinhanca 2 ------------ ///
	vizinhanca[1].push_back(7);
	vizinhanca[1].push_back(8);
	vizinhanca[1].push_back(9);
	vizinhanca[1].push_back(10);
	vizinhanca[1].push_back(11);
	vizinhanca[1].push_back(53);
	vizinhanca[1].push_back(54);
	
	//------ Vizinhanca 3 ------------ ///
	vizinhanca[2].push_back(12);
	vizinhanca[2].push_back(13);
	vizinhanca[2].push_back(14);
	vizinhanca[2].push_back(15);
	vizinhanca[2].push_back(16);
	
	//------ Vizinhanca 4 ------------ ///
	vizinhanca[3].push_back(44);
	vizinhanca[3].push_back(45);
	vizinhanca[3].push_back(46);
	vizinhanca[3].push_back(47);
	
	//------ Vizinhanca 5 ------------ ///
	vizinhanca[4].push_back(2);
	vizinhanca[4].push_back(3);
	vizinhanca[4].push_back(4);
	vizinhanca[4].push_back(5);
	vizinhanca[4].push_back(6);
	
	//------ Vizinhanca 6 ------------ ///
	vizinhanca[5].push_back(17);
	vizinhanca[5].push_back(18);
	vizinhanca[5].push_back(19);
	vizinhanca[5].push_back(20);
	vizinhanca[5].push_back(21);
	
	//------ Vizinhanca 7 ------------ ///
	vizinhanca[6].push_back(38);
	vizinhanca[6].push_back(39);
	vizinhanca[6].push_back(40);
	vizinhanca[6].push_back(41);
	vizinhanca[6].push_back(42);
	vizinhanca[6].push_back(43);
	
    //------ Vizinhanca 8 ------------ ///
	vizinhanca[7].push_back(1);
	vizinhanca[7].push_back(33);
	vizinhanca[7].push_back(34);
	vizinhanca[7].push_back(35);
	vizinhanca[7].push_back(36);
	vizinhanca[7].push_back(37);
	
	//------ Vizinhanca 9 ------------ ///
	vizinhanca[8].push_back(28);
	vizinhanca[8].push_back(29);
	vizinhanca[8].push_back(30);
	vizinhanca[8].push_back(31);
	vizinhanca[8].push_back(32);
	
	//------ Vizinhanca 10 ------------ ///
	vizinhanca[9].push_back(22);
	vizinhanca[9].push_back(23);
	vizinhanca[9].push_back(24);
	vizinhanca[9].push_back(25);
	vizinhanca[9].push_back(26);
	vizinhanca[9].push_back(27);
	
	gambiarra++;
	
	}
	
	int no_defeituoso = stoi(SELF_NETWORK_ADDRESS);

	
	switch (index){
		case SEND_PACKET: {
			 
			string addr(SELF_NETWORK_ADDRESS);
			
			
			//is an END NODE
			if (addr.compare("0") != 0) {
				
				int nodeId = stoi(SELF_NETWORK_ADDRESS);
				
				string arquivo = "/home/miqueas/Dados/Log_" + addr + ".txt";
				trace() << "Sending packet (nó"<< " " << addr << ")" << dataSN;
				
				packets_sent++;
                FILE *file;
                file = fopen(arquivo.c_str(),"r");
                double *data = new double[10];
               
              
                int leitura, node;
                double temperatura, umidade;
                char frase[100];
				
		        while(fgets(frase,100,file) != NULL)	 { 
							
					
							 fscanf(file,"%i,%i,%lf,%lf", &leitura, &node, &temperatura, &umidade);
		                     if (leitura == contadorleitura[nodeId]){
									
                                //     trace()<< "entrou aqui dentro do leitura";
                                     data[0] = 0;
				                     data[1] = prr_beacon_end;
				                     data[2] = RNP;
                                     data[3] = temperatura;
                                     data[4] = umidade;
                                     data[5] = node;
                                     data[6] = leitura;
                                     
									
                                    DataPacket* data_packet = createDataPacket(data, dataSN, 80);
                               
									
                                    toNetworkLayer(data_packet, (const char*)("0"));
                                    dataSN++;
                                    cont_tx++;
									contadorleitura[nodeId]++;
									
									
									
									
									
									break;
									
                                    
								}
							
						
                             
	                 }

					 
					
					
					fclose(file);
					
					
					
					setTimer(SEND_PACKET, packet_spacing);
				
				
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
