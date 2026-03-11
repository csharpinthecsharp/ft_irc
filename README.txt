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
* - Parser( std::string port, std::string psswrd ) > Le constructeur qui permet d'initialiser le port et le password,
    Ce constructeur doit accepter le PORT + PASSWORD. 
        Sinon on throw une erreur et le programme se stop.
*
* - ~Parser() > Le destructeur.
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
