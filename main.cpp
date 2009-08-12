#include <boost/test/unit_test.hpp>

#include "graph.hpp"
#include "inputToGraph.hpp"

#define BOOST_TEST_MODULE GraphTest

using namespace boost;
using namespace unit_test;
using namespace std;

static int numOfArgs;
static vector<string> arguments;

void add_node_test()
{
    cout << "-----------add_nodes_test----------" << endl;

    Graph G;

    std::string one("one");
    std::string two("two");
    std::string three("three");
    std::string four("four");
	
    G.addNode(one);
    G.addNode(two);
    BOOST_CHECK( G.getNumberOfNodes() == 2 );
    G.addNode(three);
    G.addNode(four);
	BOOST_CHECK( G.getNumberOfNodes() == 4 );

	G.addNode(one);
	BOOST_CHECK( G.getNumberOfNodes() == 4 );

    cout << "----------------------------------" << endl << endl;
}

void addNodes_test()
{
    cout << "-----------addNodes_test----------" << endl;
    Graph G;
    std::vector<std::string> nodeVector;
    nodeVector.push_back("A");
    nodeVector.push_back("B");
    nodeVector.push_back("C");
    nodeVector.push_back("D");
    nodeVector.push_back("E");
    nodeVector.push_back("F");
    nodeVector.push_back("G");
    nodeVector.push_back("H");
    addNodes(G, nodeVector );
    BOOST_CHECK( G.getNumberOfNodes() == 8 );
    cout << G.getNumberOfNodes() << endl;

    cout << "----------------------------------" << endl << endl;
}

void set_dependency_test ()
{
    cout << "--------set_dependency_test--------" << endl;

    Graph G;

    std::string one("one");
    std::string two("two");
    std::string three("three");
    std::string four("four");
    std::string eight("eight");

    G.addNode(one);
    G.addNode(two);
    G.addNode(three);
    G.addNode(four);
    
    G.setDependency(one, two);
    G.setDependency(one, three);
    G.setDependency(one, four);
    G.setDependency(two, three);  
    G.setDependency(two, four);
    G.setDependency(three, four);
	G.setDependency(one, eight);
	G.setDependency(eight, one);

    cout << "----------------------------------" << endl<< endl;
}

void get_Parents_and_Children_test()
{
    cout << "--get_Parents_and_Children_test---" << endl;
    cout << "----------------------------------" << endl<< endl;

}

void doesNodeExist_test()
{
    cout << "---------doesNodeExist_test-------" << endl;
    cout << "-----------------------------------" << endl<< endl;
}

void getRootNodes_getLeafNodes_test()
{
    cout << " ---getRootNodes_getLeafNodes_test--" << endl;
    cout << "-----------------------------------" << endl<< endl;
}

void appendNodefrom_removeNodefrom_test()
{
    cout << "-appendNodefrom_removeNodefrom_test-" << endl;
    Graph G;

    std::string one("one");
    std::string two("two");
    std::string three("three");
    std::string four("four");
    std::string eight("eight");

    G.addNode(one);
    G.addNode(two);
    G.addNode(three);
    G.addNode(four);

    G.setDependency(one, two);
    G.setDependency(one, three);
    G.setDependency(one, four);
    G.setDependency(two, three);  
    G.setDependency(two, four);
    G.setDependency(three, four);

    std::vector<std::string> nodeVector;

    appendNodeFrom( one, nodeVector );
    appendNodeFrom( two, nodeVector );
    BOOST_CHECK( nodeVector.size() == 2 );
    appendNodeFrom( one, nodeVector );
    BOOST_CHECK( nodeVector.size() == 2);

    removeNodeFrom( two, nodeVector );
    BOOST_CHECK( nodeVector.size() == 1 );
    removeNodeFrom( three, nodeVector );
    BOOST_CHECK( nodeVector.size() == 1 );
    removeNodeFrom( one, nodeVector );
    BOOST_CHECK( nodeVector.empty() );
    removeNodeFrom( one, nodeVector );
    BOOST_CHECK( nodeVector.empty() );

    cout << "-----------------------------------" << endl<< endl;
}

void removeNode_test()
{
    cout << "----------removeNode_test----------" << endl;
    cout << "-----------------------------------" << endl<< endl;
}

