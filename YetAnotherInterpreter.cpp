#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <memory>

#include "Exceptions.h"
#include "Token.h"
#include "Tokenizer.h"
#include "NodeManager.h"
#include "Parser.h"
#include "Visitor.h"
#include "EvaluationVisitor.h"
#include "PrintVisitor.h"

// Facciamo in modo che l'espressione sia letta da file
// Introduciamo la parte relativa alla tokenizzazione del file
// Introduciamo le eccezioni per la gestione delle anomalie

int main(int argc, char* argv[])
{
    // Controllo di avere abbastanza parametri (mi serve un parametro con 
    // il nome del file di testo contenente l'espressione da valutare)
    // Il primo elemento di argv � sempre il nome del programma eseguibile
    // Gli elementi dal secondo in poi sono i parametri da linea di comando
    if (argc < 2) {
        std::cerr << "File non specificato!" << std::endl;
        std::cerr << "Utilizzo: " << argv[0] << " <nome_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream inputFile;
    // Il nome passato dall'utente potrebbe non corrispondere ad un file esistente
    // Se tento di aprire un file non esistente il programma normalmente va in crash
    // Posso "intercettare" questo evento e gestirlo
    try {
        inputFile.open(argv[1]);
    } catch (std::exception& exc) {
        // exc � un oggetto della classe std::exception e viene costruito nel
        // momento in cui si genera una condizione di errore; si dice che l'eccezione
        // (l'oggetto exc) viene "propagata" dalla funzione che ha generato l'errore
        // all'indietro nello stack di esecuzione fino a che non incontra un "catch"
        std::cerr << "Non posso aprire " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
    // NOTA: std::exception � la classe base di tutte le eccezioni in CC+
    // Intercettando un oggetto eccezione come std::exception is intercettano tutti
    // gli oggetti delle possibili sottoclassi

    // Fase di Tokenizzazione
    // L'oggetto tokenize trasforma il file di input in una sequenza di token
    Tokenizer tokenize;
    std::vector<Token> inputTokens;

    try {
        // Se non ci fidiamo del fatto che il C++ ottimizzi il ritorno del vector
        // dei token spostando il contenuto invece che copiandolo, possiamo rendere
        // l'utilizzo del move constructor esplicito con la primitiva std::move
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close();
    } catch (LexicalError& le) {
        std::cerr << "Error in Tokenizer: ";
        std::cerr << le.what() << std::endl;
        return EXIT_FAILURE;
    } catch (std::exception& exc) {
        std::cerr << "Non posso leggere da " << argv[1] << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    //for (Token t : inputTokens) {
    //    std::cout << t << std::endl;
    //}

    // Fase di Parsing

    // Creo il manager per i nodi
    NodeManager manager;

    // Creo il function object per il parsing
    ParseProgram parse{ manager };

    try {
        Program* progr = parse(inputTokens);
        // Print dell'albero sintattico
        //parse.printSyntaxTree(progr, 0);
        
        std::unique_ptr<EvaluationVisitor> evalVisitor = std::make_unique<EvaluationVisitor>();
        progr->accept(evalVisitor.get());

    } catch (ParseError& pe) {
        std::cerr << "Error in Parser: ";
        std::cerr << pe.what() << std::endl;
        manager.clearMemory();
        return EXIT_FAILURE;
    }  catch (EvalError& ee) {
        std::cerr << "Error in evaluator: ";
        std::cerr << ee.what() << std::endl;
        manager.clearMemory();
        return EXIT_FAILURE;
    } catch (std::exception& exc) {
        // Catturo qualsiasi altra eccezione si verifichi
        std::cerr << "Errore generico: ";
        std::cerr << exc.what() << std::endl;
        manager.clearMemory();
        return EXIT_FAILURE;
    }
 
    return EXIT_SUCCESS;
    
}




