# >>>>>> = DEBUT DE FICHIER.
# >>> = LISTE FONCTIONS MEMBRES.
# * = SAUT DE LIGNES/CHANGEMENT DE SUJET.
# - = DEBUT DE FONCTION.
# <<<<<< = FIN DE FICHIER.

>>>>>> Parser.hpp ->
*
*
>>> Private:
* - unsigned int _port > Ce int contient le port que le user a entré.
*
* - std::string _psswrd > Ce std::string contient le password que le user a entré.
*
*
>>> Public:
* - Parser() > Le constructeur par défaut.
*
* - Parser( std::string port, std::string psswrd ) > Le constructeur, il va enregistrer les arguments (port + password).
*
* - ~Parser() > Le destructeur.
* 
* void Parser::checkArguments( const std::string& port, const std::string& psswrd ) const > Doit accepter le PORT + PASSWORD. 
                                                                                                Sinon on throw une erreur et le programme se stop.
*
* - unsigned int getPort() const > Fonction getter qui retourne le port.
*
* - unsigned int getPsswrd() const > Fonction getter qui retourne le password.
*
* - std::ostream &operator<<( std::ostream& os, const Parser& other ) > Overload pour nous montrer quel port et quel password à été choisis.
*
<<<<<< <- Parser.hpp
*
>>>>>> Exceptions.hpp ->
*
*
* - class WrongPortSizeException > Exception si la taille du port n'est pas entre 1 et 5.
*
* - class WrongDigitPortException > Exception si le port n'est pas seulement des digits.
*
* - class WrongPasswordException > Exception si le password est incorrect.
*
<<<<<< Exceptions.hpp ->
*
*
>>>>>> Server.hpp ->
* - Server() > Le constructeur par défaut, il appelle createSocket();
*
* - Server() > Le destructeur.
*
* - int createSocket(); > On créer un endpoint pour la communication, 
        socket() nous retourne le fd qui pointe dessus.
            si ça fail == -1.
*
<<<<<< <- Server.hpp
