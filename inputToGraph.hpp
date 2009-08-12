#ifndef INPUTTOGRAPH_HPP_INCLUDED
#define INPUTTOGRAPH_HPP_INCLUDED

#include "graph.hpp"
#include <string>
#include <iostream>

using namespace std;

void inputToGraphHelp( void )
{
    cout << "For an edge from node a to node b : \"-console (a,b)\"" << endl;
    cout << "For multiple edges and nodes : '-console (a,b)(c,d)'...etc." << endl;
    cout << "For file with edge and node description : \"-file <filename>\"" << endl;
    cout << "To run the Unit tests : \"-test\"" << endl;
}

void consoleToGraph( Graph& G, const string& aString )
{
    std::string node1, node2; 
    string::const_iterator iter;
    char openingBracket('(');
    char closingBracket(')');
    char comma(',');
    string firstNode;
    string secondNode;
    std::vector<std::string> allNodesVec;
    std::vector< std::pair< std::string, std::string> > allPairsVec;

    for ( iter = aString.begin(); iter != aString.end(); ++iter )
    {
        if ( *iter == openingBracket )
        {
            if( iter != aString.end() )
                ++iter;
            else
            {
                cout << "Out of bounds!" << endl;
                break;
            }
            if ( (*iter != openingBracket) && (*iter != comma) && (*iter != closingBracket) )
            {
                firstNode = *iter;
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
                cout << "First Node = " << firstNode;
#endif
            } 
            else
            {
                cout << "Error, not correctly formatted input!" << endl;
                break;
            }
            //////////////////////////////////////////////////////////////////////////
            if( iter != aString.end() )
                ++iter;
            else
            {
                cout << "Out of bounds!" << endl;
                break;
            }
            if ( *iter != comma)
            {
                cout << "Error, not correctly formatted input!" << endl;
                break;
            }
            else
            {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
                cout << " " << *iter << " ";
#endif
            }
            if( iter != aString.end() )
                ++iter;
            else
            {
                cout << "Out of bounds!" << endl;
                break;
            }
            if ( (*iter != openingBracket) && (*iter != comma) && (*iter != closingBracket) )
            {
                secondNode = *iter;
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
                cout << "Second Node = " << secondNode << endl;
#endif
            } 
            else
            {
                cout << "Error, not correctly formatted input!" << endl;
                break;
            }
            //////////////////////////////////////////////////////////////////////////
            if( iter != aString.end() )
                ++iter;
            else
            {
                cout << "Out of bounds!" << endl;
                break;
            }
            if ( *iter == closingBracket )
            {
                appendUniqueNodeToNodeVec( firstNode, allNodesVec );
                appendUniqueNodeToNodeVec( secondNode, allNodesVec );
                allPairsVec.push_back( std::pair<std::string, std::string>(firstNode, secondNode) );
            }
            else
            {
                cout << "Error, not correctly formatted input!" << endl;
                break;
            }
        }
        else if ( *iter == closingBracket )
        {
            cout << "Error, not correctly formatted input : Incorrect occurrence of closing bracket ')' " << endl;
            break;
        }
        else if ( *iter == comma )
        {
            cout << "Error, not correctly formatted input : Incorrect occurrence of comma ',' " << endl;
            break;
        }
        else
        {
            cout << "Error, not correctly formatted input" << endl;
            break;
        }
    }
    cout << "Setting up Graph with : " << allNodesVec.size() << " nodes and " << allPairsVec.size() << " edges." << endl;
    addNodes( G, allNodesVec );
    setDependencies(G, allPairsVec );
}

#endif //INPUTTOGRAPH_HPP_INCLUDED
