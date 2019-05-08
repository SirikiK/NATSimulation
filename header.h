#include <iostream>
#include <string>
#include <vector>

using namespace std ;


// Chaque ordinateur possede une adresse IP et un numero de port
struct Ordinateur
{
	int IP ;		// IP de l'ordinateur
	int PORT ;  	// Port de l'ordinateur
	
} ;

// Chaque messsage envoyé a une adresse de destination, une adresse d'expedition et un contenu
struct message
{
	Ordinateur Expediteur ; // Information sur l'expediteur du message (IP et Port)
	Ordinateur Destinateur ; // Informatuion du Destinateur (IP et Port)
	string Contenu ; // Pour la simulation On suppose que le message à envoyer est une chaine de caractère
} ;

// Une NAT possede une adresse publique et une table de correspondance pour les numeros de port 
struct NAT
{
	int NAT_IP ; 	// contient l'adresse public de la nat
	vector <int> NAT_Table ; 
};

// Teste si un indice est dejà rempli ou pas 
/*bool exist(vector <int> tab, size_t indice)
{
	if(indice < tab.size())
		return true ;

	else
		return false ;

}*/

bool Exist(vector <int> tab, int cle)
{
	size_t indice ;
	bool trouve = false ;

	for( size_t i=0; i<tab.size(); i++ && !trouve)
	{
		if(tab[i] == cle)
		{
			indice = i ;
			trouve = true ;
		}
	}

	return trouve ;
}

	// Surcharge pour afficher Les info sur un ordi.
ostream& operator << (ostream &e, const Ordinateur ordi) 
{
	e << "IP = " << ordi.IP << " Port = " << ordi.PORT  ;
	
	return e ;
}

class Simulation
{
	private: 
		Ordinateur *Internet ; // Resaux internet
		Ordinateur *ReseauPrivee ;	// Reseaux Local Privée
		NAT uneNAT ;					// Table d'adresse du NAT
		int K ; 					// Nombre d'ordinateur sur l'intenet
		int N ;						// Nombre d'ordinateur sur le Reseau Local
		int M ; 					// Nombre d'adresse IP publiques du NAT 
		int ordi_X ;						// pour le choix de l'utilisateur: ordiX sur l'internet
		int ordi_Y ;						// pour le choix de l'utilasateur: ordiY sur le reseaau Local
		unsigned short choix ;				// choix de la Direction d'envoi du message
		
	public:
		Simulation() ;
		
		void Accueil() ; // Pour l'initialisation des elements
		
		void Routeur(message unMessage) ; // Passage du message à traver un Routeur
		
		void Direction_Envoi() ;
		
		void afficher(message unmessage) ;
		
		// Envoyer un message d'un ordi Exp vers un ordi Dest 
		void Envoi(Ordinateur Exp, Ordinateur Dest, string mssge = "Hello Dear !!!" )
		{
			message unMessage ;
			// Encapsulation du message
			
			//Envoi du Local vers Internet (encapsulé avec l'adresse IP de la machine)
			unMessage.Expediteur = Exp ;
					
			if(choix == 1)	// Envoi de l'internet vers le reseau privée (encapsulé avec l'adresse public)
				Dest.IP = uneNAT.NAT_IP ;
						
			unMessage.Destinateur = Dest ;

			unMessage.Contenu = mssge ;
			
			cout << "Message Avant traitement par le NAT ! " <<endl << endl;

			afficher(unMessage) ; // Message avant passage par le routeur

			Routeur(unMessage) ;	// Passage à travers le routeur
			
		}
} ;

Simulation::Simulation()
{	
	cout << "Configuration des parametres de Simulation" <<endl << endl ;
		//K = N = 5 ; // cinq ordi sur chacune des reseaux
		M = N+1 ;

		cout << " Nombre d'ordinateur sur le l'Internet: ";  cin >> K ; cout<< endl ;

		Internet = new Ordinateur[K] ; 		// Creation du résaux Internet

		//Initialisation des adresses IP des ordinateurs sur Internet

			for(int i=0; i<K; i++)
			{	
				Internet[i].IP = i ; // IP = numero de l'ordinateur 
			} 
		
		cout << " Nombre d'ordinateur sur le Reseau Local : ";  cin >> N ; cout << endl;

		ReseauPrivee = new Ordinateur[N] ; // Creation du reseau Local avec N ordinateurs

			//Initialisation des adresses IP des ordinateurs sur le reseau Local

			for(int i=0; i<N; i++)
			{	
				ReseauPrivee[i].IP = i ; // IP = numero de l'ordinateur
			 
			} 
			
		// Initialisation des adresse IP du NAT	
		uneNAT.NAT_IP = 192 ;	// Adresse IP public de la NAT
	
			
	}
		