void topology_sort_test()
{
    cout << "--------topology_sort_test----------" << endl;

    Graph G;

    std::string A("A");
    std::string B("B");
    std::string C("C");
    std::string D("D");
    std::string E("E");
    std::string F("F");

    G.addNode(A);
    G.addNode(B);
    G.addNode(C);
    G.addNode(D);
    G.addNode(E);

    G.setDependency(A, B);
    G.setDependency(A, C);  
    G.setDependency(B, C);
    G.setDependency(C, D);
    G.setDependency(C, E);
    G.setDependency(D, E);

    std::vector<std::string> nodeVector;
    G.topologicalSortRoot( nodeVector );

    unsigned i(1);
    BOOST_CHECK( nodeVector[0] == A );
    BOOST_CHECK( nodeVector[1] == B );
    BOOST_CHECK( nodeVector[2] == C );
    BOOST_CHECK( nodeVector[3] == D );
    BOOST_CHECK( nodeVector[4] == E );
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin(); iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    G.addNode(A);
    G.addNode(B);
    G.addNode(C);
    G.addNode(D);
    G.addNode(E);
    G.addNode(F);

    G.setDependency( A, B );
    G.setDependency( A, C );
    G.setDependency( D, E );
    G.setDependency( D, F );
    G.topologicalSortRoot(nodeVector);
    BOOST_CHECK( nodeVector[0] == A );
    BOOST_CHECK( nodeVector[1] == D );
    BOOST_CHECK( nodeVector[2] == B );
    BOOST_CHECK( nodeVector[3] == C );
    BOOST_CHECK( nodeVector[4] == E );
    BOOST_CHECK( nodeVector[5] == F );

    i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    //***********Using Boost_Shared_Ptr leaks memory if the node removal algorithm is 
    //***********given a cyclic dependency Graph. 
    //G.addNode(A);
    //G.addNode(B);
    //G.addNode(C);
    //G.addNode(D);
    //G.setDependency( A, B );
    //G.setDependency( B, C );
    //G.setDependency( C, D );
    //G.setDependency( D, B );
    //G.topologicalSortRoot( nodeVector );
    //i = 1;
    //for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
    //    cout << i << " : " << (*iter) << endl;
    //************

    cout << "-----------------------------------" << endl<< endl;
}

void toplogicalSortSafe_test()
{
    cout << "------toplogicalSortSafe_test------" << endl;

    Graph G;

    std::string A("A");
    std::string B("B");
    std::string C("C");
    std::string D("D");
    std::string E("E");
    std::string F("F");

    G.addNode(A);
    G.addNode(B);
    G.addNode(C);
    G.addNode(D);
    G.addNode(E);
    G.addNode(F);

    G.setDependency( A, B );
    G.setDependency( A, C );
    G.setDependency( D, E );
    G.setDependency( D, F );

    std::vector<std::string> nodeVector;

    G.topologicalSort( nodeVector );
    BOOST_CHECK( nodeVector[0] == A );
    BOOST_CHECK( nodeVector[1] == D );
    BOOST_CHECK( nodeVector[2] == B );
    BOOST_CHECK( nodeVector[3] == C );
    BOOST_CHECK( nodeVector[4] == E );
    BOOST_CHECK( nodeVector[5] == F );
    unsigned i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;


    G.addNode( D );
    G.setDependency( D, A );
    G.setDependency( D, E );
    G.setDependency( D, F );
    G.setDependency( D, B );
    G.setDependency( D, C );
    G.topologicalSort( nodeVector );
    i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    Graph cycG;

    cycG.addNode( A );
    cycG.addNode( B );
    cycG.addNode( C );
    cycG.addNode( D );

    cycG.setDependency( A, B );
    cycG.setDependency( C, B );
    cycG.setDependency( D, B );
    cycG.setDependency( C, D );

    cycG.topologicalSort( nodeVector );
    BOOST_CHECK( nodeVector[0] == A );
    BOOST_CHECK( nodeVector[1] == C );
    BOOST_CHECK( nodeVector[2] == D );
    BOOST_CHECK( nodeVector[3] == B );
    i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    cout << "-----------------------------------" << endl<< endl;
}

