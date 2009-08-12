#include "topologicalSort_test_suite.hpp"
//////////////////////////////////////////////////////////////////////////
void GraphNodeSetUp_test( void )
{
    boost::shared_ptr<GraphNode> A(new GraphNode("A"));

    std::cout << A->getName() << std::endl;

    assert( A->hasChildren() == false );
    assert( A->hasParents() == false );
    assert( A->getNumberOfParents() == 0 );
    assert( A->getNumberOfChildren() == 0 );

    A->printGraphNodeState();
}
//////////////////////////////////////////////////////////////////////////
void GraphNodeDependencySetup_test( void )
{
    boost::shared_ptr<GraphNode> A(new GraphNode("A"));
    boost::shared_ptr<GraphNode> B(new GraphNode("B"));

    A->addChild( B );

    assert( A->hasChildren() == true );
    assert( A->hasParents() == false );
    assert( A->getNumberOfChildren() == 1 );
    assert( A->getNumberOfParents() == 0 );
    assert( A->doesChildExist("B") == true );
    assert( A->doesParentExist("B") == false );

    A->printGraphNodeState();

    assert( B->hasChildren() == false );
    assert( B->hasParents() == true );
    assert( B->getNumberOfChildren() == 0 );
    assert( B->getNumberOfParents() == 1 );
    assert( B->doesChildExist("A") == false );
    assert( B->doesParentExist("A") == true );

    B->printGraphNodeState();
}
//////////////////////////////////////////////////////////////////////////
void add_node_test( void )
{
    Graph G;

    std::string one("one");
    std::string two("two");
    std::string three("three");
    std::string four("four");

    G.addNode(one);
    G.addNode(two);
    assert( G.getNumberOfNodes() == 2 );

    G.addNode(three);
    G.addNode(four);
    assert( G.getNumberOfNodes() == 4 );

    G.addNode(one);
    assert( G.getNumberOfNodes() == 4 );
}
//////////////////////////////////////////////////////////////////////////
void addNodes_test( void )
{
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

    assert( G.getNumberOfNodes() == 8 );
    
    std::cout << G.getNumberOfNodes() << std::endl;
}
//////////////////////////////////////////////////////////////////////////
void set_dependency_test ( void )
{
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
}
//////////////////////////////////////////////////////////////////////////
void setDependencies_test( void )
{
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

    assert( G.getNumberOfNodes() == 8 );
}
//////////////////////////////////////////////////////////////////////////
void appendNodefrom_removeNodefrom_test( void )
{
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
    assert( nodeVector.size() == 2 );

    appendNodeFrom( one, nodeVector );
    assert( nodeVector.size() == 2);

    removeNodeFrom( two, nodeVector );
    assert( nodeVector.size() == 1 );

    removeNodeFrom( three, nodeVector );
    assert( nodeVector.size() == 1 );

    removeNodeFrom( one, nodeVector );
    assert( nodeVector.empty() );

    removeNodeFrom( one, nodeVector );
    assert( nodeVector.empty() );
}
//////////////////////////////////////////////////////////////////////////
void topologySortRoot_test( void )
{
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

    G.setDependency(A, B);
    G.setDependency(A, C);  
    G.setDependency(D, E);
    G.setDependency(D, F);

    std::vector<std::string> nodeVector;

    G.topologicalSortRoot( nodeVector );

    assert( nodeVector[0] == A );
    assert( nodeVector[1] == D );
    assert( nodeVector[2] == B );
    assert( nodeVector[3] == C );
    assert( nodeVector[4] == E );
    assert( nodeVector[5] == F );

    unsigned i(1);
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin(); iter != nodeVector.end(); ++iter, ++i )
        std::cout << i << " : " << (*iter) << std::endl;

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
}
//////////////////////////////////////////////////////////////////////////
void topologicalSort_test( void )
{
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

    assert( nodeVector[0] == A );
    assert( nodeVector[1] == D );
    assert( nodeVector[2] == B );
    assert( nodeVector[3] == C );
    assert( nodeVector[4] == E );
    assert( nodeVector[5] == F );

    unsigned i( 1 );
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        std::cout << i << " : " << (*iter) << std::endl;

    G.addNode( D );

    G.setDependency( D, A );
    G.setDependency( D, E );
    G.setDependency( D, F );
    G.setDependency( D, B );
    G.setDependency( D, C );

    G.topologicalSort( nodeVector );

    i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        std::cout << i << " : " << (*iter) << std::endl;

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
    assert( nodeVector[0] == A );
    assert( nodeVector[1] == C );
    assert( nodeVector[2] == D );
    assert( nodeVector[3] == B );

    i = 1;
    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
        std::cout << i << " : " << (*iter) << std::endl;
}
//////////////////////////////////////////////////////////////////////////
//void consoleInputTest( void )
//{
//    cout <<  numOfArgs  << endl;
//    vector<string>::reverse_iterator r_iter;
//    for (r_iter = arguments.rbegin(); r_iter != arguments.rend(); ++r_iter )
//    {
//        cout << *r_iter << endl;
//    }
//    Graph G;
//    if ( arguments[1] == "-help" )
//    {
//        inputToGraphHelp();
//    }
//    else if ( arguments[1] == "-console" )
//    {
//        cout << "Reading from console" << endl;
//        consoleToGraph(G, *( arguments.rbegin() ));
//    }
//
//    std::vector<std::string> nodeVector;
//    G.topologicalSort( nodeVector );
//
//    unsigned i = 1;
//    for ( std::vector<std::string>::const_iterator iter = nodeVector.begin() ; iter != nodeVector.end(); ++iter, ++i )
//        cout << i << " : " << (*iter) << endl;
//}
//////////////////////////////////////////////////////////////////////////