void Simulation :: Routeur(message unMessage) // Passage du message à travers un Routeur
{
	cout << "Traitement du message par le NAT ...... " << endl << endl ;
	
	if(choix == 2) // envoi LAN ---> Internet 
	{
		cout << "La NAT Reserve un numero de port Public a l'ordinateur du reseau Local: " <<endl << endl;

		// L'ordinateur poosede un numero de port Public sur la NAT
		uneNAT.NAT_Table.push_back(ReseauPrivee[ordi_Y].PORT) ;	 

		unMessage.Expediteur.IP = uneNAT.NAT_IP ;

		unMessage.Expediteur.PORT = uneNAT.NAT_Table.size() -1  ; // Nouvelle port reseve par la NAT 

		
		// L'ordi Y possede maintenant  une adresse Public 

		cout  << "L'ordinateur "<<ordi_Y << " possede sur l'INTERNET: IP = " << unMessage.Expediteur.IP 
			  << " PORT = " << unMessage.Expediteur.PORT << endl << endl;

		cout << "Encapsulation du message avec les nouveaux parametres attribue par la NAT...." << endl << endl;
		

		cout << "Message apres traitement par le NAT" << endl << endl;

		afficher(unMessage) ; 	// Envoi Reussi 
	}

	else if(choix == 1) // envoi: Internet ---> Local

	{
		cout << "Verification du port de destination ......" << endl ;

		// Si la case correspondant au numero de port de destination est rempli, donc le destinatueur a déjà communiquer sur l'internet 

		if( Exist(uneNAT.NAT_Table, unMessage.Destinateur.PORT) ) // On peut donc lui envoyer un message
		{
			unMessage.Destinateur.IP = ReseauPrivee[ordi_Y].IP ;

			
			cout << "L'ordinateur Y possede " << unMessage.Destinateur.IP 
				 << " " << unMessage.Destinateur.PORT 
				 <<" parametres prives" <<endl << endl; 

			cout << "Message apres traitement par le NAT." << endl << endl;

			afficher(unMessage) ; 	// Envoi Reussi
			
		}

		else
		{	// Le message ne peut pas être envoyer
			cout <<endl ;
			cout << "========================================================================" <<endl<<endl;

			cout << "Erreur !! L'ordinateur  de destination ne possede pas d'adresse Public. " <<endl << endl; 

			cout << "========================================================================" <<endl ;
		}
	}
		
}
	
void Simulation::Direction_Envoi()
{
	//unsigned short choix ; // choix sur la direction d'envoi du message (1 ou 2)

	// Menu
	cout << "_____________________________________________________________________" << endl << endl ;

	cout << "Dans quelle direction souhaitez-vous envoyer votre message ? : " << endl << endl ;
	cout << " 1:     Internet ---> Reseau Local " << endl <<endl;
	cout << " 2: Reseau Local ---> Internet " << endl ;
	cout << "______________________________________________________________________" << endl <<endl ;

	do
	{
		cout << "Votre choix : " ; cin >> choix ; cout << endl ;
		switch(choix)
		{
			case 1: 
				Envoi(Internet[ordi_X], ReseauPrivee[ordi_Y]) ; // Internet ---> Reseau Local
				break ;

			case 2: 
				Envoi(ReseauPrivee[ordi_Y], Internet[ordi_X]) ; // Reseau Local ---> Internet
				break ;

			default:
				cout << " choix non Valide! " << endl << endl ;
		}
	} while ((choix!= 1) && (choix != 2)) ;
}

void Simulation::afficher(message unmessage)
{
	cout << endl ;

	cout << "============================================================================" << endl ;
	cout << " De: " << unmessage.Expediteur << endl ;

	cout << " A : " << unmessage.Destinateur << endl ;

	cout << "  " << unmessage.Contenu << endl ;
	
	cout << "============================================================================" << endl <<endl ;
	
}

void Simulation::Accueil()
{
	char choix ;
	
	do{
			// Information Sur l'ordinateur X
			cout << "Choisissez un ordinateur X (entre 0 et " << K-1 <<") sur l'INTERNET: " <<endl << endl; 

				cout << "  Numero de l'ordinateur: " ; cin >> ordi_X ; cout << endl ;

				cout << "          Numero du Port: " ; cin >> Internet[ordi_X].PORT ;

				cout << endl << endl ;
				
			// Information Sur l'ordinateur Y
			cout << "Choisissez un ordinateur Y (0 et " << N-1 <<"): sur le RESEAU LOCAL: " << endl << endl;

				cout << "  Numero de l'ordinateur: " ; cin >> ordi_Y ; cout << endl ;

				cout << "          Numero du Port: " ; cin >> ReseauPrivee[ordi_Y].PORT ; cout << endl ;

				Direction_Envoi() ;
			
			cout << "Voulez vous envoyer un autre message ? (O/N): " ; cin >> choix ;

		} while((choix == 'o') || (choix == 'O')) ;
}
		