void randomTests()
{
    cout << "------------randomTests------------" << endl;

    Graph DAG;

    std::string A("A");
    std::string B("B");
    std::string C("C");
    std::string D("D");
    std::string E("E");
    std::string F("F");
    std::string G("G");
    std::string H("H");

    DAG.addNode(A);
    DAG.addNode(B);
    DAG.addNode(C);
    DAG.addNode(D);
    DAG.addNode(E);
    DAG.addNode(F);
    DAG.addNode(G);
    DAG.addNode(H);

    DAG.setDependency(A,B);
    DAG.setDependency(A,C);
    DAG.setDependency(C,D);
    DAG.setDependency(D,E);
    DAG.setDependency(E,F);
    DAG.setDependency(F,G);
    DAG.setDependency(G,H);
    DAG.setDependency(A,H);
    DAG.setDependency(B,D);
    DAG.setDependency(B,E);
    DAG.setDependency(D,G);
    DAG.setDependency(B,H);

    std::string node("ABCDEFGHIJKLMNOP");
    std::string::iterator nodeIter;
    cout << node.size() << endl;
    unsigned j(1);
    for ( nodeIter = node.begin() ; nodeIter != node.end(); ++nodeIter, ++j )
        cout << j << " : " << (*nodeIter) << endl;

    std::vector<std::string> nodeVector;

    DAG.topologicalSort( nodeVector );

    unsigned i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    cout << "-----------------------------------" << endl<< endl;
}

void consoleInputTest()
{
    cout << "------------consoleInputTest------------" << endl;
    
    cout <<  numOfArgs  << endl;
    vector<string>::reverse_iterator r_iter;
    for (r_iter = arguments.rbegin(); r_iter != arguments.rend(); ++r_iter )
    {
        cout << *r_iter << endl;
    }
    Graph G;
    if ( arguments[1] == "-help" )
    {
        inputToGraphHelp();
    }
    else if ( arguments[1] == "-console" )
    {
        cout << "Reading from console" << endl;
        consoleToGraph(G, *( arguments.rbegin() ));
    }
    
    std::vector<std::string> nodeVector;
    G.topologicalSort( nodeVector );

    unsigned i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        cout << i << " : " << (*iter) << endl;

    cout << "-----------------------------------" << endl<< endl;
}

void collectiveGraphSetupTest()
{
    cout << "------------collectiveGraphSetupTest------------" << endl;
    Graph G;

    std::vector<std::string> nodeVector;

    nodeVector.push_back("A");
    nodeVector.push_back("B");
    nodeVector.push_back("C");
    nodeVector.push_back("D");
    nodeVector.push_back("E");
    nodeVector.push_back("F");
    nodeVector.push_back("G");
    nodeVector.push_back("H");

    std::vector< std::pair< std::string, std::string> > nodePairVector;

    nodePairVector.push_back( std::pair<std::string, std::string>("A" , "B") );
    nodePairVector.push_back( std::pair<std::string, std::string>("A" , "C") );
    nodePairVector.push_back( std::pair<std::string, std::string>("A" , "D") );
    nodePairVector.push_back( std::pair<std::string, std::string>("A" , "E") );
    nodePairVector.push_back( std::pair<std::string, std::string>("A" , "F") );
    nodePairVector.push_back( std::pair<std::string, std::string>("B" , "G") );

    addNodes(G, nodeVector );
    setDependencies(G, nodePairVector );

    BOOST_CHECK( G.getNumberOfNodes() == 8 );


    cout << "-----------------------------------" << endl<< endl;
}

test_suite* init_unit_test_suite( int argc, char * argv[] )
{

    numOfArgs = argc;
    
    for (int i=0; i < numOfArgs; ++i)
    {
        arguments.push_back( argv[i] );
    }

    test_suite* ts_01 = BOOST_TEST_SUITE( "test_cases_01" );
    ts_01->add( BOOST_TEST_CASE (&add_node_test) );
    ts_01->add( BOOST_TEST_CASE (&addNodes_test) );
    ts_01->add( BOOST_TEST_CASE (&set_dependency_test) );
    ts_01->add( BOOST_TEST_CASE (&collectiveGraphSetupTest) );
    ts_01->add( BOOST_TEST_CASE (&get_Parents_and_Children_test) );
    ts_01->add( BOOST_TEST_CASE (&doesNodeExist_test) );
    ts_01->add( BOOST_TEST_CASE (&getRootNodes_getLeafNodes_test) );
    ts_01->add( BOOST_TEST_CASE (&appendNodefrom_removeNodefrom_test) );
    ts_01->add( BOOST_TEST_CASE (&removeNode_test) );
    ts_01->add( BOOST_TEST_CASE (&topology_sort_test) );
    ts_01->add( BOOST_TEST_CASE (&toplogicalSortSafe_test) );
    ts_01->add( BOOST_TEST_CASE (&randomTests) );
    ts_01->add( BOOST_TEST_CASE (&consoleInputTest) );

    framework::master_test_suite().add( ts_01 );
    return 0;
}
